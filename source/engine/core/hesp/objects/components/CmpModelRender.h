/***
 * hesperus: CmpModelRender.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMPMODELRENDER
#define H_HESP_CMPMODELRENDER

#include <hesp/math/vectors/Vector3.h>
#include "ICmpModelRender.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class AnimationController> AnimationController_Ptr;
typedef shared_ptr<const class ConfiguredPose> ConfiguredPose_CPtr;
typedef shared_ptr<class Model> Model_Ptr;
typedef shared_ptr<const class Model> Model_CPtr;
typedef shared_ptr<class Skeleton> Skeleton_Ptr;
typedef shared_ptr<const class Skeleton> Skeleton_CPtr;

class CmpModelRender : public virtual ICmpModelRender
{
	//#################### PROTECTED VARIABLES ####################
protected:
	AnimationController_Ptr m_animController;
	bool m_highlights;
	std::string m_modelName;
	ConfiguredPose_CPtr m_modelPose;

	//#################### CONSTRUCTORS ####################
protected:
	explicit CmpModelRender(const std::string& modelName);

	//#################### PUBLIC METHODS ####################
public:
	void set_highlights(bool enabled);

	//#################### PROTECTED METHODS ####################
protected:
	const Model_Ptr& model();
	Model_CPtr model() const;
	void render_bounds(const Vector3d& p) const;
	static void render_nuv_axes(const Vector3d& p, const Vector3d& n, const Vector3d& u, const Vector3d& v);
	void set_object_manager(ObjectManager *objectManager);
	Skeleton_Ptr skeleton();
	Skeleton_CPtr skeleton() const;
};

}

#endif
