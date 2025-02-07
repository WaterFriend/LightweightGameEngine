#ifndef EAE6320_MATH_SVECTOR_INL
#define EAE6320_MATH_SVECTOR_INL

// Includes
//=========

#include <algorithm>
#include <cmath>
#include "sVector.h"

// Interface
//==========

// Addition
//---------

constexpr eae6320::Math::sVector eae6320::Math::sVector::operator +( const sVector& i_rhs ) const
{
	return sVector( x + i_rhs.x, y + i_rhs.y, z + i_rhs.z );
}

constexpr eae6320::Math::sVector& eae6320::Math::sVector::operator +=( const sVector& i_rhs )
{
	x += i_rhs.x;
	y += i_rhs.y;
	z += i_rhs.z;
	return *this;
}

constexpr eae6320::Math::sVector eae6320::Math::sVector::operator +( const float i_rhs ) const
{
	return sVector( x + i_rhs, y + i_rhs, z + i_rhs );
}

constexpr eae6320::Math::sVector& eae6320::Math::sVector::operator +=( const float i_rhs )
{
	x += i_rhs;
	y += i_rhs;
	z += i_rhs;
	return *this;
}

constexpr eae6320::Math::sVector eae6320::Math::operator +( const float i_lhs, const sVector& i_rhs )
{
	return i_rhs + i_lhs;
}

// Subtraction / Negation
//-----------------------

constexpr eae6320::Math::sVector eae6320::Math::sVector::operator -( const sVector& i_rhs ) const
{
	return sVector( x - i_rhs.x, y - i_rhs.y, z - i_rhs.z );
}

constexpr eae6320::Math::sVector& eae6320::Math::sVector::operator -=( const sVector& i_rhs )
{
	x -= i_rhs.x;
	y -= i_rhs.y;
	z -= i_rhs.z;
	return *this;
}

constexpr eae6320::Math::sVector eae6320::Math::sVector::operator -() const
{
	return sVector( -x, -y, -z );
}

constexpr eae6320::Math::sVector eae6320::Math::sVector::operator -( const float i_rhs ) const
{
	return sVector( x - i_rhs, y - i_rhs, z - i_rhs );
}

constexpr eae6320::Math::sVector& eae6320::Math::sVector::operator -=( const float i_rhs )
{
	x -= i_rhs;
	y -= i_rhs;
	z -= i_rhs;
	return *this;
}

constexpr eae6320::Math::sVector eae6320::Math::operator -( const float i_lhs, const sVector& i_rhs )
{
	return sVector( i_lhs - i_rhs.x, i_lhs - i_rhs.y, i_lhs - i_rhs.z );
}

// Products
//---------

constexpr eae6320::Math::sVector eae6320::Math::sVector::operator *( const float i_rhs ) const
{
	return sVector( x * i_rhs, y * i_rhs, z * i_rhs );
}

constexpr eae6320::Math::sVector& eae6320::Math::sVector::operator *=( const float i_rhs )
{
	x *= i_rhs;
	y *= i_rhs;
	z *= i_rhs;
	return *this;
}

constexpr eae6320::Math::sVector eae6320::Math::operator *( const float i_lhs, const sVector& i_rhs )
{
	return i_rhs * i_lhs;
}

constexpr float eae6320::Math::Dot( const sVector& i_lhs, const sVector& i_rhs )
{
	return ( i_lhs.x * i_rhs.x ) + ( i_lhs.y * i_rhs.y ) + ( i_lhs.z * i_rhs.z );
}

constexpr eae6320::Math::sVector eae6320::Math::Cross( const sVector& i_lhs, const sVector& i_rhs )
{
	return sVector(
		( i_lhs.y * i_rhs.z ) - ( i_lhs.z * i_rhs.y ),
		( i_lhs.z * i_rhs.x ) - ( i_lhs.x * i_rhs.z ),
		( i_lhs.x * i_rhs.y ) - ( i_lhs.y * i_rhs.x )
	);
}


constexpr eae6320::Math::sVector eae6320::Math::Max(const sVector& i_lhs, const sVector& i_rhs)
{
	const float max_x = std::max(i_lhs.x, i_rhs.x);
	const float max_y = std::max(i_lhs.y, i_rhs.y);
	const float max_z = std::max(i_lhs.z, i_rhs.z);
	return sVector(max_x, max_y, max_z);
}


constexpr eae6320::Math::sVector eae6320::Math::Min(const sVector& i_lhs, const sVector& i_rhs)
{
	const float min_x = std::min(i_lhs.x, i_rhs.x);
	const float min_y = std::min(i_lhs.y, i_rhs.y);
	const float min_z = std::min(i_lhs.z, i_rhs.z);
	return sVector(min_x, min_y, min_z);
}




// Comparison
//-----------

constexpr bool eae6320::Math::sVector::operator ==( const sVector& i_rhs ) const
{
	// Use & rather than && to prevent branches (all three comparisons will be evaluated)
	return ( x == i_rhs.x ) & ( y == i_rhs.y ) & ( z == i_rhs.z );
}

constexpr bool eae6320::Math::sVector::operator !=( const sVector& i_rhs ) const
{
	// Use | rather than || to prevent branches (all three comparisons will be evaluated)
	return ( x != i_rhs.x ) | ( y != i_rhs.y ) | ( z != i_rhs.z );
}

// Initialization / Clean Up
//--------------------------

constexpr eae6320::Math::sVector::sVector( const float i_x, const float i_y, const float i_z )
	:
	x( i_x ), y( i_y ), z( i_z )
{

}

#endif	// EAE6320_MATH_SVECTOR_INL
