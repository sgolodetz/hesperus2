/***
 * hesperus: UnlitGeometryRenderer.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_UNLITGEOMETRYRENDERER
#define H_HESP_UNLITGEOMETRYRENDERER

#include <hesp/util/PolygonTypes.h>
#include "GeometryRenderer.h"

namespace hesp {

class UnlitGeometryRenderer : public GeometryRenderer
{
	//#################### TYPEDEFS ####################
private:
	typedef std::vector<TexturedPolygon_Ptr> TexPolyVector;

	//#################### PRIVATE VARIABLES ####################
private:
	TexPolyVector m_polygons;

	//#################### CONSTRUCTORS ####################
public:
	UnlitGeometryRenderer(const TexPolyVector& polygons);

	//#################### PUBLIC METHODS ####################
public:
	void render(const std::vector<int>& polyIndices) const;

	//#################### PRIVATE METHODS ####################
private:
	void render_simple(const std::vector<int>& polyIndices) const;
	void render_proper(const std::vector<int>& polyIndices) const;
};

}

#endif
