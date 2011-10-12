/***
 * hesperus: Camera.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CAMERA
#define H_HESP_CAMERA

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <hesp/math/vectors/Vector3.h>

namespace hesp {

class Camera
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~Camera() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual Vector3d at() const = 0;
	virtual Vector3d eye() const = 0;
	virtual bool is_inside_player() const = 0;
	virtual Vector3d look() const = 0;
	virtual Vector3d up() const = 0;
	virtual void update() = 0;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Camera> Camera_Ptr;
typedef shared_ptr<const Camera> Camera_CPtr;

}

#endif
