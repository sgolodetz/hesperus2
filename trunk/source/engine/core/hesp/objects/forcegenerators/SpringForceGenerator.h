/***
 * hesperus: SpringForceGenerator.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_SPRINGFORCEGENERATOR
#define H_HESP_SPRINGFORCEGENERATOR

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <hesp/physics/ForceGenerator.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<const PhysicsObject> PhysicsObject_CPtr;

class SpringForceGenerator : public ForceGenerator
{
	//#################### PRIVATE VARIABLES ####################
private:
	double m_naturalLength;
	double m_springConstant;

	PhysicsObject_CPtr m_otherEnd;
	PhysicsObjectID m_otherEndID;

	//#################### CONSTRUCTORS ####################
public:
	SpringForceGenerator(double naturalLength, double springConstant, const PhysicsObject_CPtr& otherEnd, PhysicsObjectID otherEndID);

	//#################### PUBLIC METHODS ####################
public:
	std::vector<PhysicsObjectID> referenced_objects() const;
	void update_force(PhysicsObject& object) const;
};

}

#endif
