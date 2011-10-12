/***
 * hesperus: GUIComponent.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "GUIComponent.h"

#include <hesp/ogl/WrappedGL.h>
#include "Extents.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
GUIComponent::GUIComponent()
:	m_parent(NULL)
{}

//#################### DESTRUCTOR ####################
GUIComponent::~GUIComponent() {}

//#################### PUBLIC METHODS ####################
const Extents& GUIComponent::extents() const
{
	return *m_extents;
}

void GUIComponent::fit(const Extents& extents, GUIComponent *parent)
{
	m_extents = Extents_Ptr(new Extents(extents));
	m_parent = parent;
}

void GUIComponent::handle_input(InputState& input)
{
	// Note:	A default implementation is provided so that components which don't
	//			need to handle input don't have to.
}

//#################### PROTECTED METHODS ####################
void GUIComponent::render_extents() const
{
	// Draw a border round the component on the screen (using the current colour).
	const int& x1 = m_extents->left(), y1 = m_extents->top(), x2 = m_extents->right(), y2 = m_extents->bottom();
	const int w = x2 - x1, h = y2 - y1;
	glBegin(GL_LINE_LOOP);
		glVertex2i(0,0);
		glVertex2i(w,0);
		glVertex2i(w,h);
		glVertex2i(0,h);
	glEnd();
}

}
