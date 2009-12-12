/***
 * hesperus: ObjectSpecification.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_OBJECTSPECIFICATION
#define H_HESP_OBJECTSPECIFICATION

#include <map>
#include <string>
#include <utility>
#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <hesp/util/Properties.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class IObjectComponent> IObjectComponent_Ptr;

class ObjectSpecification
{
	//#################### TYPEDEFS ####################
private:
	typedef IObjectComponent_Ptr (*ComponentLoader)(const Properties&);
	typedef std::map<std::string,std::pair<std::string,Properties> > ComponentMap;

	//#################### PRIVATE VARIABLES ####################
private:
	ComponentMap m_components;

	static bool s_mapsBuilt;
	static std::map<std::string,ComponentLoader> s_componentLoaders;
	static std::map<std::string,std::string> s_groupNames;

	//#################### PUBLIC METHODS ####################
public:
	void add_component(const std::string& componentName, const Properties& properties);
	std::vector<IObjectComponent_Ptr> instantiate_components() const;
	template <typename T> void set_component_property(const std::string& groupName, const std::string& propertyName, const T& value);

	//#################### LOOKUP METHODS ####################
private:
	static void build_maps();
	static std::map<std::string,ComponentLoader>& component_loaders();
	static std::map<std::string,std::string>& group_names();
	static IObjectComponent_Ptr invoke_component_loader(const std::string& componentName, const Properties& properties);
	static std::string lookup_group(const std::string& componentName);
};

}

#include "ObjectSpecification.tpp"

#endif
