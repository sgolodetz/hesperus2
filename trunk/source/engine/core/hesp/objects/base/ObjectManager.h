/***
 * hesperus: ObjectManager.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_OBJECTMANAGER
#define H_HESP_OBJECTMANAGER

#include <map>
#include <queue>
#include <string>
#include <vector>

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <ASXEngine.h>

#include <hesp/datastructures/PriorityQueue.h>
#include <hesp/util/IDAllocator.h>
#include "ComponentPropertyTypeMap.h"
#include "ListenerTable.h"
#include "ObjectID.h"
#include "ObjectSpecification.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<const class BoundsManager> BoundsManager_CPtr;
typedef shared_ptr<class IObjectComponent> IObjectComponent_Ptr;
typedef shared_ptr<const class Message> Message_CPtr;
typedef shared_ptr<class ModelManager> ModelManager_Ptr;
typedef shared_ptr<const class ModelManager> ModelManager_CPtr;
typedef shared_ptr<class PhysicsSystem> PhysicsSystem_Ptr;
typedef shared_ptr<class SpriteManager> SpriteManager_Ptr;
typedef shared_ptr<const class SpriteManager> SpriteManager_CPtr;

class ObjectManager
{
	//#################### TYPEDEFS ####################
public:
	typedef boost::function<bool (const ObjectID&,const ObjectManager*)> GroupPredicate;
private:
	typedef std::queue<ObjectSpecification> ConstructionQueue;
	typedef PriorityQueue<ObjectID,int,bool,std::greater<int> > DestructionQueue;
	typedef std::map<std::string,IObjectComponent_Ptr> Object;

	//#################### PRIVATE VARIABLES ####################
private:
	ASXEngine_Ptr m_aiEngine;
	std::map<std::string,ObjectSpecification> m_archetypes;
	BoundsManager_CPtr m_boundsManager;
	ComponentPropertyTypeMap m_componentPropertyTypes;
	std::map<std::string,GroupPredicate> m_groupPredicates;
	IDAllocator m_idAllocator;
	ModelManager_Ptr m_modelManager;
	std::map<ObjectID,Object> m_objects;
	PhysicsSystem_Ptr m_physicsSystem;
	SpriteManager_Ptr m_spriteManager;

	ConstructionQueue m_constructionQueue;
	DestructionQueue m_destructionQueue;
	ListenerTable m_listenerTable;

	//#################### CONSTRUCTORS ####################
public:
	ObjectManager(const BoundsManager_CPtr& boundsManager, const ComponentPropertyTypeMap& componentPropertyTypes, const std::map<std::string,ObjectSpecification>& archetypes, const ModelManager_Ptr& modelManager, const SpriteManager_Ptr& spriteManager);

	//#################### PUBLIC METHODS ####################
public:
	void add_listener(IObjectComponent *listener, const ObjectID& id);
	const ASXEngine_Ptr& ai_engine();
	const BoundsManager_CPtr& bounds_manager() const;
	void broadcast_message(const Message_CPtr& msg);
	const ComponentPropertyTypeMap& component_property_types() const;
	void consolidate_object_ids();
	void flush_queues();
	const ObjectSpecification& get_archetype(const std::string& archetypeName) const;
	template <typename T> shared_ptr<T> get_component(const ObjectID& id, const shared_ptr<T>& = shared_ptr<T>());
	template <typename T> shared_ptr<const T> get_component(const ObjectID& id, const shared_ptr<const T>& = shared_ptr<const T>()) const;
	std::vector<IObjectComponent_Ptr> get_components(const ObjectID& id);
	std::vector<ObjectID> group(const std::string& name) const;
	const ModelManager_Ptr& model_manager();
	ModelManager_CPtr model_manager() const;
	int object_count() const;
	const PhysicsSystem_Ptr& physics_system();
	ObjectID player() const;
	void post_message(const ObjectID& target, const Message_CPtr& msg);
	void queue_child_for_destruction(const ObjectID& child, const ObjectID& parent);
	void queue_for_construction(const ObjectSpecification& specification);
	void queue_for_destruction(const ObjectID& id);
	void register_group(const std::string& name, const GroupPredicate& pred);
	void remove_listener(IObjectComponent *listener, const ObjectID& id);
	const SpriteManager_Ptr& sprite_manager();
	SpriteManager_CPtr sprite_manager() const;

	//#################### PRIVATE METHODS ####################
private:
	ObjectID create_object(const ObjectSpecification& specification);
	void destroy_object(const ObjectID& id);
	void flush_construction_queue();
	void flush_destruction_queue();
	template <typename T> shared_ptr<T> get_component(const ObjectID& id, const std::string& group);
	template <typename T> shared_ptr<const T> get_component(const ObjectID& id, const std::string& group) const;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ObjectManager> ObjectManager_Ptr;
typedef shared_ptr<const ObjectManager> ObjectManager_CPtr;

}

#include "ObjectManager.tpp"

#endif
