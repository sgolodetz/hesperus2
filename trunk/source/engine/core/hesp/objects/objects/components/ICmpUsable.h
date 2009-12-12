/***
 * hesperus: ICmpUsable.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPUSABLE
#define H_HESP_ICMPUSABLE

#include <boost/optional.hpp>

#include <hesp/level/objects/base/ObjectComponent.h>
#include <hesp/math/vectors/Vector3.h>

namespace hesp {

/**
Objects with an ICmpUsable component can be used.
*/
class ICmpUsable : public ObjectComponent
{
	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	/**
	Returns the usable's hotspots (e.g. the muzzle opening of the pistol for creating projectiles, etc.).
	Each hotspot is a string referring to a bone in the usable's skeleton.
	*/
	virtual const std::vector<std::string>& hotspots() const = 0;

	/**
	Sets the orientation of the specified hotspot.
	*/
	virtual void set_hotspot_orientation(const std::string& name, const Vector3d& orientation) = 0;

	/**
	Sets the position of the specified hotspot.
	*/
	virtual void set_hotspot_position(const std::string& name, const Vector3d& position) = 0;

	/**
	Returns the usable's group (e.g. sword, pistol, tool, etc.)
	*/
	virtual std::string usable_group() const = 0;

	/**
	Uses the usable (e.g. fires the weapon, uses the lockpick, etc.)
	*/
	virtual void use() = 0;

	//#################### PUBLIC METHODS ####################
public:
	std::string group_type() const			{ return "Usable"; }
	static std::string static_group_type()	{ return "Usable"; }

	std::string own_type() const			{ return "Usable"; }
	static std::string static_own_type()	{ return "Usable"; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICmpUsable> ICmpUsable_Ptr;
typedef shared_ptr<const ICmpUsable> ICmpUsable_CPtr;

}

#endif
