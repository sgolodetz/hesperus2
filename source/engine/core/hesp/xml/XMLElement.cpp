/***
 * hesperus: XMLElement.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "XMLElement.h"

#include <hesp/exceptions/Exception.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
XMLElement::XMLElement(const std::string& name)
:	m_name(name)
{}

//#################### PUBLIC METHODS ####################
void XMLElement::add_child(const XMLElement_Ptr& child)
{
	m_children[child->name()].push_back(child);
}

const std::string& XMLElement::attribute(const std::string& name) const
{
	std::map<std::string,std::string>::const_iterator it = m_attributes.find(name);
	if(it != m_attributes.end()) return it->second;
	else throw Exception("The element does not have an attribute named " + name);
}

std::vector<XMLElement_CPtr> XMLElement::find_children(const std::string& name) const
{
	ChildMap::const_iterator it = m_children.find(name);
	if(it != m_children.end()) return it->second;
	else return std::vector<XMLElement_CPtr>();
}

XMLElement_CPtr XMLElement::find_unique_child(const std::string& name) const
{
	ChildMap::const_iterator it = m_children.find(name);
	if(it != m_children.end())
	{
		const std::vector<XMLElement_CPtr>& children = it->second;
		if(children.size() == 1) return children[0];
		else throw Exception("The element has more than one child named " + name);
	}
	else throw Exception("The element has no child named " + name);
}

bool XMLElement::has_attribute(const std::string& name) const
{
	return m_attributes.find(name) != m_attributes.end();
}

bool XMLElement::has_child(const std::string& name) const
{
	return m_children.find(name) != m_children.end();
}

const std::string& XMLElement::name() const
{
	return m_name;
}

void XMLElement::set_attribute(const std::string& name, const std::string& value)
{
	m_attributes[name] = value;
}

}
