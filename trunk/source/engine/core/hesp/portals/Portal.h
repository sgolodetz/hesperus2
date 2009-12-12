/***
 * hesperus: Portal.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_PORTAL
#define H_HESP_PORTAL

#include <iosfwd>

#include <hesp/math/geom/Polygon.h>

namespace hesp {

//#################### CLASSES ####################
struct PortalInfo
{
	//#################### PUBLIC VARIABLES ####################
	int index;		// the index of the portal in a portal array (-1 if not being used)
	int fromLeaf;
	int toLeaf;

	//#################### CONSTRUCTORS ####################
	PortalInfo() : index(-1), fromLeaf(-1), toLeaf(-1) {}

	PortalInfo(int fromLeaf_, int toLeaf_) : index(-1), fromLeaf(fromLeaf_), toLeaf(toLeaf_) {}

	//#################### PUBLIC OPERATORS ####################
	bool operator==(const PortalInfo& rhs) const
	{
		return fromLeaf == rhs.fromLeaf && toLeaf == rhs.toLeaf;
	}

	//#################### PUBLIC METHODS ####################
	PortalInfo flip() const
	{
		return PortalInfo(toLeaf, fromLeaf);
	}
};

//#################### GLOBAL OPERATORS ####################
std::ostream& operator<<(std::ostream& os, const PortalInfo& rhs);
std::istream& operator>>(std::istream& is, PortalInfo& rhs);

//#################### TYPEDEFS ####################
typedef Polygon<Vector3d,PortalInfo> Portal;
typedef shared_ptr<Portal> Portal_Ptr;
typedef shared_ptr<const Portal> Portal_CPtr;

}

#endif
