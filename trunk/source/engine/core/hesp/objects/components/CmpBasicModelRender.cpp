/***
 * hesperus: CmpBasicModelRender.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpBasicModelRender.h"

#include <hesp/ogl/WrappedGL.h>

#include <hesp/axes/NUVAxes.h>
#include <hesp/math/matrices/RBTMatrix.h>
#include <hesp/models/AnimationController.h>
#include <hesp/models/BoneHierarchy.h>
#include <hesp/models/ConfiguredBone.h>
#include <hesp/models/ConfiguredPose.h>
#include <hesp/models/Model.h>
#include <hesp/models/Skeleton.h>
#include <hesp/util/Properties.h>
#include "ICmpAnimChooser.h"
#include "ICmpOrientation.h"
#include "ICmpPosition.h"
#include "ICmpUsable.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpBasicModelRender::CmpBasicModelRender(const std::string& modelName)
:	CmpModelRender(modelName)
{}

//#################### STATIC FACTORY METHODS ####################
IObjectComponent_Ptr CmpBasicModelRender::load(const Properties& properties)
{
	return IObjectComponent_Ptr(new CmpBasicModelRender(properties.get<std::string>("ModelName")));
}

//#################### PUBLIC METHODS ####################
void CmpBasicModelRender::check_dependencies() const
{
	check_dependency<ICmpOrientation>();
	check_dependency<ICmpPosition>();
}

void CmpBasicModelRender::render() const
{
	ICmpOrientation_CPtr cmpOrientation = m_objectManager->get_component(m_objectID, cmpOrientation);
	ICmpPosition_CPtr cmpPosition = m_objectManager->get_component(m_objectID, cmpPosition);

	const Vector3d& p = cmpPosition->position();
	const Vector3d& n = cmpOrientation->nuv_axes()->n();
	const Vector3d& u = cmpOrientation->nuv_axes()->u();
	const Vector3d& v = cmpOrientation->nuv_axes()->v();

	if(m_modelPose != NULL)
	{
		// Render the model.
		RBTMatrix_CPtr mat = construct_model_matrix(p, n, u, v);
		glPushMatrix();
			glMultMatrixd(&mat->rep()[0]);
			model()->render(m_modelPose);
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

void CmpBasicModelRender::render_child() const
{
	if(m_modelPose != NULL)
	{
		// Render the model.
		model()->render(m_modelPose);
	}
}

void CmpBasicModelRender::render_first_person() const
{
	// Calculate the *unattached* pose of the model for first-person rendering.
	ConfiguredPose_Ptr firstPersonPose = model()->configure_pose(m_animController);

	// Render the model.
	glPushMatrix();
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);		// disable z-buffer testing
	glDisable(GL_DEPTH_WRITEMASK);	// disable z-buffer writing
		RBTMatrix_CPtr firstPersonMatrix = construct_first_person_matrix();
		glLoadMatrixd(&firstPersonMatrix->rep()[0]);
		model()->render(firstPersonPose);
	glPopAttrib();
	glPopMatrix();
}

Properties CmpBasicModelRender::save() const
{
	Properties properties;
	properties.set("ModelName", m_modelName);
	return properties;
}

void CmpBasicModelRender::update_animation(int milliseconds, const std::vector<CollisionPolygon_Ptr>& polygons,
										   const OnionTree_CPtr& tree, const NavManager_CPtr& navManager)
{
	// Decide which animation should be playing, and update it.
	ICmpAnimChooser_Ptr cmpAnimChooser = m_objectManager->get_component(m_objectID, cmpAnimChooser);
	if(cmpAnimChooser) m_animController->request_animation(cmpAnimChooser->choose_animation(polygons, tree, navManager));
	m_animController->update(milliseconds);

	// Configure the pose.
	m_modelPose = model()->configure_pose(m_animController);
}

void CmpBasicModelRender::update_child_animation(int milliseconds, const BoneHierarchy_Ptr& parent,
												 const std::string& parentBoneName, const RBTMatrix_CPtr& parentMatrix,
												 const std::vector<CollisionPolygon_Ptr>& polygons,
												 const OnionTree_CPtr& tree, const NavManager_CPtr& navManager)
{
	BoneHierarchy_Ptr child = skeleton()->bone_hierarchy();
	child->attach_to_parent(parent, parentBoneName);
	update_animation(milliseconds, polygons, tree, navManager);
	child->detach_from_parent();

	// If this object's a usable one (e.g. a weapon), update the positions and orientations of its hotspots.
	ICmpUsable_Ptr cmpUsable = m_objectManager->get_component(m_objectID, cmpUsable);
	if(cmpUsable)
	{
		update_hotspots(cmpUsable, parentMatrix);
	}
}

//#################### PRIVATE METHODS ####################
RBTMatrix_CPtr CmpBasicModelRender::construct_first_person_matrix()
{
	RBTMatrix_Ptr mat = RBTMatrix::zeros();
	RBTMatrix& m = *mat;

	const int N_OFFSET = 3;		// the offset in the camera's n direction at which the item model will be rendered

	// Note:	This matrix maps x -> x, -y -> y, z -> -z, and translates by (1,-1,-N_OFFSET).
	//			Since item models are built in Blender with the direction they're supposed to
	//			point (e.g. their muzzle direction) in the z direction, and their up direction
	//			in the -y direction, and since by default we're looking down the -z axis in
	//			OpenGL with y being the up direction, this is an appropriate transformation to
	//			get the item model to render with the correct position and orientation.
	m(0,0) = 1;			/*m(0,1) = 0;*/		/*m(0,2) = 0;*/		m(0,3) = 1;
	/*m(1,0) = 0;*/		m(1,1) = -1;		/*m(1,2) = 0;*/		m(1,3) = -1;
	/*m(2,0) = 0;*/		/*m(2,1) = 0;*/		m(2,2) = -1;		m(2,3) = -N_OFFSET;

	return mat;
}

