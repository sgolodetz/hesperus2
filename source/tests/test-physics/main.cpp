/***
 * test-physics: main.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <iostream>

#include <hesp/bounds/BoundsManager.h>
#include <hesp/bounds/SphereBounds.h>
#include <hesp/exceptions/Exception.h>
#include <hesp/objects/forcegenerators/SpringForceGenerator.h>
#include <hesp/objects/forcegenerators/WeightForceGenerator.h>
#include <hesp/physics/NormalPhysicsObject.h>
#include <hesp/physics/PhysicsSystem.h>
using namespace hesp;

void output(const PhysicsObject_Ptr& fixed, const PhysicsObject_Ptr& dynamic)
{
	std::cout << "Fixed: " << fixed->position() << '\n';
	std::cout << "Dynamic: " << dynamic->position() << ' ' << dynamic->velocity() << '\n';
	std::cout << '\n';
}

int main()
try
{
	// Set up the bounds manager (this is a necessary step, even though we're not resolving any contacts in this test).
	std::vector<Bounds_CPtr> bounds;
	std::map<std::string,BoundsManager::BoundsGroup> groups;
	std::map<std::string,int> lookup;
	boost::dynamic_bitset<> navFlags;

	bounds.push_back(Bounds_CPtr(new SphereBounds(1.0)));
	lookup.insert(std::make_pair("normalsphere", 0));
	BoundsManager::BoundsGroup sphereGroup;
	sphereGroup.insert(std::make_pair("default", "normalsphere"));
	groups.insert(std::make_pair("sphere", sphereGroup));

	BoundsManager_CPtr boundsManager(new BoundsManager(bounds, groups, lookup, navFlags));

	// Set up the physics system.
	PhysicsSystem physicsSystem;

	// Define the physics objects.
	PhysicsObject_Ptr fixed(new NormalPhysicsObject("sphere", 1.0, 1.0, PM_ITEM, ObjectID(), Vector3d(0,0,20), "default"));
	PhysicsObject_Ptr dynamic(new NormalPhysicsObject("sphere", 1.0, 1.0, PM_ITEM, ObjectID(), Vector3d(0,0,10), "default"));

	// Register them.
	PhysicsObjectHandle fixedHandle = physicsSystem.register_object(fixed);
	PhysicsObjectHandle dynamicHandle = physicsSystem.register_object(dynamic);

	// Add a weight force generator to the dynamic object.
	physicsSystem.set_force_generator(dynamicHandle, "Weight", ForceGenerator_CPtr(new WeightForceGenerator(10.0)));

	// Add a spring between them. Note that no force should be applied to the *fixed* object here (think about it!).
	const double naturalLength = 10;
	const double springConstant = 2;
	physicsSystem.set_force_generator(dynamicHandle, "Spring", ForceGenerator_CPtr(new SpringForceGenerator(naturalLength, springConstant, fixed, *fixedHandle)));

	// Update the physics system over a number of frames and output the results.
	const int milliseconds = 10;

	output(fixed, dynamic);

	for(int i=0; i<250; ++i)
	{
		physicsSystem.update(boundsManager, OnionTree_CPtr(), milliseconds);
		output(fixed, dynamic);
	}

	std::cout << "\n******\n\n";

	// Kill off the fixed object (the spring force generator which references it should be destroyed by this when we update).
	fixedHandle.reset();

	// Remove the weight force generator on the dynamic object.
	physicsSystem.remove_force_generator(dynamicHandle, "Weight");

	// Update the physics system over a few more frames and output the results. Since there are no forces
	// applied anywhere now, the dynamic object's velocity should stay constant.
	for(int i=0; i<10; ++i)
	{
		physicsSystem.update(boundsManager, OnionTree_CPtr(), milliseconds);
		output(fixed, dynamic);
	}

	return 0;
}
catch(Exception& e)
{
	std::cout << e.cause() << '\n';
}
