/***
 * hesperus: CmpCharacterModelRender.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpCharacterModelRender.h"

#include <cmath>

#include <hesp/ogl/WrappedGL.h>

#include <hesp/axes/NUVAxes.h>
#include <hesp/level/models/AnimationController.h>
#include <hesp/level/models/Model.h>
#include <hesp/level/models/Skeleton.h>
#include <hesp/math/matrices/RBTMatrix.h>
#include <hesp/util/Properties.h>
#include "ICmpAnimChooser.h"
#include "ICmpBasicModelRender.h"
#include "ICmpInventory.h"
#include "ICmpOrientation.h"
#include "ICmpOwnable.h"
#include "ICmpSimulation.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpCharacterModelRender::CmpCharacterModelRender(const BoneModifierMap& inclineBones, const std::string& modelName)
:	CmpModelRender(modelName), m_inclineBones(inclineBones)
{}

//#################### STATIC FACTORY METHODS ####################
IObjectComponent_Ptr CmpCharacterModelRender::load(const Properties& properties)
{
	return IObjectComponent_Ptr(new CmpCharacterModelRender(
		properties.get<BoneModifierMap>("InclineBones"),
		properties.get<std::string>("ModelName")
	));
}

//#################### PUBLIC METHODS ####################
void CmpCharacterModelRender::check_dependencies() const
{
	check_dependency<ICmpAnimChooser>();
	check_dependency<ICmpInventory>();
	check_dependency<ICmpOrientation>();
	check_dependency<ICmpSimulation>();
}

void CmpCharacterModelRender::render() const
{
	ICmpOrientation_CPtr cmpOrientation = m_objectManager->get_component(m_objectID, cmpOrientation);
	ICmpPosition_CPtr cmpPosition = m_objectManager->get_component(m_objectID, cmpPosition);

	const Vector3d& p = cmpPosition->position();
	const Vector3d& n = cmpOrientation->nuv_axes()->n();
	const Vector3d& u = cmpOrientation->nuv_axes()->u();
	const Vector3d& v = cmpOrientation->nuv_axes()->v();

	if(m_modelPose != NULL)
	{
		RBTMatrix_CPtr mat = construct_model_matrix(p, n, u, v);
		glPushMatrix();
			glMultMatrixd(&mat->rep()[0]);

			// Render the model.
			model()->render(m_modelPose);

			// Render the active item (if any).
			ICmpInventory_Ptr cmpInventory = m_objectManager->get_component(m_objectID, cmpInventory);	assert(cmpInventory);
			ObjectID activeItem = cmpInventory->active_item();
			if(activeItem.valid())
			{
				ICmpBasicModelRender_Ptr cmpItemRender = m_objectManager->get_component(activeItem, cmpItemRender);
				if(cmpItemRender)
				{
					cmpItemRender->render_child();
				}
			}
		glPopMatrix();
	}

	if(m_highlights)
	{
		// If the object should be highlighted, render the object's bounds.
		render_bounds(p);
	}

	// Render the object's NUV axes.
	render_nuv_axes(p, n, u, v);
}

void CmpCharacterModelRender::render_first_person() const
{
	ICmpInventory_Ptr cmpInventory = m_objectManager->get_component(m_objectID, cmpInventory);
	assert(cmpInventory);

	ObjectID activeItem = cmpInventory->active_item();
	if(!activeItem.valid()) return;

	ICmpBasicModelRender_Ptr cmpItemRender = m_objectManager->get_component(activeItem, cmpItemRender);
	if(!cmpItemRender) return;

	cmpItemRender->render_first_person();
	render_crosshair();
}

Properties CmpCharacterModelRender::save() const
{
	Properties properties;
	properties.set("InclineBones", m_inclineBones);
	properties.set("ModelName", m_modelName);
	return properties;
}

void CmpCharacterModelRender::update_animation(int milliseconds, const std::vector<CollisionPolygon_Ptr>& polygons,
											   const OnionTree_CPtr& tree, const NavManager_CPtr& navManager)
{
	// Decide which animation should be playing, and update it.
	ICmpAnimChooser_Ptr cmpAnimChooser = m_objectManager->get_component(m_objectID, cmpAnimChooser);	assert(cmpAnimChooser);
	m_animController->request_animation(cmpAnimChooser->choose_animation(polygons, tree, navManager));
	m_animController->update(milliseconds);

	// Clear any existing pose modifiers.
	m_animController->clear_pose_modifiers();

	// Determine the animation extension of any carried item in order to determine which bones need to be inclined.
	std::string animExtension = "";		// the explicit initialisation is to make it clear that "" is the default

	ICmpInventory_Ptr cmpInventory = m_objectManager->get_component(m_objectID, cmpInventory);	assert(cmpInventory);
	ObjectID activeItem = cmpInventory->active_item();
	if(activeItem.valid())
	{
		ICmpOwnable_Ptr cmpItemOwnable = m_objectManager->get_component(activeItem, cmpItemOwnable);	assert(cmpItemOwnable);
		animExtension = cmpItemOwnable->anim_extension();
	}

	// Calculate the inclination of the object's coordinate system and apply pose modifiers to the relevant bones.
	BoneModifierMap::const_iterator it = m_inclineBones.find(animExtension);
	if(it != m_inclineBones.end())
	{
		ICmpOrientation_Ptr cmpOrientation = m_objectManager->get_component(m_objectID, cmpOrientation);
		const Vector3d& n = cmpOrientation->nuv_axes()->n();

		double sinInclination = n.z / n.length();
		if(sinInclination < -1) sinInclination = -1;
		if(sinInclination > 1) sinInclination = 1;
		double inclination = asin(sinInclination);

		for(std::map<std::string,Vector3d>::const_iterator jt=it->second.begin(), jend=it->second.end(); jt!=jend; ++jt)
		{
			m_animController->set_pose_modifier(jt->first, PoseModifier(jt->second, -inclination));
		}
	}

	// Configure the pose.
	m_modelPose = model()->configure_pose(m_animController);

	// Update the animation for the active item (if any), e.g. the weapon being carried.
	if(activeItem.valid())
	{
		ICmpOrientation_CPtr cmpOrientation = m_objectManager->get_component(m_objectID, cmpOrientation);
		ICmpPosition_CPtr cmpPosition = m_objectManager->get_component(m_objectID, cmpPosition);

		const Vector3d& p = cmpPosition->position();
		const Vector3d& n = cmpOrientation->nuv_axes()->n();
		const Vector3d& u = cmpOrientation->nuv_axes()->u();
		const Vector3d& v = cmpOrientation->nuv_axes()->v();

		RBTMatrix_CPtr modelMatrix = construct_model_matrix(p, n, u, v);

		ICmpOwnable_Ptr cmpItemOwnable = m_objectManager->get_component(activeItem, cmpItemOwnable);	assert(cmpItemOwnable);
		ICmpBasicModelRender_Ptr cmpItemRender = m_objectManager->get_component(activeItem, cmpItemRender);
		if(cmpItemRender)
		{
			cmpItemRender->update_child_animation(milliseconds, skeleton()->bone_hierarchy(), cmpItemOwnable->attach_point(),
												  modelMatrix, polygons, tree, navManager);
		}
	}
}

//#################### PRIVATE METHODS ####################
RBTMatrix_CPtr CmpCharacterModelRender::construct_model_matrix(const Vector3d& p, const Vector3d& n, const Vector3d& u,
															   const Vector3d& v)
{
	// Note:	The vertical axis of a character model does not rotate, unlike that for a
	//			normal model. In other words, looking up/down should have no effect on the
	//			way the model is rendered in this case (e.g. the character's head stays above
	//			its feet, for biped models). This necessitates the modified model matrix
	//			shown below.

	// Project u and n into the horizontal plane (i.e. z = 0) to form uH and nH respectively.
	// Note that the camera is prevented from ever pointing directly upwards/downwards, so
	// renormalizing the resulting vectors isn't a problem.
	Vector3d uH = u;	uH.z = 0;	uH.normalize();
	Vector3d nH = n;	nH.z = 0;	nH.normalize();

	// Note:	This matrix maps x -> uH, -y -> nH, z -> z, and translates by p. Since models are
	//			built in Blender facing in the -y direction, this turns out to be exactly the
	//			transformation required to render the models with the correct position and
	//			orientation.
	RBTMatrix_Ptr mat = RBTMatrix::zeros();
	RBTMatrix& m = *mat;
	m(0,0) = uH.x;		m(0,1) = -nH.x;		/*m(0,2) = 0;*/		m(0,3) = p.x;
	m(1,0) = uH.y;		m(1,1) = -nH.y;		/*m(1,2) = 0;*/		m(1,3) = p.y;
	/*m(2,0) = 0;*/		/*m(2,1) = 0;*/		m(2,2) = 1;			m(2,3) = p.z;

	return mat;
}

void CmpCharacterModelRender::render_crosshair()
{
	glPushMatrix();
	glPushAttrib(GL_ENABLE_BIT);

	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);		// disable z-buffer testing
	glDisable(GL_DEPTH_WRITEMASK);	// disable z-buffer writing

	const double Z_OFFSET = 30;

	glColor3d(1,1,1);
	glBegin(GL_LINES);
		glVertex3d(-1,0,-Z_OFFSET);		glVertex3d(1,0,-Z_OFFSET);
		glVertex3d(0,-1,-Z_OFFSET);		glVertex3d(0,1,-Z_OFFSET);
	glEnd();

	glPopAttrib();
	glPopMatrix();
}

}
