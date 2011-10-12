/***
 * hesperus: ObjectsFile.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_OBJECTSFILE
#define H_HESP_OBJECTSFILE

#include <map>
#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<const class BoundsManager> BoundsManager_CPtr;
class ComponentPropertyTypeMap;
typedef shared_ptr<class ObjectManager> ObjectManager_Ptr;
class ObjectSpecification;

struct ObjectsFile
{
	//#################### LOADING METHODS ####################
	static ObjectManager_Ptr load(const std::string& filename, const BoundsManager_CPtr& boundsManager, const ComponentPropertyTypeMap& componentPropertyTypes, const std::map<std::string,ObjectSpecification>& archetypes);

	//#################### SAVING METHODS ####################
	static void save(const std::string& filename, const ObjectManager_Ptr& objectManager);
};

}

#endif
