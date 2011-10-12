/***
 * hesperus: PolygonTypes.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_POLYGONTYPES
#define H_HESP_POLYGONTYPES

#include <iosfwd>
#include <string>

#include <hesp/math/geom/Polygon.h>
#include <hesp/math/vectors/TexturedVector3d.h>
#include <hesp/math/vectors/TexturedLitVector3d.h>

namespace hesp {

//#################### CLASSES ####################
/*
TODO

We will eventually want to store the following sorts of things as well:

- type of surface (for sound effects)
*/
class ColPolyAuxData
{
	//#################### FRIENDS ####################
	friend std::ostream& operator<<(std::ostream& os, const ColPolyAuxData& rhs);
	friend std::istream& operator>>(std::istream& is, ColPolyAuxData& rhs);

	//#################### PRIVATE VARIABLES ####################
private:
	int m_mapIndex;
	bool m_walkable;

	//#################### CONSTRUCTORS ####################
public:
	ColPolyAuxData()
	:	m_mapIndex(-1), m_walkable(false)
	{}

	explicit ColPolyAuxData(bool walkable)
	:	m_mapIndex(-1), m_walkable(walkable)
	{}

	//#################### PUBLIC METHODS ####################
public:
	int map_index() const				{ return m_mapIndex; }
	void set_map_index(int mapIndex)	{ m_mapIndex = mapIndex; }
	void set_walkable(bool walkable)	{ m_walkable = walkable; }
	bool walkable() const				{ return m_walkable; }
};

//#################### GLOBAL OPERATORS ####################
std::ostream& operator<<(std::ostream& os, const ColPolyAuxData& rhs);
std::istream& operator>>(std::istream& is, ColPolyAuxData& rhs);

//#################### TYPEDEFS ####################
typedef Polygon<Vector3d, ColPolyAuxData> CollisionPolygon;
typedef shared_ptr<CollisionPolygon> CollisionPolygon_Ptr;
typedef shared_ptr<const CollisionPolygon> CollisionPolygon_CPtr;

typedef Polygon<TexturedVector3d, std::string> TexturedPolygon;
typedef shared_ptr<TexturedPolygon> TexturedPolygon_Ptr;
typedef shared_ptr<const TexturedPolygon> TexturedPolygon_CPtr;

typedef Polygon<TexturedLitVector3d, std::string> TexturedLitPolygon;
typedef shared_ptr<TexturedLitPolygon> TexturedLitPolygon_Ptr;
typedef shared_ptr<const TexturedLitPolygon> TexturedLitPolygon_CPtr;

}

#endif
