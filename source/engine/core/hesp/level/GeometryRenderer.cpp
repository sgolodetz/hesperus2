/***
 * hesperus: GeometryRenderer.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "GeometryRenderer.h"

#include <hesp/images/PNGLoader.h>
#include <hesp/io/util/DirectoryFinder.h>
#include <hesp/textures/TextureFactory.h>
namespace bf = boost::filesystem;

namespace hesp {

//#################### PROTECTED METHODS ####################
void GeometryRenderer::load_textures(const std::set<std::string>& textureNames)
{
	bf::path texturesDir = DirectoryFinder::instance().determine_textures_directory();

	for(std::set<std::string>::const_iterator it=textureNames.begin(), iend=textureNames.end(); it!=iend; ++it)
	{
		std::string filename = (texturesDir / (*it + ".png")).file_string();
		Image24_Ptr image = PNGLoader::load_image24(filename);
		m_textures.insert(std::make_pair(*it, TextureFactory::create_texture24(image)));
	}
}

}
