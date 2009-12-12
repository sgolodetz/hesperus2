/***
 * hesperus: MinimusScriptYoke.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MINIMUSSCRIPTYOKE
#define H_HESP_MINIMUSSCRIPTYOKE

#include <ASXEngine.h>
#include <ASXRefType.h>

#include <hesp/level/objects/base/IYoke.h>
#include <hesp/level/objects/base/ObjectID.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
class ObjectManager;

class MinimusScriptYoke : public IYoke, public ASXRefType<MinimusScriptYoke>
{
	//#################### PRIVATE VARIABLES ####################
private:
	ObjectID m_objectID;
	ObjectManager *m_objectManager;

	ASXEngine_Ptr m_engine;
	ASXModule_Ptr m_module;
	bool m_initialised;

	IYoke_Ptr m_subyoke;

	//#################### CONSTRUCTORS ####################
public:
	MinimusScriptYoke(const ObjectID& objectID, ObjectManager *objectManager, const std::string& scriptName, const ASXEngine_Ptr& engine);

	//#################### PUBLIC METHODS ####################
public:
	void add_ref();
	std::vector<ObjectCommand_Ptr> generate_commands(InputState& input, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_CPtr& tree, const NavManager_CPtr& navManager);
	static void register_for_scripting(const ASXEngine_Ptr& engine);
	void release();
	static std::string type_string();

	//#################### SCRIPT INTERFACE ####################
private:
	void clear_subyoke();
	void goto_position(double x, double y, double z);
	bool subyoke_active() const;
	bool subyoke_exists() const;
};

}

#endif
