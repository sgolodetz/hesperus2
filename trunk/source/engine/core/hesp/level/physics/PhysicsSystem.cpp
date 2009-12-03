/***
 * hesperus: PhysicsSystem.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "PhysicsSystem.h"

#include <boost/pointer_cast.hpp>

#include "BroadPhaseCollisionDetector.h"
#include "ContactResolver.h"
#include "ForceGenerator.h"
#include "NarrowPhaseCollisionDetector.h"
#include "NormalPhysicsObject.h"
#include "PhysicsObject.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
PhysicsObjectHandle PhysicsSystem::register_object(const PhysicsObject_Ptr& object)
{
	int id = m_idAllocator.allocate();
	shared_ptr<int> sid(new int(id));
	weak_ptr<int> wid(sid);
	object->set_id(id);
	m_objects.insert(std::make_pair(id, ObjectData(wid, object)));
	m_forceGeneratorRegistry.register_id(id);
	return sid;
}

void PhysicsSystem::remove_contact_resolver(PhysicsMaterial material1, PhysicsMaterial material2)
{
	m_contactResolverRegistry.remove_resolver(material1, material2);
}

void PhysicsSystem::remove_force_generator(const PhysicsObjectHandle& handle, const std::string& forceName)
{
	m_forceGeneratorRegistry.remove_generator(*handle, forceName);
}

void PhysicsSystem::set_contact_resolver(PhysicsMaterial material1, PhysicsMaterial material2,
										 const ContactResolver_CPtr& resolver)
{
	m_contactResolverRegistry.set_resolver(material1, material2, resolver);
}

void PhysicsSystem::set_force_generator(const PhysicsObjectHandle& handle, const std::string& forceName,
										const ForceGenerator_CPtr& generator)
{
	m_forceGeneratorRegistry.set_generator(*handle, forceName, generator);
}

void PhysicsSystem::update(const BoundsManager_CPtr& boundsManager, const OnionTree_CPtr& tree, int milliseconds)
{
	typedef std::vector<Contact_CPtr> ContactSet;

	// Step 1:	Check for any objects which no longer exist, and deregister them.
	check_objects();

	// Step 2:	Do the physical simulation of the objects.
	simulate_objects(milliseconds);

	// Step 3:	Generate all necessary contacts for them.
	ContactSet contacts;
	detect_contacts(contacts, boundsManager, tree);

	// Step 4:	Batch the contacts into groups which might mutually interact.
	std::vector<ContactSet> batches = batch_contacts(contacts);

	// Step 5:	Resolve each batch of contacts in turn.
	for(std::vector<ContactSet>::const_iterator it=batches.begin(), iend=batches.end(); it!=iend; ++it)
	{
		resolve_contacts(*it, tree);
	}
}

//#################### PRIVATE METHODS ####################
/**
Group the contacts into batches which might interact with each other.

@param contacts	The array of contacts which need resolving
@return			The contact batches
*/
std::vector<std::vector<Contact_CPtr> > PhysicsSystem::batch_contacts(const std::vector<Contact_CPtr>& contacts)
{
	// TODO: This is the simplest possible batching processor (for test purposes only) - it needs doing properly.
	std::vector<std::vector<Contact_CPtr> > batches;
	if(!contacts.empty()) batches.push_back(contacts);
	return batches;
}

/**
Checks for objects which no longer exist in order to remove them from consideration.
*/
void PhysicsSystem::check_objects()
{
	for(std::map<int,ObjectData>::iterator it=m_objects.begin(), iend=m_objects.end(); it!=iend;)
	{
		shared_ptr<int> sid = it->second.m_wid.lock();
		if(!sid)
		{
			int id = it->first;
			m_idAllocator.deallocate(id);
			m_forceGeneratorRegistry.deregister_id(id);
			it = m_objects.erase(it);
		}
		else ++it;
	}
}

