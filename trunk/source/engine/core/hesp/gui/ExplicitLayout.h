/***
 * hesperus: ExplicitLayout.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_EXPLICITLAYOUT
#define H_HESP_EXPLICITLAYOUT

#include <vector>

#include "LaidOutGUIComponent.h"

namespace hesp {

/**
This class allows a GUIContainer to be laid out explicitly by specifying
explicit extents for each component added.
*/
class ExplicitLayout
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::vector<LaidOutGUIComponent> m_components;

	//#################### PUBLIC METHODS ####################
public:
	void add(GUIComponent *component, const Extents& extents);
	void add(const GUIComponent_Ptr& component, const Extents& extents);
	void clear();
	std::vector<LaidOutGUIComponent> fit(const Extents& extents) const;
};

}

#endif
