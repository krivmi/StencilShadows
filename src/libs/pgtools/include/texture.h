// PG Tools, (c)2021-2022 Tomas Fabian
// 
// May only be used for non-commercial educational purposes at VSB-TUO, FEECS,
// Dept. of Computer Science and may not be shared with any third parties in any form.

#ifndef TEXTURE_H_
#define TEXTURE_H_

#define FAST_INTERP

#include <vector>
#include <string>
#include <freeimage.h>

#include "color.h"

FIBITMAP * BitmapFromFile( const char * file_name, int & width, int & height );
// Note that all float images in FreeImage are forced to have a range in <0, 1> after applying build-in conversions!!!
// see https://sourceforge.net/p/freeimage/bugs/259/
FIBITMAP * Custom_FreeImage_ConvertToRGBF( FIBITMAP * dib ); // this fix removes clamp from conversion of float images
FIBITMAP * Custom_FreeImage_ConvertToRGBAF( FIBITMAP * dib );  // this fix removes clamp from conversion of float images

/*! \class Texture
\brief A simple templated representation of texture.

Texture3f normal = Texture3f( "../../../data/denoise/normal_100spp.exr");
Texture3f output = Texture3f( width, height );
output.Save( "denoised.exr" );

\author Tomáš Fabián
\version 1.1
\date 2020
*/

template <class T, FREE_IMAGE_TYPE F>
class Texture
{
public:
	Texture( const int width, const int height )
	{
		assert( width > 0 && height > 0 );

		width_ = width;
		height_ = height;

		data_.resize( size_t( width ) * size_t( height ) );
	}

	Texture( const int width, const int height, const int pixel_stride, T * data )
	{
		assert( width > 0 && height > 0 && pixel_stride > 0 && data );

		width_ = width;
		height_ = height;

		data_.resize( size_t( width ) * size_t( height ) );

		for ( auto & pixel : data_ )
		{
			pixel = *data;
			data = ( T * )( ( ( BYTE * )data ) + pixel_stride );
		}
	}

	Texture( const std::string & file_name, const bool swap_RB = false )
	{
		FIBITMAP * dib = BitmapFromFile( file_name.c_str(), width_, height_ );

		if ( dib )
		{
			if ( true ) // always make sure that the loaded bitmap will fit the allocated data size
			{
				FIBITMAP * const dib_new = Convert( dib );
				assert( dib_new );
				FreeImage_Unload( dib );
				dib = dib_new;
			}

			data_.resize( size_t( width_ ) * size_t( height_ ) );

			const int scan_width = FreeImage_GetPitch( dib ); // (bytes)
			const int bpp = FreeImage_GetBPP( dib ); // (bites)

			assert( bpp == sizeof( T ) * 8 );

			FreeImage_ConvertToRawBits( ( BYTE * )( data_.data() ), dib, scan_width, bpp,
				FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, TRUE );

			if ( swap_RB && ( bpp == 24 || bpp == 96 ) )
			{
				for ( auto & data : data_ )
				{
					data.reverse();
				}
			}

			FreeImage_Unload( dib );
			dib = nullptr;

			double range[] = { ( std::numeric_limits<double>::max )( ), std::numeric_limits<double>::lowest() };

			for ( const auto & pixel : data_ )
			{
				range[0] = min( range[0], double( pixel.min_value() ) );
				range[1] = max( range[1], double( pixel.max_value() ) );
			}

			printf( "Texture '%s' (%d x %d px, %d bpp, <%0.3f, %0.3f>, %0.1f MB) loaded.\n",
				file_name.c_str(), width_, height_, bpp, range[0], range[1],
				scan_width * height_ / ( 1024.0f * 1024.0f ) );
		}
		else
		{
			printf( "Texture '%s' not loaded.\n", file_name.c_str() );
		}
	}

	Texture<T, F> Roi( const int x0, const int y0, const int x1, const int y1 )
	{
		Texture<T, F> roi( x1 - x0, y1 - y0 );

		for ( int y = y0; y < y1; ++y )
		{
			for ( int x = x0; x < x1; ++x )
			{
				roi.set_pixel( x - x0, y - y0, pixel( x, y ) );
			}
		}

		return roi;
	}

	T pixel( const int x, const int y ) const
	{
		assert( x >= 0 && x < width_ && y >= 0 && y < height_ );

		return data_[size_t( x ) + size_t( y ) * size_t( width_ )];
	}

