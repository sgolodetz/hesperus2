/***
 * hesperus: HUDViewer.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "HUDViewer.h"

#include <hesp/ogl/WrappedGL.h>
#include <GL/glu.h>

#include <boost/lexical_cast.hpp>
using boost::lexical_cast;

#include <hesp/gui/Screen.h>
#include <hesp/level/objects/components/ICmpAmmoNeedingUsable.h>
#include <hesp/level/objects/components/ICmpHealth.h>
#include <hesp/level/objects/components/ICmpInventory.h>
#include <hesp/util/TextRenderer.h>
#include "Level.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
HUDViewer::HUDViewer(const Level_Ptr& level)
:	m_level(level)
{}

//#################### PUBLIC METHODS ####################
void HUDViewer::render() const
{
	Screen::instance().set_ortho_viewport(*m_extents);
	const int& x1 = m_extents->left(), y1 = m_extents->top(), x2 = m_extents->right(), y2 = m_extents->bottom();
	const int w = x2 - x1, h = y2 - y1;
	render_ammo(w, h);
	render_health_bar(w, h);
}

//#################### PRIVATE METHODS ####################
void HUDViewer::render_ammo(int w, int h) const
try
{
	// Get the player's ammo count for the current weapon (if any).
	ObjectID player = m_level->object_manager()->player();
	ICmpInventory_CPtr cmpInventory = m_level->object_manager()->get_component(player, cmpInventory);
	if(!cmpInventory) return;
	ObjectID activeItem = cmpInventory->active_item();
	if(!activeItem.valid()) return;
	ICmpAmmoNeedingUsable_CPtr cmpItemUsable = m_level->object_manager()->get_component(activeItem, cmpItemUsable);
	if(!cmpItemUsable) return;
	int ammoCount = cmpInventory->consumables_count(cmpItemUsable->ammo_type());

	// Render it in the bottom-left of the screen.
	TextRenderer textRenderer("Impact", 14);
	std::string ammoStr = "Ammo: " + lexical_cast<std::string>(ammoCount);
	textRenderer.write_aligned(ammoStr, 20, h-20, TextRenderer::HALIGN_LEFT, TextRenderer::VALIGN_BOTTOM, Colour3d(1,1,1));
}
catch(Exception&) {}

void HUDViewer::render_health_bar(int w, int h) const
{
	int left = 20, right = left + 2*w/5, top = 20, bottom = top + h/20;

	// Get the player's health and max health.
	ObjectID player = m_level->object_manager()->player();
	ICmpHealth_CPtr cmpHealth = m_level->object_manager()->get_component(player, cmpHealth);
	if(!cmpHealth) return;
	double health = cmpHealth->health();
	double maxHealth = cmpHealth->max_health();

	// Render the actual health bar.
	double t = health / maxHealth;
	double healthRight = (1-t) * left + t * right;
	glColor3d(1,0,0);
	glBegin(GL_QUADS);
		glVertex2d(left,top);
		glVertex2d(healthRight,top);
		glVertex2d(healthRight,bottom);
		glVertex2d(left,bottom);
	glEnd();

	// Render the box around the health bar.
	glColor3d(1,1,1);
	glBegin(GL_LINE_LOOP);
		glVertex2i(left,top);
		glVertex2i(right,top);
		glVertex2i(right,bottom);
		glVertex2i(left,bottom);
	glEnd();
}

}
