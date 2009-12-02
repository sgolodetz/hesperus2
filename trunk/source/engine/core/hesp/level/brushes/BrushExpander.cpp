/***
 * hesperus: BrushExpander.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "BrushExpander.h"

#include <iostream>

#include <hesp/level/bounds/Bounds.h>

namespace hesp {

//#################### PUBLIC METHODS ####################
/**
Expand the brush in such a way that the expanded brush touches the bounds centre
whenever the original brush touches the bounds.

@param brush		The brush
@param bounds		The bounds
@param mapIndex		The index of the map the expanded brush will be in (i.e. the index of this bounds in the bounds array)
@return				The expanded brush
*/
BrushExpander::ColPolyBrush_Ptr BrushExpander::expand_brush(const ColPolyBrush_CPtr& brush, const Bounds& bounds, int mapIndex)
{
	// Determine which planes need expanding (these are the face planes + any bevel planes).
	BrushPlaneSet_Ptr brushPlanes = determine_brush_planes(brush);

	// Expand the brush planes against the bounds.
	brushPlanes = expand_brush_planes(brushPlanes, bounds);

	std::vector<CollisionPolygon_Ptr> expandedFaces;

	// Generate the new collision faces from the expanded brush planes.
	for(BrushPlaneSet::const_iterator it=brushPlanes->begin(), iend=brushPlanes->end(); it!=iend; ++it)
	{
		// Build a large initial face on each brush plane.
		CollisionPolygon_Ptr face;
		if(it->auxData)
		{
			face = make_universe_polygon<ColPolyAuxData>(it->plane, *(it->auxData));
		}
		else
		{
			// Construct an appropriate CPAuxData for any (axis-aligned) bevel plane we needed to add.

			// A bevel plane is only walkable if its normal is less than 45 degrees to the vertical.
			const double MAX_ANGLE_TO_VERTICAL = 45 * PI/180;	// i.e. 45 degrees
			double angleToVertical = acos(it->plane.normal().dot(Vector3d(0,0,1)));
			bool walkable = fabs(angleToVertical) <= MAX_ANGLE_TO_VERTICAL;

			face = make_universe_polygon<ColPolyAuxData>(it->plane, ColPolyAuxData(walkable));
		}

		// Set its map index.
		face->auxiliary_data().set_map_index(mapIndex);

		// Clip it to the other planes.
		bool discard = false;
		for(BrushPlaneSet::const_iterator jt=brushPlanes->begin(), jend=brushPlanes->end(); jt!=jend; ++jt)
		{
			if(jt == it) continue;

			switch(classify_polygon_against_plane(*face, jt->plane))
			{
				case CP_BACK:
				{
					// The face is entirely behind this plane, so no clipping is needed.
					continue;
				}
				case CP_COPLANAR:
				{
					// The planes are unique, so this should never happen.
					std::cout << "BrushExpander: Unexpected duplicate plane" << std::endl;
					continue;
				}
				case CP_FRONT:
				{
					// The face is entirely in front of this plane, so it's not part of the expanded brush.
					discard = true;
					break;
				}
				case CP_STRADDLE:
				{
					// The face straddles the plane, so split it and keep the bit behind it.
					SplitResults<Vector3d,ColPolyAuxData> sr = split_polygon(*face, jt->plane);
					face = sr.back;
					break;
				}
			}

			if(discard) break;
		}

		// If it hasn't been clipped out of existence, add it to the expanded brush.
		if(!discard) expandedFaces.push_back(face);
	}

	// Construct a bounding box for the expanded brush.
	AABB3d expandedBounds = construct_bounding_box(expandedFaces);

	return ColPolyBrush_Ptr(new ColPolyBrush(expandedBounds, expandedFaces, brush->function()));
}

//#################### PRIVATE METHODS ####################
/**
Classifies the brush against a plane.

@param brush	The brush
@param plane	The plane
@return			The brush classification against the plane
*/
PlaneClassifier BrushExpander::classify_brush_against_plane(const ColPolyBrush_CPtr& brush, const Plane& plane)
{
	bool backFlag = false, frontFlag = false;

	const std::vector<CollisionPolygon_Ptr>& faces = brush->faces();
	int faceCount = static_cast<int>(faces.size());
	for(int i=0; i<faceCount; ++i)
	{
		switch(classify_polygon_against_plane(*faces[i], plane))
		{
		case CP_BACK:
			backFlag = true;
			break;
		case CP_COPLANAR:
			break;
		case CP_FRONT:
			frontFlag = true;
			break;
		case CP_STRADDLE:
			return CP_STRADDLE;
		}

		if(backFlag && frontFlag) return CP_STRADDLE;
	}

	if(backFlag) return CP_BACK;
	else if(frontFlag) return CP_FRONT;
	else return CP_COPLANAR;
}

