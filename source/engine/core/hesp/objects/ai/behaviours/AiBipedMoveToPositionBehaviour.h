/***
 * hesperus: AiBipedMoveToPositionBehaviour.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_AIBIPEDMOVETOPOSITIONBEHAVIOUR
#define H_HESP_AIBIPEDMOVETOPOSITIONBEHAVIOUR

#include <hesp/math/vectors/Vector3.h>
#include <hesp/objects/base/ObjectID.h>
#include "AiBehaviour.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class AiSequenceBehaviour> AiSequenceBehaviour_Ptr;
class ObjectManager;

class AiBipedMoveToPositionBehaviour : public AiBehaviour
{
	//#################### PRIVATE VARIABLES ####################
private:
	ObjectID m_objectID;
	const ObjectManager *m_objectManager;
	Vector3d m_dest;

	AiSequenceBehaviour_Ptr m_plan;
	Status m_status;

	//#################### CONSTRUCTORS ####################
public:
	AiBipedMoveToPositionBehaviour(const ObjectID& objectID, const ObjectManager *objectManager, const Vector3d& dest);

	//#################### PUBLIC METHODS ####################
public:
	std::vector<ObjectCommand_Ptr> generate_commands();
	Status status() const;

	//#################### PRIVATE METHODS ####################
private:
	void make_plan();
};

}

#endif
