/***
 * hesperus: CmdBipedMove.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMDBIPEDMOVE
#define H_HESP_CMDBIPEDMOVE

#include <hesp/level/objects/base/ObjectCommand.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class ICmpBipedAnimChooser> ICmpBipedAnimChooser_Ptr;
typedef shared_ptr<const class ICmpMovement> ICmpMovement_CPtr;

class CmdBipedMove : public ObjectCommand
{
	//#################### PRIVATE VARIABLES ####################
private:
	ObjectID m_objectID;
	Vector3d m_dir;

	//#################### CONSTRUCTORS ####################
public:
	CmdBipedMove(const ObjectID& objectID, const Vector3d& dir);

	//#################### PRIVATE ABSTRACT METHODS ####################
private:
	virtual double lookup_speed(const ICmpMovement_CPtr& cmpMovement) const = 0;
	virtual void set_anim_flag(const ICmpBipedAnimChooser_Ptr& cmpAnimChooser) const = 0;

	//#################### PUBLIC METHODS ####################
public:
	void execute(const ObjectManager_Ptr& objectManager, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_CPtr& tree, const NavManager_CPtr& navManager, int milliseconds);
};

}

#endif
