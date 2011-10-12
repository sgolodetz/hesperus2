/***
 * hesperus: Properties.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Properties.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
bool Properties::has(const std::string& name) const
{
	return m_properties.find(name) != m_properties.end();
}

}
