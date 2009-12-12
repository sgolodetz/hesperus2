/***
 * hesperus: ObjectManager.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ObjectManager.h"

#include <hesp/level/objects/components/ICmpActivatable.h>
#include <hesp/level/objects/components/ICmpInventory.h>
#include <hesp/level/objects/components/ICmpModelRender.h>
#include <hesp/level/objects/components/ICmpMovement.h>
#include <hesp/level/objects/components/ICmpOwnable.h>
#include <hesp/level/objects/components/ICmpYoke.h>
#include <hesp/level/objects/contactresolvers/AbsorbProjectileContactResolver.h>
#include <hesp/level/objects/contactresolvers/BounceContactResolver.h>
#include <hesp/level/objects/contactresolvers/ProjectileDamageContactResolver.h>
#include <hesp/level/objects/messages/MsgObjectDestroyed.h>
#include <hesp/level/objects/messages/MsgObjectPredestroyed.h>
#include <hesp/level/physics/PhysicsSystem.h>
#include "ObjectSpecification.h"

namespace hesp {

//#################### LOCAL METHODS - DECLARATIONS ####################
bool has_owner(const ObjectID& id, const ObjectManager *objectManager);
bool is_activatable(const ObjectID& id, const ObjectManager *objectManager);
bool is_animatable(const ObjectID& id, const ObjectManager *objectManager);
bool is_moveable(const ObjectID& id, const ObjectManager *objectManager);
bool is_renderable(const ObjectID& id, const ObjectManager *objectManager);
bool is_yokeable(const ObjectID& id, const ObjectManager *objectManager);

//#################### CONSTRUCTORS ####################
ObjectManager::ObjectManager(const BoundsManager_CPtr& boundsManager, const ComponentPropertyTypeMap& componentPropertyTypes,
							 const std::map<std::string,ObjectSpecification>& archetypes, const ASXEngine_Ptr& aiEngine,
							 const ModelManager_Ptr& modelManager, const SpriteManager_Ptr& spriteManager)
:	m_boundsManager(boundsManager),
	m_componentPropertyTypes(componentPropertyTypes),
	m_archetypes(archetypes),
	m_aiEngine(aiEngine),
	m_modelManager(modelManager),
	m_physicsSystem(new PhysicsSystem),
	m_spriteManager(spriteManager)
{
	// Set up the physics system.
	m_physicsSystem->set_contact_resolver(PM_BULLET, PM_CHARACTER, ContactResolver_CPtr(new ProjectileDamageContactResolver(this, PM_BULLET)));
	m_physicsSystem->set_contact_resolver(PM_BULLET, PM_WORLD, ContactResolver_CPtr(new AbsorbProjectileContactResolver(this, PM_BULLET)));
	m_physicsSystem->set_contact_resolver(PM_CHARACTER, PM_CHARACTER, ContactResolver_CPtr(new BounceContactResolver(0.1)));
	m_physicsSystem->set_contact_resolver(PM_CHARACTER, PM_WORLD, ContactResolver_CPtr(new BounceContactResolver(0.0)));
	m_physicsSystem->set_contact_resolver(PM_ITEM, PM_WORLD, ContactResolver_CPtr(new BounceContactResolver(0.0)));

	// Register object groupings.
	register_group("Activatables", is_activatable);
	register_group("Animatables", is_animatable);
	register_group("Moveables", is_moveable);
	register_group("Renderables", is_renderable);
	register_group("Yokeables", is_yokeable);
}

//#################### PUBLIC METHODS ####################
/**
Informs the object manager that the specified component is interested in receiving
broadcasted messsages about the specified object.

@param listener	The component which wants to receive object messages
@param id		The ID of the object about which the component wants to receive messages
*/
void ObjectManager::add_listener(IObjectComponent *listener, const ObjectID& id)
{
	m_listenerTable.add_listener(listener->object_id(), listener->group_type(), id);
}

const ASXEngine_Ptr& ObjectManager::ai_engine()
{
	return m_aiEngine;
}

const BoundsManager_CPtr& ObjectManager::bounds_manager() const
{
	return m_boundsManager;
}

