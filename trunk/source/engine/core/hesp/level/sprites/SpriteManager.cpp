/***
 * hesperus: SpriteManager.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "SpriteManager.h"

#include <boost/filesystem/operations.hpp>
namespace bf = boost::filesystem;

#include <hesp/images/ImageLoader.h>
#include <hesp/io/util/DirectoryFinder.h>
#include <hesp/textures/TextureFactory.h>
#include "Sprite.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
const Vector3d& SpriteManager::camera_position() const					{ return m_cameraPos; }
void SpriteManager::register_sprite(const std::string& spriteName)		{ register_resource(spriteName); }
void SpriteManager::set_camera_position(const Vector3d& cameraPos)		{ m_cameraPos = cameraPos; }
const Sprite_Ptr& SpriteManager::sprite(const std::string& spriteName)	{ return resource(spriteName); }
Sprite_CPtr SpriteManager::sprite(const std::string& spriteName) const	{ return resource(spriteName); }
std::set<std::string> SpriteManager::sprite_names() const				{ return resource_names(); }

//#################### PRIVATE METHODS ####################
Sprite_Ptr SpriteManager::load_resource(const std::string& spriteName) const
{
	bf::path spritesDir = DirectoryFinder::instance().determine_sprites_directory();
	std::string filename = (spritesDir / (spriteName + ".png")).file_string();
	return Sprite_Ptr(new Sprite(TextureFactory::create_texture32(ImageLoader::load_image32(filename))));
}

std::string SpriteManager::resource_type() const
{
	return "sprite";
}

}
