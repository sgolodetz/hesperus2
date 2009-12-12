/***
 * hesperus: CmpCharacterModelRender.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMPCHARACTERMODELRENDER
#define H_HESP_CMPCHARACTERMODELRENDER

#include "CmpModelRender.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<const class RBTMatrix> RBTMatrix_CPtr;

class CmpCharacterModelRender : public CmpModelRender
{
	//#################### TYPEDEFS ####################
private:
	typedef std::map<std::string, std::map<std::string,Vector3d> > BoneModifierMap;

	//#################### PRIVATE VARIABLES ####################
private:
	BoneModifierMap m_inclineBones;

	//#################### CONSTRUCTORS ####################
public:
	explicit CmpCharacterModelRender(const BoneModifierMap& inclineBones, const std::string& modelName);

	//#################### STATIC FACTORY METHODS ####################
public:
	static IObjectComponent_Ptr load(const Properties& properties);

	//#################### PUBLIC METHODS ####################
public:
	void check_dependencies() const;
	void render() const;
	void render_first_person() const;
	Properties save() const;
	void update_animation(int milliseconds, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_CPtr& tree, const NavManager_CPtr& navManager);

	std::string own_type() const			{ return "CharacterModelRender"; }
	static std::string static_own_type()	{ return "CharacterModelRender"; }

	//#################### PRIVATE METHODS ####################
private:
	static RBTMatrix_CPtr construct_model_matrix(const Vector3d& p, const Vector3d& n, const Vector3d& u, const Vector3d& v);
	static void render_crosshair();
};

}

#endif
