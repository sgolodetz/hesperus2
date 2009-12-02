/***
 * hesperus: DefinitionsFile.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_DEFINITIONSFILE
#define H_HESP_DEFINITIONSFILE

#include <map>
#include <string>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class Bounds> Bounds_Ptr;
typedef shared_ptr<class BoundsManager> BoundsManager_Ptr;
class ComponentPropertyTypeMap;
class ObjectSpecification;
typedef shared_ptr<const class XMLElement> XMLElement_CPtr;

class DefinitionsFile
{
	//#################### LOADING METHODS ####################
public:
	static void load(const std::string& filename, BoundsManager_Ptr& boundsManager, ComponentPropertyTypeMap& componentPropertyTypes, std::map<std::string,ObjectSpecification>& archetypes);
	static BoundsManager_Ptr load_bounds_only(const std::string& filename);

	//#################### LOADING SUPPORT METHODS ####################
private:
	static Bounds_Ptr load_aabb_bounds(const XMLElement_CPtr& elt);
	static void load_archetypes(const XMLElement_CPtr& archetypesElt, std::map<std::string,ObjectSpecification>& archetypes, const ComponentPropertyTypeMap& componentPropertyTypes);
	static BoundsManager_Ptr load_bounds(const XMLElement_CPtr& boundsElt);
	static void load_component_property_types(const XMLElement_CPtr& componentsElt, ComponentPropertyTypeMap& componentPropertyTypes);
	static Bounds_Ptr load_point_bounds(const XMLElement_CPtr&);
	static Bounds_Ptr load_sphere_bounds(const XMLElement_CPtr& elt);
	static void replace_xml_specials(std::string& s);
};

}

#endif
