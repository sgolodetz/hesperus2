/***
 * hesperus: ICmpInventory.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPINVENTORY
#define H_HESP_ICMPINVENTORY

#include <hesp/level/objects/base/ObjectComponent.h>

namespace hesp {

/**
Objects with an ICmpInventory component can own other objects.
*/
class ICmpInventory : public ObjectComponent
{
	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual ObjectID active_item() const = 0;
	virtual void add_consumables(const std::string& type, int amount) = 0;
	virtual void add_item(const ObjectID& objectID) = 0;
	virtual int consumables_count(const std::string& type) const = 0;
	virtual void destroy_consumables(const std::string& type, int amount) = 0;
	virtual void remove_item(const ObjectID& objectID) = 0;

	//#################### PUBLIC METHODS ####################
public:
	std::string group_type() const			{ return "Inventory"; }
	static std::string static_group_type()	{ return "Inventory"; }

	std::string own_type() const			{ return "Inventory"; }
	static std::string static_own_type()	{ return "Inventory"; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICmpInventory> ICmpInventory_Ptr;
typedef shared_ptr<const ICmpInventory> ICmpInventory_CPtr;

}

#endif
