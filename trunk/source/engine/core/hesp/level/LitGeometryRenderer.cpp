/***
 * hesperus: LitGeometryRenderer.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <GL/glew.h>

#include "LitGeometryRenderer.h"

#include <hesp/textures/Texture.h>
#include <hesp/textures/TextureFactory.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
LitGeometryRenderer::LitGeometryRenderer(const TexLitPolyVector& polygons, const std::vector<Image24_Ptr>& lightmaps)
:	m_polygons(polygons)
{
	assert(polygons.size() == lightmaps.size());

	// Determine the set of unique texture names.
	std::set<std::string> textureNames;
	int polyCount = static_cast<int>(polygons.size());
	for(int i=0; i<polyCount; ++i)
	{
		textureNames.insert(polygons[i]->auxiliary_data());
	}

	load_textures(textureNames);

	// Create the lightmaps.
	int lightmapCount = static_cast<int>(lightmaps.size());
	m_lightmaps.resize(lightmapCount);
	for(int i=0; i<lightmapCount; ++i)
	{
		m_lightmaps[i] = TextureFactory::create_texture24(lightmaps[i], true);
	}
}

//#################### PUBLIC METHODS ####################
void LitGeometryRenderer::render(const std::vector<int>& polyIndices) const
{
	// FIXME: This should be replaced with render_proper() once the proper version is ready.
	render_simple(polyIndices);
}

//#################### PRIVATE METHODS ####################
void LitGeometryRenderer::render_simple(const std::vector<int>& polyIndices) const
{
	glPushAttrib(GL_ENABLE_BIT | GL_POLYGON_BIT | GL_TEXTURE_BIT);

	// Set up the two texture units.
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glColor3d(1,1,1);

	int indexCount = static_cast<int>(polyIndices.size());
	for(int i=0; i<indexCount; ++i)
	{
		TexturedLitPolygon_Ptr poly = m_polygons[polyIndices[i]];

		// Note:	If we got to this point, all textures were loaded successfully,
		//			so the texture's definitely in the map.
		std::map<std::string,Texture_Ptr>::const_iterator jt = m_textures.find(poly->auxiliary_data());
		glActiveTextureARB(GL_TEXTURE0_ARB);
		jt->second->bind();

		glActiveTextureARB(GL_TEXTURE1_ARB);
		m_lightmaps[polyIndices[i]]->bind();

		int vertCount = poly->vertex_count();
		glBegin(GL_POLYGON);
			for(int j=0; j<vertCount; ++j)
			{
				const TexturedLitVector3d& v = poly->vertex(j);
				glMultiTexCoord2dARB(GL_TEXTURE0_ARB, v.u, v.v);
				glMultiTexCoord2dARB(GL_TEXTURE1_ARB, v.lu, v.lv);
				glVertex3d(v.x, v.y, v.z);
			}
		glEnd();
	}

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_2D);
	glActiveTextureARB(GL_TEXTURE0_ARB);

	glPopAttrib();
}

}
