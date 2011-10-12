/***
 * hesperus: ObjectsSection.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ObjectsSection.h"

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/tuple/tuple.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <hesp/io/util/LineIO.h>
#include <hesp/io/util/PropertyIO.h>
#include <hesp/objects/base/IObjectComponent.h>
#include <hesp/objects/base/ObjectManager.h>
#include <hesp/util/Properties.h>

namespace hesp {

//#################### LOADING METHODS ####################
ObjectManager_Ptr ObjectsSection::load(std::istream& is, const BoundsManager_CPtr& boundsManager,
									   const ComponentPropertyTypeMap& componentPropertyTypes,
									   const std::map<std::string,ObjectSpecification>& archetypes,
									   const ModelManager_Ptr& modelManager, const SpriteManager_Ptr& spriteManager,
									   const Database_Ptr& database)
{
	ObjectManager_Ptr objectManager(new ObjectManager(boundsManager, componentPropertyTypes, archetypes, modelManager, spriteManager, database));

	LineIO::read_checked_line(is, "Objects");
	LineIO::read_checked_line(is, "{");

	int objectCount = FieldIO::read_typed_trimmed_field<int>(is, "Count");
	for(int i=0; i<objectCount; ++i)
	{
		ObjectSpecification specification = load_object_specification(is, componentPropertyTypes);
		objectManager->queue_for_construction(specification);
	}

	// Create all the objects whose specifications we just added to the construction queue.
	objectManager->flush_queues();

	LineIO::read_checked_line(is, "}");

	return objectManager;
}

//#################### SAVING METHODS ####################
void ObjectsSection::save(std::ostream& os, const ObjectManager_Ptr& objectManager)
{
	objectManager->consolidate_object_ids();

	os << "Objects\n";
	os << "{\n";

	os << '\t';
	int objectCount = objectManager->object_count();
	FieldIO::write_typed_field(os, "Count", objectCount);

	// Note:	The objects with the specified IDs are guaranteed to exist because we called consolidate_object_ids() above.
	//			This would not necessarily be the case otherwise, as the IDs of the objects may not form a contiguous series.
	for(int i=0; i<objectCount; ++i)
	{
		std::vector<IObjectComponent_Ptr> components = objectManager->get_components(ObjectID(i));
		save_object(os, components, objectManager->component_property_types());
	}

	os << "}\n";
}

//#################### LOADING SUPPORT METHODS ####################
ObjectSpecification ObjectsSection::load_object_specification(std::istream& is, const ComponentPropertyTypeMap& componentPropertyTypes)
{
	ObjectSpecification specification;

	LineIO::read_checked_trimmed_line(is, "Object");
	LineIO::read_checked_trimmed_line(is, "{");

	std::string line;
	for(;;)
	{
		LineIO::read_trimmed_line(is, line, "object component name");
		if(line == "}") break;
		if(line.length() == 0) continue;

		bool hasFields = line[line.length()-1] != ';';
		std::string componentName = hasFields ? line : line.substr(0, line.length()-1);

		Properties properties;
		if(hasFields)
		{
			LineIO::read_checked_trimmed_line(is, "{");
			for(;;)
			{
				LineIO::read_trimmed_line(is, line, "object component property");
				if(line == "}") break;

				// Parse the field.
				std::string name, value;
				boost::tie(name, value) = FieldIO::parse_field(line);

				// Lookup the type of the property.
				std::string type = componentPropertyTypes.lookup(componentName, name);

				// Convert the value to the correct type and add it to the properties map.
				PropertyIO::load_property(properties, name, type, value);
			}
		}

		specification.add_component(componentName, properties);
	}

	return specification;
}

//#################### SAVING SUPPORT METHODS ####################
void ObjectsSection::save_object(std::ostream& os, const std::vector<IObjectComponent_Ptr>& components,
								 const ComponentPropertyTypeMap& componentPropertyTypes)
{
	os << "\tObject\n";
	os << "\t{\n";

	for(size_t i=0, size=components.size(); i<size; ++i)
	{
		const IObjectComponent_Ptr& component = components[i];
		std::string componentName = component->own_type();
		Properties properties = component->save();

		os << "\t\t" << componentName;

		const std::map<std::string,std::string>& types = componentPropertyTypes.property_types(componentName);

		if(types.empty())
		{
			os << ";\n";
		}
		else
		{
			os << "\n\t\t{\n";
			for(std::map<std::string,std::string>::const_iterator kt=types.begin(), kend=types.end(); kt!=kend; ++kt)
			{
				const std::string& name = kt->first;
				const std::string& type = kt->second;

				os << "\t\t\t";

				PropertyIO::save_property(os, properties, name, type);
			}
			os << "\t\t}\n";
		}
	}

	os << "\t}\n";
}

}
