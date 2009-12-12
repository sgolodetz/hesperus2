/***
 * hesperus: ObjectSpecification.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

namespace hesp {

//#################### PUBLIC METHODS ####################
template <typename T>
void ObjectSpecification::set_component_property(const std::string& groupName, const std::string& propertyName, const T& value)
{
	ComponentMap::iterator it = m_components.find(groupName);
	if(it != m_components.end())
	{
		Properties& properties = it->second.second;
		properties.set(propertyName, value);
	}
	else throw Exception("The object specification doesn't contain a component in group: " + groupName);
}

}
