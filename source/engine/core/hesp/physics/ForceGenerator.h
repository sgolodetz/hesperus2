/***
 * hesperus: ForceGenerator.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_FORCEGENERATOR
#define H_HESP_FORCEGENERATOR

#include <vector>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
class PhysicsObject;

//#################### TYPEDEFS ####################
typedef int PhysicsObjectID;

class ForceGenerator
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~ForceGenerator() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual std::vector<PhysicsObjectID> referenced_objects() const = 0;
	virtual void update_force(PhysicsObject& object) const = 0;
};

}

#endif
