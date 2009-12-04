/***
 * hesperus: GameState_Level.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "GameState_Level.h"

#include <algorithm>
#include <iostream>
#include <list>

#include <SDL.h>

#include <hesp/axes/NUVAxes.h>
#include <hesp/cameras/FirstPersonCamera.h>
#include <hesp/cameras/FixedCamera.h>
#include <hesp/gui/ExplicitLayout.h>
#include <hesp/gui/Picture.h>
#include <hesp/gui/Screen.h>
#include <hesp/input/InputState.h>
#include <hesp/io/files/LevelFile.h>
#include <hesp/io/util/DirectoryFinder.h>
#include <hesp/math/geom/GeomUtil.h>
#include <hesp/level/HUDViewer.h>
#include <hesp/level/LevelViewer.h>
#include <hesp/level/bounds/Bounds.h>
#include <hesp/level/bounds/BoundsManager.h>
#include <hesp/level/objects/base/ObjectCommand.h>
#include <hesp/level/objects/components/ICmpActivatable.h>
#include <hesp/level/objects/components/ICmpModelRender.h>
#include <hesp/level/objects/components/ICmpMovement.h>
#include <hesp/level/objects/components/ICmpOrientation.h>
#include <hesp/level/objects/components/ICmpSimulation.h>
#include <hesp/level/objects/components/ICmpYoke.h>
#include <hesp/level/objects/messages/MsgTimeElapsed.h>
#include <hesp/level/physics/PhysicsSystem.h>
namespace bf = boost::filesystem;

namespace hesp {

//#################### CONSTRUCTORS ####################
GameState_Level::GameState_Level(const std::string& levelFilename)
:	m_inputGrabbed(false)
{
	m_level = LevelFile::load(levelFilename);
}

//#################### PUBLIC METHODS ####################
void GameState_Level::enter()
{
	// Clear all pending SDL events before we get started.
	{ SDL_Event e; while(SDL_PollEvent(&e)) {} }

	set_display(construct_display());
	grab_input();
}

void GameState_Level::leave()
{
	ungrab_input();
}

GameState_Ptr GameState_Level::update(int milliseconds, InputState& input)
{
	if(input.key_down(SDLK_ESCAPE))
	{
		set_quit_flag();
		return GameState_Ptr();
	}

	// TEMPORARY: Allow quick toggling of the input grab using the 'g' key (for debugging purposes).
	if(input.key_down(SDLK_g))
	{
		input.release_key(SDLK_g);
		if(m_inputGrabbed) ungrab_input();
		else grab_input();
	}

	do_yokes(milliseconds, input);
	do_physics(milliseconds);
	do_animations(milliseconds);
	do_activatables(input);

	// Safely create any new objects which were spawned during this update,
	// and destroy any objects which were queued up for destruction.
	m_level->object_manager()->flush_queues();

	// Broadcast an elapsed time message so that time-sensitive components can update themselves.
	m_level->object_manager()->broadcast_message(Message_CPtr(new MsgTimeElapsed(milliseconds)));

	return GameState_Ptr();
}

//#################### PRIVATE METHODS ####################
GUIComponent_Ptr GameState_Level::construct_display()
{
	GUIContainer<ExplicitLayout> *display = new GUIContainer<ExplicitLayout>;

	bf::path imagesDir = DirectoryFinder::instance().determine_images_directory();

	const Screen& screen = Screen::instance();
	int width = screen.dimensions().width();
	int height = screen.dimensions().height();
	display->layout().add(new Picture((imagesDir / "title.png").file_string()), Extents(width/4, 0, width*3/4, width/8));

	Extents mainExtents(50, width/8, width - 50, height - 50);

	Camera_Ptr camera(new FirstPersonCamera(m_level->object_manager()->player(), m_level->object_manager()));
	display->layout().add(new LevelViewer(m_level, camera), mainExtents);

	display->layout().add(new HUDViewer(m_level), mainExtents);

	return GUIComponent_Ptr(display);
}

void GameState_Level::do_activatables(InputState& input)
{
	// Step 1:	Set all activatable objects to be unhighlighted when rendered.
	const ObjectManager_Ptr& objectManager = m_level->object_manager();
	std::vector<ObjectID> activatables = objectManager->group("Activatables");
	for(size_t i=0, size=activatables.size(); i<size; ++i)
	{
		ICmpModelRender_Ptr cmpRender = objectManager->get_component(activatables[i], cmpRender);
		cmpRender->set_highlights(false);
	}

	// Step 2:	Find the half-ray representing the player's line of sight.
	ICmpOrientation_Ptr cmpPlayerOrientation = objectManager->get_component(objectManager->player(), cmpPlayerOrientation);
	ICmpPosition_Ptr cmpPlayerPosition = objectManager->get_component(objectManager->player(), cmpPlayerPosition);
	Vector3d eye = cmpPlayerPosition->position();
	Vector3d look = cmpPlayerOrientation->nuv_axes()->n();

	// Step 3:	Check all activatable objects in leaves in the PVS of the viewer's leaf and find the nearest within range (if any)
	//			whose AABB is intersected by the half-ray. (If there's no such object, return.)
	ObjectID nearestObject;
	double nearestDistSquared = INT_MAX;

	std::vector<int> visibleLeaves = m_level->find_visible_leaves(eye);
	for(size_t i=0, size=activatables.size(); i<size; ++i)
	{
		// TODO: Determine whether the activatable is in a visible leaf and skip further testing if not.

		ICmpSimulation_Ptr cmpSimulation = objectManager->get_component(activatables[i], cmpSimulation);

		const Bounds_CPtr& bounds = objectManager->bounds_manager()->bounds(cmpSimulation->bounds_group(), cmpSimulation->posture());
		const Vector3d& position = cmpSimulation->position();
		Vector3d localEye = eye - position;		// the eye location in local model coordinates
		boost::optional<std::pair<Vector3d,Vector3d> > result = bounds->determine_halfray_intersection(localEye, look);
		if(result)
		{
			Vector3d hit = result->first;		// the location of the hit (in local model coordinates)
			double distSquared = localEye.distance_squared(hit);
			if(distSquared < nearestDistSquared)
			{
				nearestObject = activatables[i];
				nearestDistSquared = distSquared;
			}
		}
	}

	const double RANGE = 3.0;
	if(!nearestObject.valid() || nearestDistSquared > RANGE*RANGE) return;

	// Step 4:	If the player is trying to activate an object, activate the nearest object. Otherwise, set it to be highlighted.
	if(input.mouse_button_down(MOUSE_BUTTON_RIGHT))
	{
		ICmpActivatable_Ptr cmpActivatable = objectManager->get_component(nearestObject, cmpActivatable);
		cmpActivatable->activated_by(objectManager->player());
		input.release_mouse_button(MOUSE_BUTTON_RIGHT);
	}
	else
	{
		ICmpModelRender_Ptr cmpRender = objectManager->get_component(nearestObject, cmpRender);
		cmpRender->set_highlights(true);
	}
}

void GameState_Level::do_animations(int milliseconds)
{
	// Update the model animations.
	const ObjectManager_Ptr& objectManager = m_level->object_manager();
	std::vector<ObjectID> animatables = objectManager->group("Animatables");
	for(size_t i=0, size=animatables.size(); i<size; ++i)
	{
		ICmpModelRender_Ptr cmpRender = objectManager->get_component(animatables[i], cmpRender);
		cmpRender->update_animation(milliseconds, m_level->onion_polygons(), m_level->onion_tree(), m_level->nav_manager());
	}
}

void GameState_Level::do_gravity(int milliseconds)
{
	// Apply gravity to moveable objects.

	// FIXME: Gravity strength should eventually be a level property.
	const double GRAVITY_STRENGTH = 9.81;	// strength of gravity in Newtons

	const ObjectManager_Ptr& objectManager = m_level->object_manager();
	std::vector<ObjectID> moveables = objectManager->group("Moveables");
	for(size_t i=0, size=moveables.size(); i<size; ++i)
	{
		ICmpMovement_Ptr cmpMovement = objectManager->get_component(moveables[i], cmpMovement);
		ICmpSimulation_Ptr cmpSimulation = objectManager->get_component(moveables[i], cmpSimulation);
		Vector3d velocity = cmpSimulation->velocity();
		cmpSimulation->set_velocity(velocity + Vector3d(0,0,-GRAVITY_STRENGTH*(milliseconds/1000.0)));
		if(cmpMovement->single_move(cmpSimulation->velocity(), 7.0 /* FIXME */, milliseconds, m_level->onion_tree()))
		{
			// A collision occurred, so set the velocity back to zero.
			cmpSimulation->set_velocity(Vector3d(0,0,0));
		}
	}
}

