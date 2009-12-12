/***
 * hesperus: BrushExpander.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_BRUSHEXPANDER
#define H_HESP_BRUSHEXPANDER

#include <set>

#include <hesp/math/geom/UniquePlanePred.h>
#include <hesp/util/PolygonTypes.h>
#include "PolyhedralBrush.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
class Bounds;

class BrushExpander
{
	//#################### TYPEDEFS ####################
private:
	typedef shared_ptr<ColPolyAuxData> ColPolyAuxData_Ptr;

	//#################### NESTED CLASSES ####################
private:
	struct BrushPlane
	{
		//#################### PUBLIC VARIABLES ####################
		Plane plane;
		ColPolyAuxData_Ptr auxData;

		//#################### CONSTRUCTORS ####################
		explicit BrushPlane(const Plane& plane_, const ColPolyAuxData_Ptr auxData_ = ColPolyAuxData_Ptr())
		:	plane(plane_), auxData(auxData_)
		{}

		//#################### PUBLIC OPERATORS ####################
		bool operator<(const BrushPlane& rhs) const
		{
			const double angleTolerance = 2 * PI / 180;	// convert 2 degrees to radians
			const double distTolerance = 0.005;
			return UniquePlanePred(angleTolerance, distTolerance)(plane, rhs.plane);
		}
	};

	//#################### TYPEDEFS ####################
private:
	typedef PolyhedralBrush<CollisionPolygon> ColPolyBrush;
	typedef shared_ptr<ColPolyBrush> ColPolyBrush_Ptr;
	typedef shared_ptr<const ColPolyBrush> ColPolyBrush_CPtr;

	typedef std::set<BrushPlane> BrushPlaneSet;
	typedef shared_ptr<BrushPlaneSet> BrushPlaneSet_Ptr;
	typedef shared_ptr<const BrushPlaneSet> BrushPlaneSet_CPtr;

	//#################### PUBLIC METHODS ####################
public:
	static ColPolyBrush_Ptr expand_brush(const ColPolyBrush_CPtr& brush, const Bounds& bounds, int mapIndex);

	//#################### PRIVATE METHODS ####################
private:
	static PlaneClassifier classify_brush_against_plane(const ColPolyBrush_CPtr& brush, const Plane& plane);
	static BrushPlaneSet_Ptr determine_brush_planes(const ColPolyBrush_CPtr& brush);
	static BrushPlane expand_brush_plane(const BrushPlane& brushPlane, const Bounds& bounds);
	static BrushPlaneSet_Ptr expand_brush_planes(const BrushPlaneSet_CPtr& brushPlanes, const Bounds& bounds);
};

}

#endif
