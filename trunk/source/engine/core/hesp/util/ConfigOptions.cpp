/***
 * hesperus: ConfigOptions.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ConfigOptions.h"

namespace hesp {

//#################### SINGLETON IMPLEMENTATION ####################
ConfigOptions::ConfigOptions() {}

ConfigOptions& ConfigOptions::instance()
{
	static ConfigOptions s_instance;
	return s_instance;
}

//#################### PUBLIC METHODS ####################
bool ConfigOptions::has(const std::string& name) const
{
	return m_properties.has(name);
}

}
