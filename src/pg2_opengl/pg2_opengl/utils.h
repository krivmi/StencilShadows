#ifndef UTILS_H_
#define UTILS_H_

#define MAT_ELEM( mat, type, x, y ) reinterpret_cast<type *>( ( mat ).data + \
	( mat ).step * ( y ) + ( mat ).elemSize() * ( x ) )

#define SAFE_DELETE( p ) { \
	if ( p != NULL ) \
	{ \
		delete p; \
		p = NULL; \
	} \
}

#define SAFE_DELETE_ARRAY( p ) { \
	if ( p != NULL ) \
	{ \
		delete [] p; \
		p = NULL; \
	} \
}

/*! \fn float template<typename T> void SafeDeleteVectorItems( std::vector<T> v )
\brief Dealokuje všechny prvky typu T vektoru v.
\param v Standardní vektor.
*/
template<typename T> void SafeDeleteVectorItems( std::vector<T> v )
{
	while ( v.size() > 0 )
	{
		T item = v.back();
		v.pop_back();
		SAFE_DELETE( item );
	}
}

namespace utils
{
	/*! \fn void swap( T & a, T & b )
	\brief Prohodí hodnoty a, b.
	\param a Prvni T hodnota.
	\param b Druha T hodnota.
	*/
	template<typename T> void swap( T & a, T & b )
	{
		const T tmp = a;
		a = b;
		b = tmp;
	}
}

/*! \fn float Random( const float range_min, const float range_max )
\brief Vrátí pseudonáhodné èíslo s normálním rozdìlením v intervalu <range_min, range_max).
\param range_min Dolní mez intervalu.
\param range_max Horní mez intervalu.
\return Pseudonáhodné èíslo.
*/
float Random( const float range_min = 0.0f, const float range_max = 1.0f );

/*! \fn long long GetFileSize64( const char * file_name )
\brief Vrátí velikost souboru v bytech.
\param file_name Úplná cesta k souboru
*/
long long GetFileSize64( const char * file_name );

/*! \fn void PrintTime( double t )
\brief Vytiskne na stdout èas ve formátu Dd:Mm:Ss.
\param t èas v sekundách.
*/
void PrintTime( double t, char * buffer );

/*! \fn std::string TimeToString( const double t )
\brief Pøevede èas v sekundách do øetìzce ve formátu Dd:Mm:Ss nebo Ss.m nebo m.u
\param t èas v sekundách.
*/
std::string TimeToString( const double t );

/*! \fn char * LTrim( char * s )
\brief Oøeže øetìzec o bílé znaky zleva.
\param s ukazatel na øetìzec
\return Ukazatel na novou pozici v tomtéž øetìzci
*/
char * LTrim( char * s );

/*! \fn char * RTrim( char * s )
\brief Oøeže øetìzec o bílé znaky zprava.
\param s ukazatel na øetìzec
\return Ukazatel na novou pozici v tomtéž øetìzci
*/
char * RTrim( char * s );

/*! \fn char * Trim( char * s )
\brief Oøeže øetìzec o bílé znaky z obou stran.
\param s ukazatel na øetìzec
\return Ukazatel na novou pozici v tomtéž øetìzci
*/
char * Trim( char *s );

/*! \fn void Swap( T & a, T & b )
\brief Prohození hodnot.
\param a první hodnota.
\param b druhá hodnota.
*/
template<typename T> inline void swap( T & a, T & b )
{
	const T tmp = a;
	a = b;
	b = tmp;
}

#endif
