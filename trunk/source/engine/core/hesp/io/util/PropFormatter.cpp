/***
 * hesperus: PropFormatter.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "PropFormatter.h"

#include <hesp/exceptions/Exception.h>

namespace hesp {

//#################### SPECIALIZATIONS ####################
std::string PropFormatter<ObjectID>::format(const ObjectID& input)
{
	return PropFormatter<int>::format(input.value());
}

std::string PropFormatter<PhysicsMaterial>::format(PhysicsMaterial input)
{
	switch(input)
	{
	case PM_BULLET:		return "\"bullet\"";
	case PM_CHARACTER:	return "\"character\"";
	case PM_ITEM:		return "\"item\"";
	case PM_WORLD:		return "\"world\"";
	default:			throw Exception("Forgot to handle one of the physics material cases");
	}
}

std::string PropFormatter<std::string>::format(const std::string& input)
{
	return "\"" + input + "\"";
}

}
