/***
 * hesperus: AiGotoPositionBehaviour.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_AIGOTOPOSITIONBEHAVIOUR
#define H_HESP_AIGOTOPOSITIONBEHAVIOUR

#include <hesp/math/vectors/Vector3.h>
#include <hesp/objects/base/ObjectID.h>
#include "AiBehaviour.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
class ObjectManager;

class AiGotoPositionBehaviour : public AiBehaviour
{
	//#################### PRIVATE VARIABLES ####################
private:
	ObjectID m_objectID;
	const ObjectManager *m_objectManager;
	Vector3d m_dest;

	//#################### CONSTRUCTORS ####################
public:
	AiGotoPositionBehaviour(const ObjectID& objectID, const ObjectManager *objectManager, const Vector3d& dest);

	//#################### PUBLIC METHODS ####################
public:
	std::vector<ObjectCommand_Ptr> generate_commands(const SteeringPipeline_CPtr& steeringPipeline);
};

}

#endif
