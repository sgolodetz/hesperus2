/***
 * hesperus: LitGeometryRenderer.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LITGEOMETRYRENDERER
#define H_HESP_LITGEOMETRYRENDERER

#include <hesp/images/Image.h>
#include <hesp/util/PolygonTypes.h>
#include "GeometryRenderer.h"

namespace hesp {

class LitGeometryRenderer : public GeometryRenderer
{
	//#################### TYPEDEFS ####################
private:
	typedef std::vector<TexturedLitPolygon_Ptr> TexLitPolyVector;

	//#################### PRIVATE VARIABLES ####################
private:
	TexLitPolyVector m_polygons;
	std::vector<Texture_Ptr> m_lightmaps;

	//#################### CONSTRUCTORS ####################
public:
	LitGeometryRenderer(const TexLitPolyVector& polygons, const std::vector<Image24_Ptr>& lightmaps);

	//#################### PUBLIC METHODS ####################
public:
	void render(const std::vector<int>& polyIndices) const;

	//#################### PRIVATE METHODS ####################
private:
	void render_simple(const std::vector<int>& polyIndices) const;
};

}

#endif
