/***
 * hesperus: CmpBasicModelRender.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMPBASICMODELRENDER
#define H_HESP_CMPBASICMODELRENDER

#ifdef _MSC_VER
	#pragma warning(disable:4250)	// Disable the spurious VC++ warning about inheritance via the dominance rule: that's what I intend here
#endif

#include "CmpModelRender.h"
#include "ICmpBasicModelRender.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class ICmpUsable> ICmpUsable_Ptr;

class CmpBasicModelRender : public ICmpBasicModelRender, public CmpModelRender
{
	//#################### CONSTRUCTORS ####################
public:
	explicit CmpBasicModelRender(const std::string& modelName);

	//#################### STATIC FACTORY METHODS ####################
public:
	static IObjectComponent_Ptr load(const Properties& properties);

	//#################### PUBLIC METHODS ####################
public:
	void check_dependencies() const;
	void render() const;
	void render_child() const;
	void render_first_person() const;
	Properties save() const;
	void update_animation(int milliseconds);
	void update_child_animation(int milliseconds, const BoneHierarchy_Ptr& parent, const std::string& parentBoneName, const RBTMatrix_CPtr& parentMatrix);

	//#################### PRIVATE METHODS ####################
private:
	static RBTMatrix_CPtr construct_first_person_matrix();
	static RBTMatrix_CPtr construct_model_matrix(const Vector3d& p, const Vector3d& n, const Vector3d& u, const Vector3d& v);
	void update_hotspots(const ICmpUsable_Ptr& cmpItemUsable, const RBTMatrix_CPtr& parentMatrix) const;
};

}

#endif
