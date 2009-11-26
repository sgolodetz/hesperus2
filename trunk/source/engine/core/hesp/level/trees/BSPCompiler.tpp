/***
 * hesperus: BSPCompiler.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

namespace hesp {

//#################### CONSTRUCTORS ####################
template <typename Poly>
BSPCompiler<Poly>::BSPCompiler(const PolyVector& polygons, const PolyVector& hintPolygons, double weight)
:	m_polygons(polygons), m_weight(weight)
{
	std::copy(hintPolygons.begin(), hintPolygons.end(), std::back_inserter(m_polygons));

	int normalPolyCount = static_cast<int>(polygons.size());
	int totalPolyCount = static_cast<int>(m_polygons.size());
	m_polyIndices.reserve(totalPolyCount);
	for(int i=0; i<totalPolyCount; ++i)
	{
		m_polyIndices.push_back(PolyIndex(i, true, i >= normalPolyCount));
	}
}

//#################### PUBLIC METHODS ####################
template <typename Poly>
void BSPCompiler<Poly>::build_tree()
{
	std::vector<BSPNode_Ptr> nodes;
	build_subtree(m_polyIndices, nodes, SD_UNKNOWN);
	m_tree.reset(new BSPTree(nodes));
}

template <typename Poly>
const typename BSPCompiler<Poly>::PolyVector& BSPCompiler<Poly>::polygons() const
{
	return m_polygons;
}

template <typename Poly>
BSPTree_Ptr BSPCompiler<Poly>::tree() const
{
	return m_tree;
}

//#################### PRIVATE METHODS ####################
template <typename Poly>
BSPNode_Ptr BSPCompiler<Poly>::build_subtree(const std::vector<PolyIndex>& polyIndices, std::vector<BSPNode_Ptr>& nodes, SolidityDescriptor solidityDescriptor)
{
	typedef typename Poly::Vert Vert;
	typedef typename Poly::AuxData AuxData;

	PolyIndex_CPtr splitPoly = choose_split_poly(polyIndices);

	// Don't allow hint polygons to split solid leaves.
	if(solidityDescriptor == SD_SOLID && splitPoly && splitPoly->hint) splitPoly.reset();

	// If there were no suitable split candidates, we must have ended up in a leaf.
	if(!splitPoly)
	{
		switch(solidityDescriptor)
		{
			case SD_UNKNOWN:
			{
				// This should never happen.
				throw Exception("Unknown solidity descriptor for leaf");
			}
			case SD_EMPTY:
			{
				std::vector<int> indicesOnly;
				for(size_t i=0, size=polyIndices.size(); i<size; ++i)
				{
					if(!polyIndices[i].hint) indicesOnly.push_back(polyIndices[i].index);
				}
				nodes.push_back(BSPLeaf::make_empty_leaf((int)nodes.size(), indicesOnly));
				break;
			}
			case SD_SOLID:
			{
				nodes.push_back(BSPLeaf::make_solid_leaf((int)nodes.size()));
				break;
			}
		}
		return nodes.back();
	}

	Plane_Ptr splitter(new Plane(make_plane(*m_polygons[splitPoly->index])));

	std::vector<PolyIndex> backPolys, frontPolys;

	for(std::vector<PolyIndex>::const_iterator it=polyIndices.begin(), iend=polyIndices.end(); it!=iend; ++it)
	{
		int curIndex = it->index;
		const Polygon<Vert,AuxData>& curPoly = *m_polygons[curIndex];
		switch(classify_polygon_against_plane(curPoly, *splitter))
		{
			case CP_BACK:
			{
				backPolys.push_back(*it);
				break;
			}
			case CP_COPLANAR:
			{
				if(splitter->normal().dot(curPoly.normal()) > 0) frontPolys.push_back(PolyIndex(curIndex,false,it->hint));
				else backPolys.push_back(PolyIndex(curIndex,false,it->hint));
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
				m_polygons[curIndex] = sr.back;
				int k = static_cast<int>(m_polygons.size());
				m_polygons.push_back(sr.front);
				backPolys.push_back(PolyIndex(curIndex,it->splitCandidate,it->hint));
				frontPolys.push_back(PolyIndex(k,it->splitCandidate,it->hint));
				break;
			}
		}
	}

	BSPNode_Ptr left, right;
	if(splitPoly->hint)
	{
		left = build_subtree(frontPolys, nodes, solidityDescriptor);
		right = build_subtree(backPolys, nodes, solidityDescriptor);
	}
	else
	{
		left = build_subtree(frontPolys, nodes, SD_EMPTY);
		right = build_subtree(backPolys, nodes, SD_SOLID);
	}

	BSPNode_Ptr subtreeRoot(new BSPBranch((int)nodes.size(), splitter, left, right));
	nodes.push_back(subtreeRoot);
	return subtreeRoot;
}

template <typename Poly>
typename BSPCompiler<Poly>::PolyIndex_CPtr BSPCompiler<Poly>::choose_split_poly(const std::vector<PolyIndex>& polyIndices) const
{
	PolyIndex_CPtr bestPolyIndex;
	double bestMetric = INT_MAX;

	int indexCount = static_cast<int>(polyIndices.size());
	for(int i=0; i<indexCount; ++i)
	{
		if(!polyIndices[i].splitCandidate) continue;

		Plane plane = make_plane(*m_polygons[polyIndices[i].index]);
		int balance = 0, splits = 0;
		int hintPenalty = polyIndices[i].hint ? 100000 : 0;		// make sure hint planes are chosen last

		for(int j=0; j<indexCount; ++j)
		{
			if(j == i) continue;
			if(polyIndices[j].hint) continue;	// hint polygons shouldn't affect balance or splitting

			switch(classify_polygon_against_plane(*m_polygons[polyIndices[j].index], plane))
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

		double metric = abs(balance) + m_weight * splits + hintPenalty;
		if(metric < bestMetric)
		{
			bestPolyIndex.reset(new PolyIndex(polyIndices[i]));
			bestMetric = metric;
		}
	}

	return bestPolyIndex;
}

}
