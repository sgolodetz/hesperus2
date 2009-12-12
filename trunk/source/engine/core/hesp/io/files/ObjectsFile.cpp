/***
 * hesperus: ObjectsFile.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ObjectsFile.h"

#include <fstream>

#include <hesp/exceptions/Exception.h>
#include <hesp/io/sections/ModelNamesSection.h>
#include <hesp/io/sections/ObjectsSection.h>
#include <hesp/io/sections/SpriteNamesSection.h>
#include <hesp/objects/base/ObjectManager.h>

namespace hesp {

//#################### LOADING METHODS ####################
ObjectManager_Ptr ObjectsFile::load(const std::string& filename, const BoundsManager_CPtr& boundsManager,
									const ComponentPropertyTypeMap& componentPropertyTypes,
									const std::map<std::string,ObjectSpecification>& archetypes)
{
	std::ifstream is(filename.c_str(), std::ios_base::binary);
	if(is.fail()) throw Exception("Could not open " + filename + " for reading");
	ModelManager_Ptr modelManager = ModelNamesSection().load(is);
	SpriteManager_Ptr spriteManager = SpriteNamesSection().load(is);
	return ObjectsSection::load(is, boundsManager, componentPropertyTypes, archetypes, modelManager, spriteManager);
}

//#################### SAVING METHODS ####################
void ObjectsFile::save(const std::string& filename, const ObjectManager_Ptr& objectManager)
{
	std::ofstream os(filename.c_str(), std::ios_base::binary);
	if(os.fail()) throw Exception("Could not open " + filename + " for writing");
	ModelNamesSection().save(os, objectManager->model_manager());
	SpriteNamesSection().save(os, objectManager->sprite_manager());
	ObjectsSection::save(os, objectManager);
}

}