void ObjectManager::broadcast_message(const Message_CPtr& msg)
{
	std::set<ObjectID> msgObjs = msg->referenced_objects();
	if(msgObjs.empty())
	{
		// This isn't a message about particular objects: dispatch it to every component which can receive it.
		for(std::map<ObjectID,Object>::const_iterator it=m_objects.begin(), iend=m_objects.end(); it!=iend; ++it)
		{
			const Object& object = it->second;
			for(Object::const_iterator jt=object.begin(), jend=object.end(); jt!=jend; ++jt)
			{
				IObjectComponent_Ptr component = jt->second;
				msg->dispatch(component.get());
			}
		}
	}
	else
	{
		// This is a message about particular objects: only broadcast it to components which have registered an interest.

		// Calculate the set of interested listeners.
		std::set<std::pair<ObjectID,std::string> > listeners;
		for(std::set<ObjectID>::const_iterator it=msgObjs.begin(), iend=msgObjs.end(); it!=iend; ++it)
		{
			std::vector<std::pair<ObjectID,std::string> > result = m_listenerTable.get_listeners(*it);
			std::copy(result.begin(), result.end(), std::inserter(listeners, listeners.end()));
		}

		// Dispatch the message to each of them in turn.
		for(std::set<std::pair<ObjectID,std::string> >::const_iterator it=listeners.begin(), iend=listeners.end(); it!=iend; ++it)
		{
			IObjectComponent_Ptr listener = get_component<IObjectComponent>(it->first, it->second);
			if(listener) msg->dispatch(listener.get());
		}
	}
}

const ComponentPropertyTypeMap& ObjectManager::component_property_types() const
{
	return m_componentPropertyTypes;
}

void ObjectManager::consolidate_object_ids()
{
	// TODO:	This only needs implementing if I want to save objects in-game. If I only save objects I just
	//			loaded in (e.g. as part of the level-making process), their IDs will already be contiguous.
}

void ObjectManager::flush_queues()
{
	// Note:	The destruction queue must be flushed second, since some of the
	//			newly-created objects may refer to objects which are about to be
	//			destroyed (and they need to be warned of this).
	flush_construction_queue();
	flush_destruction_queue();
}

const ObjectSpecification& ObjectManager::get_archetype(const std::string& archetypeName) const
{
	std::map<std::string,ObjectSpecification>::const_iterator it = m_archetypes.find(archetypeName);
	if(it != m_archetypes.end()) return it->second;
	else throw Exception("No such archetype: " + archetypeName);
}

std::vector<IObjectComponent_Ptr> ObjectManager::get_components(const ObjectID& id)
{
	std::map<ObjectID,Object>::iterator it = m_objects.find(id);
	if(it == m_objects.end()) throw Exception("Invalid object ID: " + id.to_string());

	Object& object = it->second;

	std::vector<IObjectComponent_Ptr> components;
	for(Object::const_iterator jt=object.begin(), jend=object.end(); jt!=jend; ++jt)
	{
		components.push_back(jt->second);
	}
	return components;
}

std::vector<ObjectID> ObjectManager::group(const std::string& name) const
{
	std::vector<ObjectID> ret;

	std::map<std::string,GroupPredicate>::const_iterator gt = m_groupPredicates.find(name);
	if(gt == m_groupPredicates.end()) throw Exception("No such object group: " + name);
	const GroupPredicate& pred = gt->second;

	for(std::map<ObjectID,Object>::const_iterator it=m_objects.begin(), iend=m_objects.end(); it!=iend; ++it)
	{
		const ObjectID& objectID = it->first;
		if(pred(objectID, this)) ret.push_back(objectID);
	}

	return ret;
}

const ModelManager_Ptr& ObjectManager::model_manager()
{
	return m_modelManager;
}

ModelManager_CPtr ObjectManager::model_manager() const
{
	return m_modelManager;
}

int ObjectManager::object_count() const
{
	return static_cast<int>(m_objects.size());
}

const PhysicsSystem_Ptr& ObjectManager::physics_system()
{
	return m_physicsSystem;
}

ObjectID ObjectManager::player() const
{
	// FIXME: The object ID of the player should be loaded in, not hard-coded like this.
	return ObjectID(0);
}

void ObjectManager::post_message(const ObjectID& target, const Message_CPtr& msg)
{
	std::map<ObjectID,Object>::iterator it = m_objects.find(target);
	if(it == m_objects.end()) throw Exception("Invalid object ID: " + target.to_string());

	Object& obj = it->second;
	for(Object::iterator jt=obj.begin(), jend=obj.end(); jt!=jend; ++jt)
	{
		msg->dispatch(jt->second.get());
	}
}

void ObjectManager::queue_child_for_destruction(const ObjectID& child, const ObjectID& parent)
{
	int parentPriority = m_destructionQueue.element(parent).key();
	m_destructionQueue.insert(child, parentPriority+1, false);
}

void ObjectManager::queue_for_construction(const ObjectSpecification& specification)
{
	m_constructionQueue.push(specification);
}

