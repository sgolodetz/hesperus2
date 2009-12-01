/***
 * hesperus: OnionCompiler.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <hesp/math/geom/GeomUtil.h>
#include "OnionBranch.h"
#include "TreeUtil.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
template <typename Poly>
OnionCompiler<Poly>::OnionCompiler(const std::vector<PolyVector>& maps,
								   const std::vector<BSPTree_CPtr>& mapTrees,
								   double weight)
:	m_mapCount(static_cast<int>(maps.size())), m_mapTrees(mapTrees), m_weight(weight), m_polygons(new PolyVector)
{
	for(int i=0; i<m_mapCount; ++i)
	{
		for(typename PolyVector::const_iterator jt=maps[i].begin(), jend=maps[i].end(); jt!=jend; ++jt)
		{
			const Poly_Ptr& poly = *jt;
			int polyIndex = static_cast<int>(m_polygons->size());
			m_polygons->push_back(poly);
			m_polyIndices.push_back(PolyIndex(polyIndex, true));
		}
	}
}

//#################### PUBLIC METHODS ####################
template <typename Poly>
void OnionCompiler<Poly>::build_tree()
{
	std::vector<OnionNode_Ptr> nodes;
	std::vector<Plane_Ptr> ancestorPlanes;
	build_subtree(m_polyIndices, nodes, ancestorPlanes);
	m_tree.reset(new OnionTree(nodes, m_mapCount));
}

template <typename Poly>
const typename OnionCompiler<Poly>::PolyVector_Ptr& OnionCompiler<Poly>::polygons() const
{
	return m_polygons;
}

template <typename Poly>
const OnionTree_Ptr& OnionCompiler<Poly>::tree() const
{
	return m_tree;
}

//#################### PRIVATE METHODS ####################
template <typename Poly>
OnionNode_Ptr OnionCompiler<Poly>::build_subtree(const std::vector<PolyIndex>& polyIndices,
												 std::vector<OnionNode_Ptr>& nodes,
												 std::vector<Plane_Ptr>& ancestorPlanes)
{
	typedef typename Poly::Vert Vert;
	typedef typename Poly::AuxData AuxData;

	PolyIndex_CPtr splitPoly = choose_split_poly(polyIndices);

	// If there were no suitable split candidates, we must have ended up in a leaf.
	if(!splitPoly)
	{
		boost::dynamic_bitset<> solidityDescriptor = determine_leaf_solidity(ancestorPlanes);
		std::vector<int> indicesOnly;
		for(size_t i=0, size=polyIndices.size(); i<size; ++i) indicesOnly.push_back(polyIndices[i].index);
		nodes.push_back(OnionNode_Ptr(new OnionLeaf((int)nodes.size(), solidityDescriptor, indicesOnly)));
		return nodes.back();
	}

	Plane_Ptr splitter(new Plane(make_plane(*(*m_polygons)[splitPoly->index])));

	std::vector<PolyIndex> backPolys, frontPolys;

	for(typename std::vector<PolyIndex>::const_iterator it=polyIndices.begin(), iend=polyIndices.end(); it!=iend; ++it)
	{
		int curIndex = it->index;
		const Poly& curPoly = *(*m_polygons)[curIndex];
		switch(classify_polygon_against_plane(curPoly, *splitter))
		{
			case CP_BACK:
			{
				backPolys.push_back(*it);
				break;
			}
			case CP_COPLANAR:
			{
				if(splitter->normal().dot(curPoly.normal()) > 0) frontPolys.push_back(PolyIndex(curIndex,false));
				else backPolys.push_back(PolyIndex(curIndex,false));
				break;
			}
			case CP_FRONT:
			{
				frontPolys.push_back(*it);
				break;
			}
			case CP_STRADDLE:
			{
				SplitResults<Vert,AuxData> sr = split_polygon(curPoly, *splitter);

				// Copy the back half over the polygon being split.
				(*m_polygons)[curIndex] = sr.back;

				// Append the front half to the end of the polygon array.
				int k = static_cast<int>(m_polygons->size());
				m_polygons->push_back(sr.front);

				backPolys.push_back(PolyIndex(curIndex,it->splitCandidate));
				frontPolys.push_back(PolyIndex(k,it->splitCandidate));
				break;
			}
		}
	}

	ancestorPlanes.push_back(splitter);
	OnionNode_Ptr left = build_subtree(frontPolys, nodes, ancestorPlanes);
	ancestorPlanes.pop_back();

	ancestorPlanes.push_back(Plane_Ptr(new Plane(splitter->flip())));
	OnionNode_Ptr right = build_subtree(backPolys, nodes, ancestorPlanes);
	ancestorPlanes.pop_back();

	OnionNode_Ptr subtreeRoot(new OnionBranch((int)nodes.size(), splitter, left, right));
	nodes.push_back(subtreeRoot);
	return subtreeRoot;
}

template <typename Poly>
typename OnionCompiler<Poly>::PolyIndex_CPtr
OnionCompiler<Poly>::choose_split_poly(const std::vector<PolyIndex>& polyIndices) const
{
	PolyIndex_CPtr bestPolyIndex;
	double bestMetric = INT_MAX;

	int indexCount = static_cast<int>(polyIndices.size());
	for(int i=0; i<indexCount; ++i)
	{
		if(!polyIndices[i].splitCandidate) continue;

		Plane plane = make_plane(*(*m_polygons)[polyIndices[i].index]);

		int balance = 0, splits = 0;
		for(int j=0; j<indexCount; ++j)
		{
			if(j == i) continue;

			switch(classify_polygon_against_plane(*(*m_polygons)[polyIndices[j].index], plane))
			{
				case CP_BACK:
					--balance;
					break;
				case CP_COPLANAR:
					break;
				case CP_FRONT:
					++balance;
					break;
				case CP_STRADDLE:
					++splits;
					break;
			}
		}

		double metric = abs(balance) + m_weight * splits;
		if(metric < bestMetric)
		{
			bestPolyIndex.reset(new PolyIndex(polyIndices[i]));
			bestMetric = metric;
		}
	}

	return bestPolyIndex;
}

template <typename Poly>
boost::dynamic_bitset<> OnionCompiler<Poly>::determine_leaf_solidity(const std::vector<Plane_Ptr>& ancestorPlanes) const
{
	// Step 1:	Find an arbitrary point within the leaf with the specified ancestor planes.
	Vector3d p = find_arbitrary_leaf_point(ancestorPlanes);

	// Step 2:	Classify the point against each map tree to determine the solidity descriptor for the leaf.
	boost::dynamic_bitset<> solidityDescriptor(m_mapCount);
	for(int i=0; i<m_mapCount; ++i)
	{
		int leafIndex = TreeUtil::find_leaf_index(p, m_mapTrees[i]);
		const BSPLeaf *leaf = m_mapTrees[i]->leaf(leafIndex);
		solidityDescriptor[i] = leaf->is_solid();
	}

	return solidityDescriptor;
}

template <typename Poly>
Vector3d OnionCompiler<Poly>::find_arbitrary_leaf_point(const std::vector<Plane_Ptr>& ancestorPlanes)
{
	// Step 1:	Create an inward-facing convex polyhedron around the leaf.

	// FIXME:	This is similar to the process for creating expanded brush faces from the expanded planes.
	//			It might be possible to factor out the commonality in the future.

	// Make an array of possible bounding planes: these are the ancestor planes themselves, plus the
	// planes which bound the 3D world. These latter planes are necessary to ensure we end up with
	// a bounded convex polyhedron to represent the leaf.

	const double HALFWORLDBOUND = 100000;	// anything large that will ensure the level is surrounded will do here

	std::vector<Plane_Ptr> planes = ancestorPlanes;
	planes.push_back(Plane_Ptr(new Plane(Vector3d(1,0,0), -HALFWORLDBOUND)));	// x = -HALFWORLDBOUND (dir +x)
	planes.push_back(Plane_Ptr(new Plane(Vector3d(-1,0,0), -HALFWORLDBOUND)));	// x = HALFWORLDBOUND (dir -x)
	planes.push_back(Plane_Ptr(new Plane(Vector3d(0,1,0), -HALFWORLDBOUND)));	// y = -HALFWORLDBOUND (dir +y)
	planes.push_back(Plane_Ptr(new Plane(Vector3d(0,-1,0), -HALFWORLDBOUND)));	// y = HALFWORLDBOUND (dir -y)
	planes.push_back(Plane_Ptr(new Plane(Vector3d(0,0,1), -HALFWORLDBOUND)));	// z = -HALFWORLDBOUND (dir +z)
	planes.push_back(Plane_Ptr(new Plane(Vector3d(0,0,-1), -HALFWORLDBOUND)));	// z = HALFWORLDBOUND (dir -z)

	typedef Polygon<Vector3d,NullAuxData> SimplePoly;
	typedef shared_ptr<SimplePoly> SimplePoly_Ptr;

	std::vector<SimplePoly_Ptr> faces;

	int planeCount = static_cast<int>(planes.size());
	for(int i=0; i<planeCount; ++i)
	{
		// Build a large initial face on each plane.
		SimplePoly_Ptr face = make_universe_polygon<NullAuxData>(*planes[i], NullAuxData());

		// Clip it to the other planes.
		bool discard = false;
		for(int j=0; j<planeCount; ++j)
		{
			if(j == i) continue;

			switch(classify_polygon_against_plane(*face, *planes[j]))
			{
				case CP_BACK:
				{
					// The face is entirely behind this plane, so it's outside the leaf.
					discard = true;
					break;
				}
				case CP_COPLANAR:
				{
					// The ancestor planes are unique, so this should never happen.
					throw Exception("OnionCompiler: Unexpected duplicate plane");
				}
				case CP_FRONT:
				{
					// The face is entirely in front of this plane, so it's inside the leaf.
					continue;
				}
				case CP_STRADDLE:
				{
					// The face straddles the plane, so split it and keep the bit in front of it.
					SplitResults<Vector3d,NullAuxData> sr = split_polygon(*face, *planes[j]);
					face = sr.front;
					break;
				}
			}

			if(discard) break;
		}

		// If it hasn't been clipped out of existence, add it to the face array.
		if(!discard) faces.push_back(face);
	}

	// Step 2:	Compute the average of the polyhedron face midpoints, which will be a point in the leaf.
	int denom = 0;
	Vector3d p(0,0,0);
	int faceCount = static_cast<int>(faces.size());
	for(int i=0; i<faceCount; ++i)
	{
		const SimplePoly& face = *faces[i];
		int vertCount = face.vertex_count();
		for(int j=0; j<vertCount; ++j)
		{
			p += face.vertex(j);
			++denom;
		}
	}
	p /= denom;

	return p;
}

}