/**
Perform collision detection and generate any necessary contacts.

@param contacts			Used to add to the array of contacts which need resolving
@param boundsManager	The bounds manager which contains the bounds for all the objects
@param tree				The tree representing the world for collision purposes
*/
void PhysicsSystem::detect_contacts(std::vector<Contact_CPtr>& contacts,
									const BoundsManager_CPtr& boundsManager, const OnionTree_CPtr& tree)
{
	BroadPhaseCollisionDetector broadDetector(boundsManager);
	NarrowPhaseCollisionDetector narrowDetector(boundsManager, tree);

	// Detect object-object contacts.
	for(std::map<int,ObjectData>::const_iterator it=m_objects.begin(), iend=m_objects.end(); it!=iend; ++it)
	{
		broadDetector.add_object(it->second.m_object);
	}

	typedef BroadPhaseCollisionDetector::ObjectPairs ObjectPairs;
	const ObjectPairs& potentialCollisions = broadDetector.potential_collisions();

	for(ObjectPairs::const_iterator it=potentialCollisions.begin(), iend=potentialCollisions.end(); it!=iend; ++it)
	{
		PhysicsObject& objectA = *it->first;
		PhysicsObject& objectB = *it->second;
		boost::optional<Contact> contact = narrowDetector.object_vs_object(objectA, objectB);
		if(contact) contacts.push_back(Contact_CPtr(new Contact(*contact)));
	}

	if(tree)
	{
		// Detect object-world contacts for normal physics objects.
		for(std::map<int,ObjectData>::const_iterator it=m_objects.begin(), iend=m_objects.end(); it!=iend; ++it)
		{
			NormalPhysicsObject_Ptr object = boost::dynamic_pointer_cast<NormalPhysicsObject,PhysicsObject>(it->second.m_object);
			if(!object) continue;
			boost::optional<Contact> contact = narrowDetector.object_vs_world(*object);
			if(contact) contacts.push_back(Contact_CPtr(new Contact(*contact)));
		}
	}
}

/**
Resolve the contacts using the appropriate contact resolver for each pair of colliding objects.

@param contacts	The array of contacts which need resolving
@param tree		The tree representing the world for collision purposes
*/
void PhysicsSystem::resolve_contacts(const std::vector<Contact_CPtr>& contacts, const OnionTree_CPtr& tree)
{
	// Step 1:	Sort the contacts in ascending order of occurrence.
	std::vector<Contact_CPtr> sortedContacts(contacts);
	std::sort(sortedContacts.begin(), sortedContacts.end(), ContactPred());

	// Step 2:	Resolve each contact in turn. Note that resolving one contact may affect others,
	//			which can't be handled completely without abandoning the sequential resolution
	//			approach. What we do here is simply to recalculate the penetration depth of
	//			each contact as we come to it, thus allowing e.g. contacts which have become
	//			irrelevant as a result of another contact being resolved to be skipped.
	const double PENETRATION_TOLERANCE = 0;
	for(std::vector<Contact_CPtr>::const_iterator it=sortedContacts.begin(), iend=sortedContacts.end(); it!=iend; ++it)
	{
		const Contact& contact = **it;
		double penetrationDepth = contact.penetration_depth();
		if(penetrationDepth < PENETRATION_TOLERANCE) continue;

		// Look up the appropriate contact resolver in the registry, based on the materials
		// of the objects involved.
		PhysicsMaterial materialA = contact.objectA().material();
		PhysicsMaterial materialB = contact.objectB() ? contact.objectB()->material() : PM_WORLD;
		ContactResolver_CPtr resolver = m_contactResolverRegistry.lookup_resolver(materialA, materialB);

		// Resolve the contact using this resolver (if any).
		if(resolver) resolver->resolve_contact(contact, tree);
	}
}

/**
Accumulate the forces on each physics object and update them appropriately.

@param milliseconds	The length of the time step (in milliseconds)
*/
void PhysicsSystem::simulate_objects(int milliseconds)
{
	for(std::map<int,ObjectData>::const_iterator it=m_objects.begin(), iend=m_objects.end(); it!=iend; ++it)
	{
		PhysicsObject& object = *it->second.m_object;

		object.clear_accumulated_force();

		// Apply all the necessary forces to the object.
		typedef ForceGeneratorRegistry::ForceGenerators ForceGenerators;
		const ForceGenerators& generators = m_forceGeneratorRegistry.generators(object.id());
		for(ForceGenerators::const_iterator jt=generators.begin(), jend=generators.end(); jt!=jend; ++jt)
		{
			jt->second->update_force(object);
		}

		object.update(milliseconds);
	}
}

}
