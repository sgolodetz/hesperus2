/***
 * hesperus: OnionPortal.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ONIONPORTAL
#define H_HESP_ONIONPORTAL

#include <iosfwd>
#include <set>

#include <hesp/math/geom/Polygon.h>

namespace hesp {

//#################### CLASSES ####################
struct OnionPortalInfo
{
	//#################### PUBLIC VARIABLES ####################
	int fromLeaf;
	int toLeaf;
	std::set<int> fromMapIndices;	// the maps in which this portal's from leaf appears (only used during portal generation)
	std::set<int> toMapIndices;		// the maps in which this portal's to leaf appears (only used during portal generation)
	std::set<int> mapIndices;		// the maps in which this portal appears

	//#################### CONSTRUCTORS ####################
	OnionPortalInfo() : fromLeaf(-1), toLeaf(-1) {}

	//#################### PUBLIC METHODS ####################
	OnionPortalInfo flip() const
	{
		OnionPortalInfo ret;
		ret.fromLeaf = toLeaf;
		ret.toLeaf = fromLeaf;
		ret.fromMapIndices = toMapIndices;
		ret.toMapIndices = fromMapIndices;
		ret.mapIndices = mapIndices;
		return ret;
	}
};

//#################### GLOBAL OPERATORS ####################
std::ostream& operator<<(std::ostream& os, const OnionPortalInfo& rhs);
std::istream& operator>>(std::istream& is, OnionPortalInfo& rhs);

//#################### TYPEDEFS ####################
typedef Polygon<Vector3d,OnionPortalInfo> OnionPortal;
typedef shared_ptr<OnionPortal> OnionPortal_Ptr;

}

#endif
