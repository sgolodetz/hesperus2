/***
 * hesperus: CmpInventory.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMPINVENTORY
#define H_HESP_CMPINVENTORY

#include <set>

#include "ICmpInventory.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
class MsgObjectDestroyed;
class MsgObjectPredestroyed;

class CmpInventory
:	public ICmpInventory,
	public MessageHandler<MsgObjectDestroyed>,
	public MessageHandler<MsgObjectPredestroyed>
{
	//#################### PRIVATE VARIABLES ####################
private:
	mutable bool m_initialised;

	ObjectID m_activeItem;											// the active item (e.g. the pistol in the entity's hand)
	std::map<std::string,int> m_consumables;						// the amounts of various types of consumable (e.g. pistol bullets) currently being held
	std::set<ObjectID> m_items;

	mutable std::map<std::string,std::set<ObjectID> > m_groups;		// which usables of each group (e.g. pistols) are currently being held

	//#################### CONSTRUCTORS ####################
public:
	CmpInventory(const ObjectID& activeItem, const std::map<std::string,int>& consumables, const std::set<ObjectID>& items);

	//#################### STATIC FACTORY METHODS ####################
public:
	static IObjectComponent_Ptr load(const Properties& properties);

	//#################### PUBLIC METHODS ####################
public:
	ObjectID active_item() const;
	void add_consumables(const std::string& type, int amount);
	void add_item(const ObjectID& id);
	int consumables_count(const std::string& type) const;
	void destroy_consumables(const std::string& type, int amount);
	void process_message(const MsgObjectDestroyed& msg);
	void process_message(const MsgObjectPredestroyed& msg);
	void register_listening();
	void remove_item(const ObjectID& id);
	Properties save() const;

	//#################### PRIVATE METHODS ####################
private:
	void initialise_if_necessary() const;
};

}

#endif
