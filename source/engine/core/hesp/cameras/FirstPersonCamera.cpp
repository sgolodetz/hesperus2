/***
 * hesperus: FirstPersonCamera.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "FirstPersonCamera.h"

#include <hesp/axes/NUVAxes.h>
#include <hesp/bounds/Bounds.h>
#include <hesp/bounds/BoundsManager.h>
#include <hesp/objects/components/ICmpOrientation.h>
#include <hesp/objects/components/ICmpSimulation.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
FirstPersonCamera::FirstPersonCamera(const ObjectID& viewer, const ObjectManager_Ptr& objectManager)
:	m_viewer(viewer), m_objectManager(objectManager)
{}

//#################### PUBLIC METHODS ####################
Vector3d FirstPersonCamera::at() const
{
	return m_at;
}

Vector3d FirstPersonCamera::eye() const
{
	return m_eye;
}

bool FirstPersonCamera::is_inside_player() const
{
	return true;
}

Vector3d FirstPersonCamera::look() const
{
	return m_look;
}

Vector3d FirstPersonCamera::up() const
{
	return Vector3d(0,0,1);
}

void FirstPersonCamera::update()
{
	ICmpOrientation_Ptr cmpOrientation = m_objectManager->get_component(m_viewer, cmpOrientation);
	ICmpSimulation_Ptr cmpSimulation = m_objectManager->get_component(m_viewer, cmpSimulation);

	const Vector3d& pos = cmpSimulation->position();
	m_look = cmpOrientation->nuv_axes()->n();

	// Calculate the viewer's eye position and where they're looking at.
	const Bounds_CPtr& bounds = m_objectManager->bounds_manager()->bounds(cmpSimulation->bounds_group(), cmpSimulation->posture());
	m_eye = pos + Vector3d(0,0,bounds->height()/2 * 0.9);
	m_at = m_eye + m_look;
}

}
