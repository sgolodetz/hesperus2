/***
 * hesperus: AABB.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "AABB.h"

#include <boost/tokenizer.hpp>

#include <hesp/exceptions/Exception.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
/**
Constructs an AABB with the specified minimum and maximum bounds.

@param minimum		The minimum bound
@param maximum		The maximum bound
@throws Exception	If the minimum bound is not strictly less than the maximum bound
*/
template <typename Vec>
AABB<Vec>::AABB(const Vec& minimum, const Vec& maximum)
:	m_minimum(minimum), m_maximum(maximum)
{
	check_invariant();
}

//#################### PUBLIC METHODS ####################
/**
Returns the maximum bound for the AABB.

@return	As stated
*/
template <typename Vec>
const Vec& AABB<Vec>::maximum() const
{
	return m_maximum;
}

/**
Returns the minimum bound for the AABB.

@return	As stated
*/
template <typename Vec>
const Vec& AABB<Vec>::minimum() const
{
	return m_minimum;
}

/**
Checks whether the two AABBs are within range of each other
(i.e. closer than the specified tolerance in at least one axis).

@param lhs			One of the AABBs
@param rhs			The other one
@param tolerance	The tolerance value
@return				true, if the AABBs are within range of each other, or false otherwise
*/
template <>
bool AABB2d::within_range(const AABB2d& lhs, const AABB2d& rhs, double tolerance)
{
	const Vector2d& minL = lhs.minimum();
	const Vector2d& maxL = lhs.maximum();
	const Vector2d& minR = rhs.minimum();
	const Vector2d& maxR = rhs.maximum();

	if(minL.x > maxR.x + tolerance) return false;
	if(minL.y > maxR.y + tolerance) return false;

	if(minR.x > maxL.x + tolerance) return false;
	if(minR.y > maxL.y + tolerance) return false;

	return true;
}

/**
Checks whether the two AABBs are within range of each other
(i.e. closer than the specified tolerance in at least one axis).

@param lhs			One of the AABBs
@param rhs			The other one
@param tolerance	The tolerance value
@return				true, if the AABBs are within range of each other, or false otherwise
*/
template <>
bool AABB3d::within_range(const AABB3d& lhs, const AABB3d& rhs, double tolerance)
{
	const Vector3d& minL = lhs.minimum();
	const Vector3d& maxL = lhs.maximum();
	const Vector3d& minR = rhs.minimum();
	const Vector3d& maxR = rhs.maximum();

	if(minL.x > maxR.x + tolerance) return false;
	if(minL.y > maxR.y + tolerance) return false;
	if(minL.z > maxR.z + tolerance) return false;

	if(minR.x > maxL.x + tolerance) return false;
	if(minR.y > maxL.y + tolerance) return false;
	if(minR.z > maxL.z + tolerance) return false;

	return true;
}

//#################### PRIVATE METHODS ####################
/**
Checks that the minimum bound is strictly less than the maximum bound.

@throws Exception	If m_minimum.r >= m_maximum.r for r in {x,y}
*/
template <>
void AABB2d::check_invariant() const
{
	if(	m_minimum.x >= m_maximum.x ||
		m_minimum.y >= m_maximum.y	)
	{
		throw Exception("The minimum bound is not strictly less than the maximum bound");
	}
}

/**
Checks that the minimum bound is strictly less than the maximum bound.

@throws Exception	If m_minimum.r >= m_maximum.r for r in {x,y,z}
*/
template <>
void AABB3d::check_invariant() const
{
	if(	m_minimum.x >= m_maximum.x ||
		m_minimum.y >= m_maximum.y ||
		m_minimum.z >= m_maximum.z	)
	{
		throw Exception("The minimum bound is not strictly less than the maximum bound");
	}
}

//#################### GLOBAL METHODS ####################
template <>
AABB3d read_aabb(const std::string& s, double scale)
{
	typedef boost::char_separator<char> sep;
	typedef boost::tokenizer<sep> tokenizer;

	tokenizer tok(s.begin(), s.end(), sep(" "));
	std::vector<std::string> tokens(tok.begin(), tok.end());
	if(tokens.size() != 10) throw Exception("PolyhedralBrush: Invalid bounds specification");

	std::vector<std::string> minComponents(&tokens[1], &tokens[4]);
	std::vector<std::string> maxComponents(&tokens[6], &tokens[9]);
	Vector3d minimum(minComponents), maximum(maxComponents);
	minimum *= scale;
	maximum *= scale;

	return AABB3d(minimum, maximum);
}

//#################### EXPLICIT INSTANTIATIONS ####################
template class AABB<Vector2d>;
template class AABB<Vector3d>;

}
