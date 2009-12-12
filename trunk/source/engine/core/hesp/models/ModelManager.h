/***
 * hesperus: ModelManager.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MODELMANAGER
#define H_HESP_MODELMANAGER

#include <hesp/util/ResourceManager.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class Model> Model_Ptr;
typedef shared_ptr<const class Model> Model_CPtr;

class ModelManager : public ResourceManager<Model>
{
	//#################### PUBLIC METHODS ####################
public:
	const Model_Ptr& model(const std::string& modelName);
	Model_CPtr model(const std::string& modelName) const;
	std::set<std::string> model_names() const;
	void register_model(const std::string& modelName);

	//#################### PRIVATE METHODS ####################
private:
	Model_Ptr load_resource(const std::string& modelName) const;
	std::string resource_type() const;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ModelManager> ModelManager_Ptr;
typedef shared_ptr<const ModelManager> ModelManager_CPtr;

}

#endif
