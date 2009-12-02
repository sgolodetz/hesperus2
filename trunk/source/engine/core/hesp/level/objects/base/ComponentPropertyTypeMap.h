/***
 * hesperus: ComponentPropertyTypeMap.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_COMPONENTPROPERTYTYPEMAP
#define H_HESP_COMPONENTPROPERTYTYPEMAP

#include <map>
#include <string>

namespace hesp {

class ComponentPropertyTypeMap
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::map<std::string,std::map<std::string,std::string> > m_componentPropertyTypes;

	//#################### PUBLIC METHODS ####################
public:
	void add_component(const std::string& componentName);
	const std::string& lookup(const std::string& componentName, const std::string& propertyName) const;
	const std::map<std::string,std::string>& property_types(const std::string& componentName) const;
	void set_property_type(const std::string& componentName, const std::string& propertyName, const std::string& type);
};

}

#endif