/**
Determines which planes are necessary for building the expanded brush (including bevel planes).

@param brush	The initial brush
@return			The necessary brush planes
*/
BrushExpander::BrushPlaneSet_Ptr BrushExpander::determine_brush_planes(const ColPolyBrush_CPtr& brush)
{
	BrushPlaneSet_Ptr brushPlanes(new BrushPlaneSet);

	// Add the planes of all the brush faces.
	const std::vector<CollisionPolygon_Ptr>& faces = brush->faces();
	int faceCount = static_cast<int>(faces.size());
	for(int i=0; i<faceCount; ++i)
	{
		ColPolyAuxData_Ptr auxData(new ColPolyAuxData(faces[i]->auxiliary_data()));
		brushPlanes->insert(BrushPlane(make_plane(*faces[i]), auxData));
	}

	// Try and add bevel planes (any unnecessary ones will not be added to the set due to the unique plane predicate).

	// Add "axial planes going through an edge or corner of the brush that are not already part of the brush".
	const AABB3d& bounds = brush->bounds();
	brushPlanes->insert(BrushPlane(Plane(Vector3d(-1,0,0), -bounds.minimum().x)));
	brushPlanes->insert(BrushPlane(Plane(Vector3d(0,-1,0), -bounds.minimum().y)));
	brushPlanes->insert(BrushPlane(Plane(Vector3d(0,0,-1), -bounds.minimum().z)));
	brushPlanes->insert(BrushPlane(Plane(Vector3d(1,0,0), bounds.maximum().x)));
	brushPlanes->insert(BrushPlane(Plane(Vector3d(0,1,0), bounds.maximum().y)));
	brushPlanes->insert(BrushPlane(Plane(Vector3d(0,0,1), bounds.maximum().z)));

	// Along edges, add "all planes that are parallel to one of the coordinate axes and do not change
	// the shape of the brush".
	for(int i=0; i<faceCount; ++i)
	{
		int vertCount = faces[i]->vertex_count();
		for(int j=0; j<vertCount; ++j)
		{
			int k = (j+1)%vertCount;

			const Vector3d& p1 = faces[i]->vertex(j);
			const Vector3d& p2 = faces[i]->vertex(k);

			Plane_Ptr possiblePlanes[] =
			{
				make_axial_plane(p1, p2, Vector3d(1,0,0)),
				make_axial_plane(p1, p2, Vector3d(0,1,0)),
				make_axial_plane(p1, p2, Vector3d(0,0,1))
			};

			for(int m=0; m<3; ++m)
			{
				if(possiblePlanes[m])
				{
					BrushPlane possiblePlane(*possiblePlanes[m]);
					BrushPlane flippedPossiblePlane(possiblePlanes[m]->flip());

					// If this plane (or its flipped equivalent) isn't currently in the brush plane set.
					if(	brushPlanes->find(possiblePlane) == brushPlanes->end() &&
						brushPlanes->find(flippedPossiblePlane) == brushPlanes->end())
					{
						// If the brush is entirely behind this plane, it's a valid
						// bevel plane, so add it. If the brush is entirely in front
						// of it, its inverse plane is a valid bevel plane, so add
						// that. Otherwise, carry on.
						switch(classify_brush_against_plane(brush, possiblePlane.plane))
						{
						case CP_BACK:
							brushPlanes->insert(possiblePlane);
							break;
						case CP_FRONT:
							brushPlanes->insert(flippedPossiblePlane);
							break;
						default:
							break;
						}
					}
				}
			}
		}
	}

	return brushPlanes;
}

/**
Moves a brush plane along its normal so that the "expanded" plane will touch the centre
of the bounds precisely when the original plane would touch the bounds itself.

@param brushPlane	The original plane
@param bounds		The bounds
@return				The expanded plane
*/
BrushExpander::BrushPlane BrushExpander::expand_brush_plane(const BrushPlane& brushPlane, const Bounds& bounds)
{
	const Vector3d& n = brushPlane.plane.normal();
	const double d = brushPlane.plane.distance_value();
	double expansionDistance = bounds.brush_expansion_distance(n);
	return BrushPlane(Plane(n, d+expansionDistance), brushPlane.auxData);
}

/**
Expands the set of brush planes against the bounds.

@param brushPlanes	The brush planes
@param bounds		The bounds
*/
BrushExpander::BrushPlaneSet_Ptr BrushExpander::expand_brush_planes(const BrushPlaneSet_CPtr& brushPlanes, const Bounds& bounds)
{
	BrushPlaneSet_Ptr expandedBrushPlanes(new BrushPlaneSet);
	for(BrushPlaneSet::const_iterator it=brushPlanes->begin(), iend=brushPlanes->end(); it!=iend; ++it)
	{
		expandedBrushPlanes->insert(expand_brush_plane(*it, bounds));
	}
	return expandedBrushPlanes;
}

}
