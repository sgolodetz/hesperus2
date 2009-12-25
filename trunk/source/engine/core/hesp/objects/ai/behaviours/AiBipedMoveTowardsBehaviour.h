/***
 * hesperus: AiBipedMoveTowardsBehaviour.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_AIBIPEDMOVETOWARDSBEHAVIOUR
#define H_HESP_AIBIPEDMOVETOWARDSBEHAVIOUR

#include <hesp/math/vectors/Vector3.h>
#include <hesp/objects/base/ObjectID.h>
#include "AiBehaviour.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
class ObjectManager;

class AiBipedMoveTowardsBehaviour : public AiBehaviour
{
	//#################### PRIVATE VARIABLES ####################
private:
	ObjectID m_objectID;
	const ObjectManager *m_objectManager;
	Vector3d m_dest;

	//#################### CONSTRUCTORS ####################
public:
	explicit AiBipedMoveTowardsBehaviour(const ObjectID& objectID, const ObjectManager *objectManager, const Vector3d& dest);

	//#################### PRIVATE ABSTRACT METHODS ####################
private:
	virtual ObjectCommand_Ptr generate_command(const ObjectID& objectID, const Vector3d& dir) const = 0;
	virtual double success_radius() const = 0;

	//#################### PUBLIC METHODS ####################
public:
	std::vector<ObjectCommand_Ptr> generate_commands();
	Status status() const;
};

}

#endif