	void set_pixel( const int x, const int y, const T & color )
	{
		assert( x >= 0 && x < width_ && y >= 0 && y < height_ );

		data_[size_t( x ) + size_t( y ) * size_t( width_ )] = color;
	}

	T texel( const float u, const float v ) const
	{
		T value;

#ifdef FAST_INTERP
		// nearest neighbour
		value = pixel(
			max( 0, min( int( u * width_ ), width_ - 1 ) ),
			max( 0, min( int( v * height_ ), height_ - 1 ) ) );
#else
		// bilinear interpolation	
		int i0 = max( 0, min( int( u * width_ ), width_ - 1 ) );
		int j0 = max( 0, min( int( v * height_ ), height_ - 1 ) );
		int i1 = min( i0 + 1, width_ - 1 );
		int j1 = min( j0 + 1, height_ - 1 );

		Color3f v00 = Color3f( pixel( i0, j0 ) );
		Color3f v10 = Color3f( pixel( i1, j0 ) );
		Color3f v01 = Color3f( pixel( i0, j1 ) );
		Color3f v11 = Color3f( pixel( i1, j1 ) );

		float x = max( 0.0f, min( 1.0f, u * width_ - i0 ) );
		float y = max( 0.0f, min( 1.0f, v * height_ - j0 ) );

		if ( ( x < 0.0f || x > 1.0f ) || ( y < 0.0f || y > 1.0f ) )
		{
			printf( "" );
		}

		//assert( x >= 0.0f && x <= 1.0f );
		//assert( y >= 0.0f && y <= 1.0f );

		Color3f tmp = ( v00 * ( 1.0f - x ) + v10 * x ) * ( 1.0f - y ) + ( v01 * ( 1.0f - x ) + v11 * x ) * y;
		value = T( tmp );
#endif		

		return value;
	}

	int width() const
	{
		return width_;
	}

	int height() const
	{
		return height_;
	}

	T * data()
	{
		return data_.data();
	}

	size_t size() const
	{
		return row_stride() * height_;
	}

	size_t pixel_stride() const
	{
		return sizeof( T );
	}

	size_t row_stride() const
	{
		return pixel_stride() * width_;
	}

	FIBITMAP * Convert( FIBITMAP * dib )
	{
		throw "Convert method is defined only for particular Texture types";

		return nullptr;
	}

	void set( void * pixel, int pixel_size, int pixel_stride, void * alpha = nullptr, int alpha_size = 0, int alpha_stride = 0 )
	{
		throw "Set method is defined only for particular Texture types";
	}

	/*static Texture Load( const std::string & file_name )
	{
		throw "Load method is defined only for particular Texture types";
	}*/

	void Save( const std::string & file_name ) const
	{
		FIBITMAP * bitmap = FreeImage_AllocateT( F, width_, height_, sizeof( T ) * 8 ); // FIT_BITMAP, FIT_BITMAP, FIT_RGBF, FIT_RGBAF
		BYTE * data = ( BYTE * )( FreeImage_GetBits( bitmap ) );
		const int scan_width = FreeImage_GetPitch( bitmap );
		memcpy( data, data_.data(), scan_width * height_ );
		FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename( file_name.c_str() );
		if ( fif != FIF_PFM )
		{
			FreeImage_FlipVertical( bitmap );
		}
		if ( FreeImage_Save( fif, bitmap, file_name.c_str() ) )
		{
			printf( "Texture has been saved successfully in '%s'.\n", file_name.c_str() );
		}
		else
		{
			printf( "Texture failed to save in '%s'.\n", file_name.c_str() );
		}
		FreeImage_Unload( bitmap );
		bitmap = nullptr;
		data = nullptr;
	}

private:
	std::vector<T> data_;

	int width_{ 0 };
	int height_{ 0 };
};

using Texture3f = Texture<Color3f, FIT_RGBF>;
using Texture4f = Texture<Color4f, FIT_RGBAF>;
using Texture3u = Texture<Color3u, FIT_BITMAP>;
using Texture4u = Texture<Color4u, FIT_BITMAP>;

template<>
FIBITMAP * Texture3u::Convert( FIBITMAP * dib );

template<>
FIBITMAP * Texture4u::Convert( FIBITMAP * dib );

template<>
FIBITMAP * Texture3f::Convert( FIBITMAP * dib );

template<>
FIBITMAP * Texture4f::Convert( FIBITMAP * dib );

template<>
void Texture4u::set( void * pixel, int pixel_size, int pixel_stride, void * alpha, int alpha_size, int alpha_stride );

#endif // TEXTURE_H_
