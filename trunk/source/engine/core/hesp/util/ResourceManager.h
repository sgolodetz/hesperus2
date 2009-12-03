/***
 * hesperus: ResourceManager.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_RESOURCEMANAGER
#define H_HESP_RESOURCEMANAGER

#include <map>
#include <set>
#include <string>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

template <typename Resource>
class ResourceManager
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::map<std::string,shared_ptr<Resource> > m_resources;

	//#################### PRIVATE ABSTRACT METHODS ####################
private:
	virtual shared_ptr<Resource> load_resource(const std::string& resourceName) const = 0;
	virtual std::string resource_type() const = 0;

	//#################### PUBLIC METHODS ####################
public:
	void load_all();
	void register_resource(const std::string& resourceName);
	const shared_ptr<Resource>& resource(const std::string& resourceName);
	shared_ptr<const Resource> resource(const std::string& resourceName) const;
	std::set<std::string> resource_names() const;
};

}

#include "ResourceManager.tpp"

#endif
