/***
 * hesperus: CmpItemActivatable.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpItemActivatable.h"

#include <hesp/util/Properties.h>
#include "ICmpInventory.h"
#include "ICmpOwnable.h"

namespace hesp {

//#################### STATIC FACTORY METHODS ####################
IObjectComponent_Ptr CmpItemActivatable::load(const Properties&)
{
	return IObjectComponent_Ptr(new CmpItemActivatable);
}

//#################### PUBLIC METHODS ####################
void CmpItemActivatable::activated_by(const ObjectID& activator)
{
	ICmpInventory_Ptr cmpActivatorInventory = m_objectManager->get_component(activator, cmpActivatorInventory);

	// If the activating object (e.g. the player) has no inventory, the item can't be picked up.
	if(!cmpActivatorInventory) return;

	ICmpOwnable_Ptr cmpItemOwnable = m_objectManager->get_component(m_objectID, cmpItemOwnable);

	// Defensively check that the item has no current owner.
	if(cmpItemOwnable->owner().valid()) throw Exception("Cannot pick up an item which is already owned by another object");

	cmpItemOwnable->set_owner(activator);
	cmpActivatorInventory->add_item(m_objectID);
}

void CmpItemActivatable::check_dependencies() const
{
	check_dependency<ICmpOwnable>();
}

Properties CmpItemActivatable::save() const
{
	return Properties();
}

}
