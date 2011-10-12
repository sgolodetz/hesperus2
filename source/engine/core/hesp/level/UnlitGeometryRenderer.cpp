/***
 * hesperus: UnlitGeometryRenderer.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "UnlitGeometryRenderer.h"

#include <hesp/ogl/WrappedGL.h>

#include <hesp/colours/Colour3d.h>
#include <hesp/textures/Texture.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
UnlitGeometryRenderer::UnlitGeometryRenderer(const std::vector<TexturedPolygon_Ptr>& polygons)
:	m_polygons(polygons)
{
	// Determine the set of unique texture names.
	std::set<std::string> textureNames;
	int polyCount = static_cast<int>(polygons.size());
	for(int i=0; i<polyCount; ++i)
	{
		textureNames.insert(polygons[i]->auxiliary_data());
	}

	load_textures(textureNames);
}

//#################### PUBLIC METHODS ####################
void UnlitGeometryRenderer::render(const std::vector<int>& polyIndices) const
{
	// FIXME: This should be replaced with render_proper() once the proper version is ready.
	render_simple(polyIndices);
}

//#################### PRIVATE METHODS ####################
void UnlitGeometryRenderer::render_simple(const std::vector<int>& polyIndices) const
{
	glPushAttrib(GL_ENABLE_BIT | GL_POLYGON_BIT);

	glEnable(GL_TEXTURE_2D);
	glColor3d(1,1,1);

	int indexCount = static_cast<int>(polyIndices.size());
	for(int i=0; i<indexCount; ++i)
	{
		TexturedPolygon_Ptr poly = m_polygons[polyIndices[i]];

		// Note:	If we got to this point, all textures were loaded successfully,
		//			so the texture's definitely in the map.
		std::map<std::string,Texture_Ptr>::const_iterator jt = m_textures.find(poly->auxiliary_data());
		jt->second->bind();

		int vertCount = poly->vertex_count();
		glBegin(GL_POLYGON);
			for(int j=0; j<vertCount; ++j)
			{
				const TexturedVector3d& v = poly->vertex(j);
				glTexCoord2d(v.u, v.v);
				glVertex3d(v.x, v.y, v.z);
			}
		glEnd();
	}

	glPopAttrib();
}

}
