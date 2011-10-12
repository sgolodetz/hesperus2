/***
 * hesperus: AiBipedRunTowardsBehaviour.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_AIBIPEDRUNTOWARDSBEHAVIOUR
#define H_HESP_AIBIPEDRUNTOWARDSBEHAVIOUR

#include "AiBipedMoveTowardsBehaviour.h"

namespace hesp {

class AiBipedRunTowardsBehaviour : public AiBipedMoveTowardsBehaviour
{
	//#################### CONSTRUCTORS ####################
public:
	explicit AiBipedRunTowardsBehaviour(const ObjectID& objectID, const ObjectManager *objectManager, const Vector3d& dest);

	//#################### PRIVATE METHODS ####################
private:
	ObjectCommand_Ptr generate_command(const ObjectID& objectID, const Vector3d& dir) const;
	double success_radius() const;
};

}

#endif
