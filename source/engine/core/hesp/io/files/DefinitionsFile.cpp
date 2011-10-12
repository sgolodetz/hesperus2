/***
 * hesperus: DefinitionsFile.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "DefinitionsFile.h"

#include <boost/algorithm/string/replace.hpp>
#include <boost/dynamic_bitset.hpp>
#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <hesp/bounds/AABBBounds.h>
#include <hesp/bounds/BoundsManager.h>
#include <hesp/bounds/PointBounds.h>
#include <hesp/bounds/SphereBounds.h>
#include <hesp/exceptions/Exception.h>
#include <hesp/io/util/PropertyIO.h>
#include <hesp/objects/base/ComponentPropertyTypeMap.h>
#include <hesp/objects/base/ObjectSpecification.h>
#include <hesp/xml/XMLParser.h>

namespace hesp {

//#################### LOADING METHODS ####################
/**
Loads the specified definitions file.

@param filename					The name of the file
@param boundsManager			Used to return the various bounds for objects
@param componentPropertyTypes	Used to return the types of the properties in the various object components
@param archetypes				Used to return the object archetypes
*/
void DefinitionsFile::load(const std::string& filename, BoundsManager_Ptr& boundsManager, ComponentPropertyTypeMap& componentPropertyTypes,
						   std::map<std::string,ObjectSpecification>& archetypes)
{
	XMLLexer_Ptr lexer(new XMLLexer(filename));
	XMLParser parser(lexer);
	XMLElement_CPtr root = parser.parse();
	XMLElement_CPtr definitionsElt = root->find_unique_child("definitions");

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Process the objects subtree.
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	{
		XMLElement_CPtr objectsElt = definitionsElt->find_unique_child("objects");

		// Load the object bounds.
		XMLElement_CPtr boundsElt = objectsElt->find_unique_child("bounds");
		boundsManager = load_bounds(boundsElt);

		// Load the component property types.
		XMLElement_CPtr componentsElt = objectsElt->find_unique_child("components");
		load_component_property_types(componentsElt, componentPropertyTypes);

		// Load the object archetypes.
		XMLElement_CPtr archetypesElt = objectsElt->find_unique_child("archetypes");
		load_archetypes(archetypesElt, archetypes, componentPropertyTypes);
	}
}

/**
Loads the bounds from the specified definitions file.

@param filename	The name of the file from which to load the bounds
@return			The bounds
*/
BoundsManager_Ptr DefinitionsFile::load_bounds_only(const std::string& filename)
{
	XMLLexer_Ptr lexer(new XMLLexer(filename));
	XMLParser parser(lexer);
	XMLElement_CPtr root = parser.parse();
	XMLElement_CPtr definitionsElt = root->find_unique_child("definitions");
	XMLElement_CPtr objectsElt = definitionsElt->find_unique_child("objects");
	XMLElement_CPtr boundsElt = objectsElt->find_unique_child("bounds");
	return load_bounds(boundsElt);
}

//#################### LOADING SUPPORT METHODS ####################
Bounds_Ptr DefinitionsFile::load_aabb_bounds(const XMLElement_CPtr& elt)
{
	double sx = lexical_cast<double>(elt->attribute("sx"));
	double sy = lexical_cast<double>(elt->attribute("sy"));
	double sz = lexical_cast<double>(elt->attribute("sz"));
	return Bounds_Ptr(new AABBBounds(Vector3d(sx,sy,sz)));
}

void DefinitionsFile::load_archetypes(const XMLElement_CPtr& archetypesElt, std::map<std::string,ObjectSpecification>& archetypes,
									  const ComponentPropertyTypeMap& componentPropertyTypes)
{
	std::vector<XMLElement_CPtr> archetypeElts = archetypesElt->find_children("archetype");
	for(size_t i=0, sizeI=archetypeElts.size(); i<sizeI; ++i)
	{
		const XMLElement_CPtr& archetypeElt = archetypeElts[i];
		const std::string& archetypeName = archetypeElt->attribute("name");
		ObjectSpecification specification;

		std::vector<XMLElement_CPtr> componentElts = archetypeElt->find_children("component");
		for(size_t j=0, sizeJ=componentElts.size(); j<sizeJ; ++j)
		{
			const XMLElement_CPtr& componentElt = componentElts[j];
			const std::string& componentName = componentElt->attribute("name");
			Properties properties;

			std::vector<XMLElement_CPtr> propertyElts = componentElt->find_children("property");
			for(size_t k=0, sizeK=propertyElts.size(); k<sizeK; ++k)
			{
				const XMLElement_CPtr& propertyElt = propertyElts[k];
				const std::string& propertyName = propertyElt->attribute("name");

				std::string value;
				if(propertyElt->has_attribute("default")) value = propertyElt->attribute("default");
				else if(propertyElt->has_attribute("fixed")) value = propertyElt->attribute("fixed");
				else continue;

				// Process the value to replace XML special characters with the actual characters they specify.
				replace_xml_specials(value);

				// Look up the property's type.
				std::string type = componentPropertyTypes.lookup(componentName, propertyName);

				// Parse the value and store the property.
				PropertyIO::load_property(properties, propertyName, type, value);
			}

			specification.add_component(componentName, properties);
		}

		archetypes.insert(std::make_pair(archetypeName, specification));
	}
}

