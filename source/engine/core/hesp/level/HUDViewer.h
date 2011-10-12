/***
 * hesperus: HUDViewer.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_HUDVIEWER
#define H_HESP_HUDVIEWER

#include <hesp/gui/GUIComponent.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class Level> Level_Ptr;

class HUDViewer : public GUIComponent
{
	//#################### PRIVATE VARIABLES ####################
private:
	Level_Ptr m_level;

	//#################### CONSTRUCTORS ####################
public:
	explicit HUDViewer(const Level_Ptr& level);

	//#################### PUBLIC METHODS ####################
public:
	void render() const;

	//#################### PRIVATE METHODS ####################
private:
	void render_ammo(int w, int h) const;
	void render_health_bar(int w, int h) const;
};

}

#endif
