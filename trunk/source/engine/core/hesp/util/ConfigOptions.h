/***
 * hesperus: ConfigOptions.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CONFIGOPTIONS
#define H_HESP_CONFIGOPTIONS

#include "Properties.h"

namespace hesp {

class ConfigOptions
{
	//#################### SINGLETON IMPLEMENTATION ####################
private:
	ConfigOptions();
	ConfigOptions(const ConfigOptions&);
	ConfigOptions& operator=(const ConfigOptions&);
public:
	static ConfigOptions& instance();

	//#################### PRIVATE VARIABLES ####################
private:
	Properties m_properties;

	//#################### PUBLIC METHODS ####################
public:
	template <typename T> const T& get(const std::string& name) const;
	bool has(const std::string& name) const;
	template <typename T> void set(const std::string& name, const T& value);
};

}

#include "ConfigOptions.tpp"

#endif
