/***
 * hesperus: ForceGenerator.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_FORCEGENERATOR
#define H_HESP_FORCEGENERATOR

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
class PhysicsObject;

class ForceGenerator
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~ForceGenerator() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void update_force(PhysicsObject& object) const = 0;
};

}

#endif
