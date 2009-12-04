/***
 * hesperus: FixedCamera.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_FIXEDCAMERA
#define H_HESP_FIXEDCAMERA

#include "Camera.h"

namespace hesp {

class FixedCamera : public Camera
{
	//#################### PRIVATE VARIABLES ####################
private:
	Vector3d m_eye, m_at, m_up;

	//#################### CONSTRUCTORS ####################
public:
	FixedCamera(const Vector3d& eye, const Vector3d& look, const Vector3d& up = Vector3d(0,0,1));

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
