/***
 * hesperus: MinimusScriptYoke.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "MinimusScriptYoke.h"

#include <iostream>

#include <hesp/io/util/DirectoryFinder.h>
#include "MinimusGotoPositionYoke.h"
namespace bf = boost::filesystem;

namespace hesp {

//#################### CONSTRUCTORS ####################
MinimusScriptYoke::MinimusScriptYoke(const ObjectID& objectID, ObjectManager *objectManager, const std::string& scriptName, const ASXEngine_Ptr& engine)
:	m_objectID(objectID), m_objectManager(objectManager), m_engine(engine), m_initialised(false)
{
	bf::path scriptsDir = DirectoryFinder::instance().determine_scripts_directory();
	std::string scriptFilename = (scriptsDir / (scriptName + ".as")).file_string();

	m_engine->clear_messages();
	if(!m_engine->load_and_build_script(scriptFilename, scriptName))
	{
		m_engine->output_messages(std::cout);
		throw Exception("Could not build script module " + scriptName);
	}

	m_module = m_engine->get_module(scriptName);
}

//#################### PUBLIC METHODS ####################
void MinimusScriptYoke::add_ref()
{
	++m_refCount;
}

std::vector<ObjectCommand_Ptr> MinimusScriptYoke::generate_commands(InputState& input, const std::vector<CollisionPolygon_Ptr>& polygons,
																	const OnionTree_CPtr& tree, const NavManager_CPtr& navManager)
{
	if(!m_initialised)
	{
		// Run the script init method.
		ASXFunction<void(MinimusScriptYoke*)> init = m_module->get_global_function("init", init);
		init(this);

		m_initialised = true;
	}

	// Run the script process method.
	ASXFunction<void(MinimusScriptYoke*)> process = m_module->get_global_function("process", process);
	process(this);

	if(m_subyoke && m_subyoke->state() == YOKE_ACTIVE)
	{
		return m_subyoke->generate_commands(input, polygons, tree, navManager);
	}
	else return std::vector<ObjectCommand_Ptr>();
}

void MinimusScriptYoke::register_for_scripting(const ASXEngine_Ptr& engine)
{
	engine->register_uninstantiable_ref_type<MinimusScriptYoke>();
	engine->register_object_method(&clear_subyoke, "clear_subyoke");
	engine->register_object_method(&goto_position, "goto_position");
	engine->register_object_method(&subyoke_active, "subyoke_active");
	engine->register_object_method(&subyoke_exists, "subyoke_exists");
}

void MinimusScriptYoke::release()
{
	if(--m_refCount == 0) delete this;
}

std::string MinimusScriptYoke::type_string()
{
	// Note: This is the name that will be used to refer to MinimusScriptYoke within scripts.
	return "ScriptYoke";
}

//#################### SCRIPT INTERFACE ####################
void MinimusScriptYoke::clear_subyoke()
{
	m_subyoke.reset();
}

void MinimusScriptYoke::goto_position(double x, double y, double z)
{
	m_subyoke.reset(new MinimusGotoPositionYoke(m_objectID, m_objectManager, Vector3d(x,y,z)));
}

bool MinimusScriptYoke::subyoke_active() const
{
	return subyoke_exists() && m_subyoke->state() == YOKE_ACTIVE;
}

bool MinimusScriptYoke::subyoke_exists() const
{
	return m_subyoke != NULL;
}

}