void ObjectManager::queue_for_destruction(const ObjectID& id)
{
	m_destructionQueue.insert(id, 0, false);
}

void ObjectManager::register_group(const std::string& name, const GroupPredicate& pred)
{
	m_groupPredicates[name] = pred;
}

/**
Informs the object manager that the specified component is no longer interested
in receiving broadcasted messsages about the specified object.

@param listener	The component which no longer wants to receive object messages
@param id		The ID of the object about which the component no longer wants to receive messages
*/
void ObjectManager::remove_listener(IObjectComponent *listener, const ObjectID& id)
{
	m_listenerTable.remove_listener(listener->object_id(), listener->group_type(), id);
}

const SpriteManager_Ptr& ObjectManager::sprite_manager()
{
	return m_spriteManager;
}

SpriteManager_CPtr ObjectManager::sprite_manager() const
{
	return m_spriteManager;
}

//#################### PRIVATE METHODS ####################
ObjectID ObjectManager::create_object(const ObjectSpecification& specification)
{
	std::vector<IObjectComponent_Ptr> components = specification.instantiate_components();

	ObjectID id(m_idAllocator.allocate());
	Object& object = m_objects[id];

	// Add the components to the object manager.
	for(size_t i=0, size=components.size(); i<size; ++i)
	{
		components[i]->set_object_id(id);
		components[i]->set_object_manager(this);
		object.insert(std::make_pair(components[i]->group_type(), components[i]));
	}

	// Check component dependencies and register listening (note that these must happen
	// after all the components have been added above).
	for(size_t i=0, size=components.size(); i<size; ++i)
	{
		components[i]->check_dependencies();
		components[i]->register_listening();
	}

	return id;
}

void ObjectManager::destroy_object(const ObjectID& id)
{
	// Remove all the listeners which are components of the object being deleted.
	m_listenerTable.remove_listeners_from(id);

	broadcast_message(Message_CPtr(new MsgObjectDestroyed(id)));

	// Remove all the listeners referring to the object.
	m_listenerTable.remove_listeners_to(id);

	m_objects.erase(id);
	m_idAllocator.deallocate(id.value());
}

void ObjectManager::flush_construction_queue()
{
	while(!m_constructionQueue.empty())
	{
		create_object(m_constructionQueue.front());
		m_constructionQueue.pop();
	}
}

void ObjectManager::flush_destruction_queue()
{
	typedef DestructionQueue::Element Elt;

	DestructionQueue& q = m_destructionQueue;

	while(!q.empty())
	{
		Elt& e = q.top();
		ObjectID id = e.id();
		bool& predestroyFlag = e.data();

		if(predestroyFlag)
		{
			// The pre-destroy message has already been sent for this object.
			destroy_object(id);
			q.pop();
		}
		else
		{
			// Note: The flag must be set before sending the pre-destroy message, which may result in queue changes.
			predestroyFlag = true;
			broadcast_message(Message_CPtr(new MsgObjectPredestroyed(id)));
		}
	}
}

//#################### LOCAL METHODS - DEFINITIONS ####################
bool has_owner(const ObjectID& id, const ObjectManager *objectManager)
{
	ICmpOwnable_CPtr cmpOwnable = objectManager->get_component(id, cmpOwnable);
	return cmpOwnable && cmpOwnable->owner().valid();
}

bool is_activatable(const ObjectID& id, const ObjectManager *objectManager)
{
	return !has_owner(id, objectManager) && objectManager->get_component<ICmpActivatable>(id) != NULL;
}

bool is_animatable(const ObjectID& id, const ObjectManager *objectManager)
{
	if(objectManager->get_component<ICmpModelRender>(id))
	{
		ICmpOwnable_CPtr cmpOwnable = objectManager->get_component(id, cmpOwnable);

		// Objects with a ModelRender component are animatable provided they are either not ownable,
		// or not currently owned.
		return !cmpOwnable || !cmpOwnable->owner().valid();
	}
	else return false;	// animatable objects must have a ModelRender component
}

bool is_moveable(const ObjectID& id, const ObjectManager *objectManager)
{
	return !has_owner(id, objectManager) && objectManager->get_component<ICmpMovement>(id) != NULL;
}

bool is_renderable(const ObjectID& id, const ObjectManager *objectManager)
{
	return !has_owner(id, objectManager) && objectManager->get_component<ICmpRender>(id) != NULL;
}

bool is_yokeable(const ObjectID& id, const ObjectManager *objectManager)
{
	return !has_owner(id, objectManager) && objectManager->get_component<ICmpYoke>(id) != NULL;
}

}