BoundsManager_Ptr DefinitionsFile::load_bounds(const XMLElement_CPtr& boundsElt)
{
	typedef BoundsManager::BoundsGroup BoundsGroup;
	std::vector<Bounds_CPtr> bounds;
	std::map<std::string,BoundsGroup> boundsGroups;
	std::map<std::string,int> boundsLookup;
	boost::dynamic_bitset<> boundsNavFlags;

	// Load the bounds.
	typedef Bounds_Ptr(*BoundsLoader)(const XMLElement_CPtr&);
	typedef std::map<std::string,BoundsLoader> BoundsLoaders;
	BoundsLoaders boundsLoaders;
	boundsLoaders["aabb"]	= load_aabb_bounds;
	boundsLoaders["point"]	= load_point_bounds;
	boundsLoaders["sphere"]	= load_sphere_bounds;

	std::vector<XMLElement_CPtr> boundElts = boundsElt->find_children("bound");
	for(size_t i=0, size=boundElts.size(); i<size; ++i)
	{
		const XMLElement_CPtr& boundElt = boundElts[i];
		const std::string& boundName = boundElt->attribute("name");
		const std::string& boundType = boundElt->attribute("type");
		bool boundNavFlag = boundElt->attribute("nav") == "true";

		BoundsLoaders::const_iterator jt = boundsLoaders.find(boundType);
		if(jt != boundsLoaders.end())
		{
			BoundsLoader loader = jt->second;
			boundsLookup.insert(std::make_pair(boundName, static_cast<int>(bounds.size())));
			bounds.push_back(loader(boundElt));
			boundsNavFlags.push_back(boundNavFlag);
		}
		else throw Exception("No such bounds type: " + boundType);
	}

	// Load the groups.
	std::vector<XMLElement_CPtr> groupElts = boundsElt->find_children("group");
	for(size_t i=0, groupCount=groupElts.size(); i<groupCount; ++i)
	{
		const XMLElement_CPtr& groupElt = groupElts[i];
		const std::string& groupName = groupElt->attribute("name");

		std::vector<XMLElement_CPtr> postureElts = groupElt->find_children("posture");
		for(size_t j=0, postureCount=postureElts.size(); j<postureCount; ++j)
		{
			const XMLElement_CPtr& postureElt = postureElts[j];
			const std::string& postureName = postureElt->attribute("name");
			const std::string& postureBound = postureElt->attribute("bound");
			boundsGroups[groupName].insert(std::make_pair(postureName, postureBound));
		}
	}

	return BoundsManager_Ptr(new BoundsManager(bounds, boundsGroups, boundsLookup, boundsNavFlags));
}

void DefinitionsFile::load_component_property_types(const XMLElement_CPtr& componentsElt, ComponentPropertyTypeMap& componentPropertyTypes)
{
	std::vector<XMLElement_CPtr> componentElts = componentsElt->find_children("component");

	for(size_t i=0, componentCount=componentElts.size(); i<componentCount; ++i)
	{
		const XMLElement_CPtr& componentElt = componentElts[i];
		const std::string& componentName = componentElt->attribute("name");
		componentPropertyTypes.add_component(componentName);

		// Load the property types for component i.
		std::vector<XMLElement_CPtr> propertyElts = componentElt->find_children("property");
		for(size_t j=0, propertyCount=propertyElts.size(); j<propertyCount; ++j)
		{
			const XMLElement_CPtr& propertyElt = propertyElts[j];
			const std::string& name = propertyElt->attribute("name");
			std::string type = propertyElt->attribute("type");
			componentPropertyTypes.set_property_type(componentName, name, type);
		}
	}
}

Bounds_Ptr DefinitionsFile::load_point_bounds(const XMLElement_CPtr&)
{
	return Bounds_Ptr(new PointBounds);
}

Bounds_Ptr DefinitionsFile::load_sphere_bounds(const XMLElement_CPtr& elt)
{
	double radius = lexical_cast<double>(elt->attribute("radius"));
	return Bounds_Ptr(new SphereBounds(radius));
}

void DefinitionsFile::replace_xml_specials(std::string& s)
{
	boost::replace_all(s, "&lt;", "<");
	boost::replace_all(s, "&gt;", ">");
}

}
