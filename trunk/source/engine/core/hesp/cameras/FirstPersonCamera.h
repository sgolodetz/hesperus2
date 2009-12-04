/***
 * hesperus: FirstPersonCamera.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_FIRSTPERSONCAMERA
#define H_HESP_FIRSTPERSONCAMERA

#include <hesp/level/objects/base/ObjectID.h>
#include "Camera.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class ObjectManager> ObjectManager_Ptr;

class FirstPersonCamera : public Camera
{
	//#################### PRIVATE VARIABLES ####################
private:
	ObjectID m_viewer;
	ObjectManager_Ptr m_objectManager;

	Vector3d m_at, m_eye, m_look;

	//#################### CONSTRUCTORS ####################
public:
	FirstPersonCamera(const ObjectID& viewer, const ObjectManager_Ptr& objectManager);

	//#################### PUBLIC METHODS ####################
public:
	Vector3d at() const;
	Vector3d eye() const;
	bool is_inside_player() const;
	Vector3d look() const;
	Vector3d up() const;
	void update();
};

}

#endif
