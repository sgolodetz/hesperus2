/***
 * hesperus: Antipenumbra.tpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

namespace hesp {

//#################### PUBLIC METHODS ####################
/**
Clips the specified polygon to the antipenumbra.

@param poly		The polygon
@return			The clipped version of the polygon
*/
template <typename Vert, typename AuxData>
shared_ptr<Polygon<Vert,AuxData> >
Antipenumbra::clip(const shared_ptr<Polygon<Vert,AuxData> >& poly)
{
	typedef Polygon<Vert,AuxData> Poly;
	typedef shared_ptr<Poly> Poly_Ptr;

	Poly_Ptr ret = poly;
	for(std::vector<Plane>::const_iterator it=m_planes.begin(), iend=m_planes.end(); it!=iend; ++it)
	{
		switch(classify_polygon_against_plane(*ret, *it))
		{
			case CP_BACK:
			{
				// The polygon is completely outside the antipenumbra.
				return Poly_Ptr();
			}
			case CP_COPLANAR:
			{
				// The polygon lies on the antipenumbra boundary and can't be seen properly from the source.
				return Poly_Ptr();
			}
			case CP_FRONT:
			{
				// Nothing to clip: move onto the next clip plane.
				break;
			}
			case CP_STRADDLE:
			{
				// Split the polygon across the clip plane and keep the bit inside the antipenumbra.
				ret = split_polygon(*ret, *it).front;
				break;
			}
		}
	}

	return ret;
}

}
