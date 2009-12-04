/***
 * hesperus: LevelViewer.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVELVIEWER
#define H_HESP_LEVELVIEWER

#include <hesp/gui/GUIComponent.h>
#include "Level.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class Camera> Camera_Ptr;

class LevelViewer : public GUIComponent
{
	//#################### TYPEDEFS ####################
private:
	typedef std::vector<Portal_Ptr> PortalVector;

	//#################### PRIVATE VARIABLES ####################
private:
	Level_Ptr m_level;
	Camera_Ptr m_camera;

	//#################### CONSTRUCTORS ####################
public:
	LevelViewer(const Level_Ptr& level, const Camera_Ptr& camera);

	//#################### PUBLIC METHODS ####################
public:
	void render() const;

	//#################### PRIVATE METHODS ####################
private:
	void render_level() const;
	void render_navlinks() const;
	void render_navmeshes() const;
	void render_objects() const;
	void render_portals() const;
};

}

#endif
