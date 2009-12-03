/***
 * hesperus: ResourceManager.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <hesp/exceptions/Exception.h>

namespace hesp {

//#################### PUBLIC METHODS ####################
/**
Loads all the registered resources.
*/
template <typename Resource>
void ResourceManager<Resource>::load_all()
{
	for(std::map<std::string,shared_ptr<Resource> >::iterator it=m_resources.begin(), iend=m_resources.end(); it!=iend; ++it)
	{
		const std::string& name = it->first;
		shared_ptr<Resource>& resource = it->second;
		if(!resource) resource = load_resource(name);
	}
}

/**
Registers a resource for subsequent loading.

@param resourceName	The name of the resource
*/
template <typename Resource>
void ResourceManager<Resource>::register_resource(const std::string& resourceName)
{
	m_resources.insert(std::make_pair(resourceName, shared_ptr<Resource>()));
}

/**
Returns the resource with the specified name, if any.

@param resourceName	The name of the resource
@return				The resource, if it exists
@throw Exception	If the resource doesn't exist
*/
template <typename Resource>
const shared_ptr<Resource>& ResourceManager<Resource>::resource(const std::string& resourceName)
{
	std::map<std::string,shared_ptr<Resource> >::const_iterator it = m_resources.find(resourceName);
	if(it != m_resources.end())
	{
		return it->second;
	}
	else
	{
		std::string resourceType = resource_type();
		throw Exception("This " + resourceType + " manager does not contain a resource named " + resourceName);
	}
}

/**
The const version of the above.
*/
template <typename Resource>
shared_ptr<const Resource> ResourceManager<Resource>::resource(const std::string& resourceName) const
{
	std::map<std::string,shared_ptr<Resource> >::const_iterator it = m_resources.find(resourceName);
	if(it != m_resources.end())
	{
		return it->second;
	}
	else
	{
		std::string resourceType = resource_type();
		throw Exception("This " + resourceType + " manager does not contain a resource named " + resourceName);
	}
}

/**
Returns the names of the registered resources.

@return	The registered resource names
*/
template <typename Resource>
std::set<std::string> ResourceManager<Resource>::resource_names() const
{
	std::set<std::string> ret;
	for(std::map<std::string,shared_ptr<Resource> >::const_iterator it=m_resources.begin(), iend=m_resources.end(); it!=iend; ++it)
	{
		ret.insert(it->first);
	}
	return ret;
}

}