RBTMatrix_CPtr CmpBasicModelRender::construct_model_matrix(const Vector3d& p, const Vector3d& n, const Vector3d& u,
														   const Vector3d& v)
{
	// Note:	This matrix maps x -> u, -y -> n, z -> v, and translates by p. Since models are
	//			built in Blender facing in the -y direction, this turns out to be exactly the
	//			transformation required to render the models with the correct position and
	//			orientation.
	RBTMatrix_Ptr mat = RBTMatrix::zeros();
	RBTMatrix& m = *mat;
	m(0,0) = u.x;		m(0,1) = -n.x;		m(0,2) = v.x;		m(0,3) = p.x;
	m(1,0) = u.y;		m(1,1) = -n.y;		m(1,2) = v.y;		m(1,3) = p.y;
	m(2,0) = u.z;		m(2,1) = -n.z;		m(2,2) = v.z;		m(2,3) = p.z;

	return mat;
}

void CmpBasicModelRender::update_hotspots(const ICmpUsable_Ptr& cmpUsable, const RBTMatrix_CPtr& parentMatrix) const
{
	BoneHierarchy_CPtr boneHierarchy = skeleton()->bone_hierarchy();

	const std::vector<std::string>& hotspots = cmpUsable->hotspots();
	for(size_t i=0, size=hotspots.size(); i<size; ++i)
	{
		const std::string& hotspot = hotspots[i];
		int boneIndex = boneHierarchy->find_bone(hotspot);
		ConfiguredBone_CPtr configuredBone = m_modelPose->bones(boneIndex);

		// Calculate the hotspot position and orientation from those of the bone (in the model-local coordinate system).
		Vector3d orientation = parentMatrix->apply_to_vector(configuredBone->orientation());
		Vector3d position = parentMatrix->apply_to_point(configuredBone->position());

		cmpUsable->set_hotspot_orientation(hotspot, orientation);
		cmpUsable->set_hotspot_position(hotspot, position);
	}
}

}
