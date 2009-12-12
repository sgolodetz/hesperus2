/***
 * hesperus: CmpModelRender.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpModelRender.h"

#include <hesp/ogl/WrappedGL.h>

#include <hesp/bounds/Bounds.h>
#include <hesp/bounds/BoundsManager.h>
#include <hesp/models/AnimationController.h>
#include <hesp/models/Model.h>
#include <hesp/models/ModelManager.h>
#include "ICmpSimulation.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpModelRender::CmpModelRender(const std::string& modelName)
:	m_modelName(modelName), m_animController(new AnimationController), m_highlights(false)
{}

//#################### PUBLIC METHODS ####################
void CmpModelRender::set_highlights(bool enabled)
{
	m_highlights = enabled;
}

//#################### PROTECTED METHODS ####################
const Model_Ptr& CmpModelRender::model()	{ return m_objectManager->model_manager()->model(m_modelName); }
Model_CPtr CmpModelRender::model() const	{ return m_objectManager->model_manager()->model(m_modelName); }

void CmpModelRender::render_bounds(const Vector3d& p) const
{
	ICmpSimulation_Ptr cmpSimulation = m_objectManager->get_component(m_objectID, cmpSimulation);
	if(cmpSimulation)
	{
		const Bounds_CPtr& bounds = m_objectManager->bounds_manager()->bounds(cmpSimulation->bounds_group(), cmpSimulation->posture());
		bounds->render(p);
	}
}

void CmpModelRender::render_nuv_axes(const Vector3d& p, const Vector3d& n, const Vector3d& u, const Vector3d& v)
{
	Vector3d pn = p + n;
	Vector3d pu = p + u;
	Vector3d pv = p + v;
	glBegin(GL_LINES);
		glColor3d(1,0,0);	glVertex3d(p.x, p.y, p.z);	glVertex3d(pn.x, pn.y, pn.z);
		glColor3d(0,1,0);	glVertex3d(p.x, p.y, p.z);	glVertex3d(pu.x, pu.y, pu.z);
		glColor3d(0,0,1);	glVertex3d(p.x, p.y, p.z);	glVertex3d(pv.x, pv.y, pv.z);
	glEnd();
}

void CmpModelRender::set_object_manager(ObjectManager *objectManager)
{
	IObjectComponent::set_object_manager(objectManager);

	// The skeleton for the animation controller can only be set after
	// we have a handle to the model manager, so it must happen after
	// the object manager pointer has been set. Note that during level
	// building the full models aren't actually loaded in, so it's
	// important to check that the skeleton's valid here.
	Skeleton_CPtr s = skeleton();
	if(s) m_animController->set_skeleton(s);
}

Skeleton_Ptr CmpModelRender::skeleton()
{
	const Model_Ptr& model = m_objectManager->model_manager()->model(m_modelName);
	if(model) return model->skeleton();
	else return Skeleton_Ptr();
}

Skeleton_CPtr CmpModelRender::skeleton() const
{
	Model_CPtr model = m_objectManager->model_manager()->model(m_modelName);
	if(model) return model->skeleton();
	else return Skeleton_CPtr();
}

}
