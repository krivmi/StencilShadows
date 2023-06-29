// PG Tools, (c)2021-2022 Tomas Fabian
// 
// May only be used for non-commercial educational purposes at VSB-TUO, FEECS,
// Dept. of Computer Science and may not be shared with any third parties in any form.

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "vector2.h"
#include "vector3.h"
#include "texture.h"

/*! \def IOR_AIR
\brief Index lomu vzduchu za norm�ln�ho tlaku.
*/
#define IOR_AIR 1.000293f

/*! \def IOR_AIR
\brief Index lomu vody.
*/
#define IOR_WATER 1.33f

/*! \def IOR_GLASS
\brief Index lomu skla.
*/
#define IOR_GLASS 1.5f

/* types of shaders */
enum class Shader : int { NONE = 0, NORMAL = 1, LAMBERT = 2, PHONG = 3, GLASS = 4, MODIFIED_PHONG = 5, MIRROR = 6, TS = 7, COOK_TORRANCE_GGX = 8, GGX = 9, TRIAL = 10 };

/* types of maps */
enum class Map : char { kDiffuse = 0, kSpecular, kGlossiness, kBase, kRoughness, kMetallic, kRMA, kNormal, kOpacity, kEmission, kMapsCount };

/*
PBR workflows:
 - Specular - glossiness: diffuse (albedo), specular, glossiness
 - Metal - roughness: base, roughness, metallic

 Maps common to both worflows: normal
*/

/*! \class Material
\brief A simple material.

\author Tom� Fabi�n
\version 0.9
\date 2011-2021
*/
class Material
{
public:
	//! Implicitn� konstruktor.
	/*!
	Inicializuje v�echny slo�ky materi�lu na v�choz� matn� �ed� materi�l.
	*/
	Material();

	Material( const std::string & name );

	//! Specializovan� konstruktor.
	/*!
	Inicializuje materi�l podle zadan�ch hodnot parametr�.

	\param name n�zev materi�lu.
	\param ambient barva prost�ed�.
	\param diffuse barva rozptylu.
	\param specular barva odrazu.
	\param emission barva emise.
	\param shininess lesklost.
	\param ior index lomu.
	\param shader shader to be used.
	\param textures pole ukazatel� na textury.
	\param no_textures d�lka pole \a textures. Maxim�ln� \a NO_TEXTURES - 1.
	*/
	Material( std::string & name, const Color3f & ambient, const Color3f & diffuse,
		const Color3f & specular, const Color3f & emission, const float reflectivity,
		const float shininess, const float ior, const Shader shader,
		Texture3u ** textures = NULL, const int no_textures = 0 );

	//! Destruktor.
	/*!
	Uvoln� v�echny alokovan� zdroje.
	*/
	~Material();

	//void Print();

	//! Nastav� n�zev materi�lu.
	/*!
	\param name n�zev materi�lu.
	*/
	void set_name( const char * name );

	//! Vr�t� n�zev materi�lu.
	/*!
	\return N�zev materi�lu.
	*/
	std::string name() const;

	//! Nastav� texturu.
	/*!
	\param slot ��slo slotu, do kter�ho bude textura p�i�azena. Maxim�ln� \a kMapCount - 1.
	\param texture ukazatel na texturu.
	*/
	void set_texture( const Map type, std::shared_ptr<Texture3u> texture );

	//! Vr�t� texturu.
	/*!
	\param slot ��slo slotu textury. Maxim�ln� \a kMapCount - 1.
	\return Ukazatel na zvolenou texturu.
	*/
	std::shared_ptr<Texture3u> texture( const Map type ) const;

	Shader shader() const;

	void set_shader( Shader shader );

	Color3f ambient( const Vector2 * tex_coord = nullptr ) const;
	Color3f & value( const Map type );
	Color3f value( const Map type, const Vector2 * tex_coord = nullptr ) const;
	void set_value( const Map type, const Color3f & value );
	//Color3f diffuse( const Vector2 * tex_coord = nullptr ) const;
	//Color3f specular( const Vector2 * tex_coord = nullptr ) const;
	Color3f albedo( const Vector2 * tex_coord = nullptr ) const;
	Vector3 normal( const Vector2 * tex_coord ) const;
	float shininess( const Vector2 * tex_coord ) const;
	void set_shininess( const float shininess );
	float roughness( const Vector2 * tex_coord = nullptr ) const;
	float metallic( const Vector2 * tex_coord = nullptr ) const;
	Color3f emission( const Vector2 * tex_coord = nullptr ) const;
	Color3f attenuation( const Vector2 * tex_coord = nullptr ) const;

public:
	Color3f ambient_; /*!< RGB barva prost�ed� \f$\left<0, 1\right>^3\f$. */
	//Color3f diffuse_; /*!< RGB barva rozptylu \f$\left<0, 1\right>^3\f$. */
	//Color3f specular_; /*!< RGB barva odrazu \f$\left<0, 1\right>^3\f$. */

	Color3f emission_; /*!< RGB barva emise \f$\left<0, 1\right>^3\f$. */
	
	float roughness_; /*!< Koeficient drsnosti. */
	float metallic_; /*!< Koeficient kovovosti. */
	float reflectivity; /*!< Koeficient odrazivosti. */
	float ior; /*!< Index lomu. */
	Color3f attenuation_; /* the (Napierian) attenuation coefficient */

	Shader shader_{ Shader::NORMAL }; /*!< Type of used shader. */

private:
	std::array<Color3f, int( Map::kMapsCount )> values_; /*!< array of linear values */
	std::array<std::shared_ptr<Texture3u>, int( Map::kMapsCount )> textures_{ nullptr }; /*!< Pole ukazatel� na textury. */

	float shininess_; /* shininess aka specular power exponent */

	std::string name_; /*!< Material name. */
};

#endif // MATERIAL_H_
