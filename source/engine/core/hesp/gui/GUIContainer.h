/***
 * hesperus: GUIContainer.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_GUICONTAINER
#define H_HESP_GUICONTAINER

#include <vector>

#include "GUIComponent.h"
#include "LaidOutGUIComponent.h"

namespace hesp {

template <typename Layout>
class GUIContainer : public GUIComponent
{
	//#################### PRIVATE VARIABLES ####################
private:
	Layout m_layout;
	std::vector<LaidOutGUIComponent> m_components;

	//#################### PUBLIC METHODS ####################
public:
	void fit(const Extents& extents, GUIComponent *parent);
	void handle_input(InputState& input);
	Layout& layout();
	void render() const;
};

}

#include "GUIContainer.tpp"

#endif
