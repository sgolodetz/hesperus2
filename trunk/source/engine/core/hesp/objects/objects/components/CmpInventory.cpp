/***
 * hesperus: CmpInventory.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpInventory.h"

#include <set>
#include <vector>

#include <hesp/level/objects/messages/MsgObjectDestroyed.h>
#include <hesp/level/objects/messages/MsgObjectPredestroyed.h>
#include <hesp/util/Properties.h>
#include "ICmpUsable.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpInventory::CmpInventory(const ObjectID& activeItem, const std::map<std::string,int>& consumables, const std::set<ObjectID>& items)
:	m_initialised(false), m_activeItem(activeItem), m_consumables(consumables), m_items(items)
{
	// Note:	We can't update the usable groups at this stage, because the items we're holding generally haven't been created yet.
	//			Instead, they are initialised "on demand" later on.

	// Check that the active item (if any) is actually one of the items in the inventory.
	if(activeItem.valid() && items.find(activeItem) == items.end())
	{
		throw Exception("The active item must be in the inventory");
	}
}

//#################### STATIC FACTORY METHODS ####################
IObjectComponent_Ptr CmpInventory::load(const Properties& properties)
{
	return IObjectComponent_Ptr(new CmpInventory(properties.get<ObjectID>("ActiveItem"),
												 properties.get<std::map<std::string,int> >("Consumables"),
												 properties.get<std::set<ObjectID> >("Items")));
}

//#################### PUBLIC METHODS ####################
ObjectID CmpInventory::active_item() const
{
	initialise_if_necessary();
	return m_activeItem;
}

void CmpInventory::add_consumables(const std::string& type, int amount)
{
	// Check precondition.
	if(amount < 0) throw Exception("Cannot add a negative number of consumables");

	std::map<std::string,int>::iterator it = m_consumables.find(type);
	if(it != m_consumables.end()) it->second += amount;
	else m_consumables.insert(std::make_pair(type, amount));
}

void CmpInventory::add_item(const ObjectID& id)
{
	initialise_if_necessary();

	m_items.insert(id);
	m_objectManager->add_listener(this, id);

	ICmpUsable_Ptr cmpItemUsable = m_objectManager->get_component(id, cmpItemUsable);
	if(cmpItemUsable) m_groups[cmpItemUsable->usable_group()].insert(id);
}

int CmpInventory::consumables_count(const std::string& type) const
{
	std::map<std::string,int>::const_iterator it = m_consumables.find(type);
	if(it != m_consumables.end()) return it->second;
	else return 0;
}

void CmpInventory::destroy_consumables(const std::string& type, int amount)
{
	// Check precondition.
	if(amount < 0) throw Exception("Cannot destroy a negative number of consumables");

	std::map<std::string,int>::iterator it = m_consumables.find(type);
	if(it != m_consumables.end() && it->second >= amount)
	{
		it->second -= amount;
	}
	else throw Exception("Cannot destroy more consumables than are being held");
}

void CmpInventory::process_message(const MsgObjectDestroyed& msg)
{
	if(m_items.find(msg.object_id()) != m_items.end())
	{
		// An item contained in the inventory has been destroyed: we need to remove it from the inventory.
		// Note that the item still exists until after processing of destroyed messages has finished (see
		// the implementation in ObjectManager), so there is no danger of accessing the item after it has
		// already been physically destroyed.
		remove_item(msg.object_id());
	}
}

void CmpInventory::process_message(const MsgObjectPredestroyed& msg)
{
	if(msg.object_id() == m_objectID)
	{
		// The object whose inventory this is is about to be destroyed. All the items in the inventory
		// thus need to be queued up for prior destruction.
		for(std::set<ObjectID>::const_iterator it=m_items.begin(), iend=m_items.end(); it!=iend; ++it)
		{
			m_objectManager->queue_child_for_destruction(*it, m_objectID);
		}
	}
}

void CmpInventory::register_listening()
{
	// Note: The inventory needs to receive predestroy messages for the object to which it belongs.
	m_objectManager->add_listener(this, m_objectID);

	for(std::set<ObjectID>::const_iterator it=m_items.begin(), iend=m_items.end(); it!=iend; ++it)
	{
		m_objectManager->add_listener(this, *it);
	}
}

void CmpInventory::remove_item(const ObjectID& id)
{
	initialise_if_necessary();

	m_items.erase(id);
	m_objectManager->remove_listener(this, id);
	if(m_activeItem == id) m_activeItem = ObjectID();

	ICmpUsable_Ptr cmpItemUsable = m_objectManager->get_component(id, cmpItemUsable);
	if(cmpItemUsable) m_groups[cmpItemUsable->usable_group()].erase(id);
}

Properties CmpInventory::save() const
{
	Properties properties;
	properties.set("ActiveItem", m_activeItem);
	properties.set("Consumables", m_consumables);
	properties.set("Items", m_items);
	return properties;
}

//#################### PRIVATE METHODS ####################
void CmpInventory::initialise_if_necessary() const
{
	if(!m_initialised)
	{
		for(std::set<ObjectID>::const_iterator it=m_items.begin(), iend=m_items.end(); it!=iend; ++it)
		{
			ICmpUsable_Ptr cmpItemUsable = m_objectManager->get_component(*it, cmpItemUsable);
			if(cmpItemUsable) m_groups[cmpItemUsable->usable_group()].insert(*it);
		}

		m_initialised = true;
	}
}

}
