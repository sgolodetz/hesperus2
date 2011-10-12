/***
 * hesperus: GUIContainer.tpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#define GUIContainer_HEADER		template <typename Layout>
#define GUIContainer_THIS		GUIContainer<Layout>

namespace hesp {

//#################### PUBLIC METHODS ####################
GUIContainer_HEADER
void GUIContainer_THIS::fit(const Extents& extents, GUIComponent *parent)
{
	GUIComponent::fit(extents, parent);

	m_components = m_layout.fit(extents);
	for(std::vector<LaidOutGUIComponent>::iterator it=m_components.begin(), iend=m_components.end(); it!=iend; ++it)
	{
		it->component->fit(it->extents, this);
	}
}

GUIContainer_HEADER
void GUIContainer_THIS::handle_input(InputState& input)
{
	for(std::vector<LaidOutGUIComponent>::const_iterator it=m_components.begin(), iend=m_components.end(); it!=iend; ++it)
	{
		it->component->handle_input(input);
	}
}

GUIContainer_HEADER
Layout& GUIContainer_THIS::layout()
{
	return m_layout;
}

GUIContainer_HEADER
void GUIContainer_THIS::render() const
{
	for(std::vector<LaidOutGUIComponent>::const_iterator it=m_components.begin(), iend=m_components.end(); it!=iend; ++it)
	{
		it->component->render();
	}
}

}

#undef GUIContainer_THIS
#undef GUIContainer_HEADER
