/***
 * hesperus: Sprite.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_SPRITE
#define H_HESP_SPRITE

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <hesp/math/vectors/Vector3.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class Texture> Texture_Ptr;

class Sprite
{
	//#################### PRIVATE VARIABLES ####################
private:
	Texture_Ptr m_texture;

	//#################### CONSTRUCTORS ####################
public:
	explicit Sprite(const Texture_Ptr& texture);

	//#################### PUBLIC METHODS ####################
public:
	void render(const Vector3d& spritePos, const Vector3d& cameraPos, double width, double height) const;
};

}

#endif