void GameState_Level::do_physics(int milliseconds)
{
	// Treat gravity as a special case.
	do_gravity(milliseconds);

	const BoundsManager_CPtr& boundsManager = m_level->object_manager()->bounds_manager();
	OnionTree_CPtr tree = m_level->onion_tree();
	m_level->object_manager()->physics_system()->update(boundsManager, tree, milliseconds);
}

void GameState_Level::do_yokes(int milliseconds, InputState& input)
{
	// Step 1:	Generate the desired object commands for the yokeable objects and add them to the queue.
	const ObjectManager_Ptr& objectManager = m_level->object_manager();
	std::vector<ObjectID> yokeables = objectManager->group("Yokeables");

	std::list<ObjectCommand_Ptr> cmdQueue;	// implement the queue as a list so that we can use back_inserter below

	for(size_t i=0, size=yokeables.size(); i<size; ++i)
	{
		ICmpYoke_Ptr cmpYoke = objectManager->get_component(yokeables[i], cmpYoke);
		
		// Use the object's yoke component to generate object commands.
		std::vector<ObjectCommand_Ptr> commands = cmpYoke->generate_commands(input, m_level->onion_polygons(), m_level->onion_tree(), m_level->nav_manager());

		// Append the object commands to the queue.
		std::copy(commands.begin(), commands.end(), std::back_inserter(cmdQueue));
	}

	// Step 2:	Execute the object commands.
	for(std::list<ObjectCommand_Ptr>::const_iterator it=cmdQueue.begin(), iend=cmdQueue.end(); it!=iend; ++it)
	{
		(*it)->execute(objectManager, m_level->onion_polygons(), m_level->onion_tree(), m_level->nav_manager(), milliseconds);
	}
}

void GameState_Level::grab_input()
{
	SDL_ShowCursor(SDL_DISABLE);
	SDL_WM_GrabInput(SDL_GRAB_ON);
	m_inputGrabbed = true;
}

void GameState_Level::ungrab_input()
{
	SDL_WM_GrabInput(SDL_GRAB_OFF);
	SDL_ShowCursor(SDL_ENABLE);
	m_inputGrabbed = false;
}

}
