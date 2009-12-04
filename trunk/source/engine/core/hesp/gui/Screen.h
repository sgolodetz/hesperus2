/***
 * hesperus: Screen.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_SCREEN
#define H_HESP_SCREEN

#include "ExplicitLayout.h"
#include "GUIContainer.h"

namespace hesp {

class Screen : public GUIContainer<ExplicitLayout>
{
	//#################### SINGLETON IMPLEMENTATION ####################
private:
	Screen();
	Screen(const Screen&);
	Screen& operator=(const Screen&);
public:
	static Screen& instance();

	//#################### PRIVATE VARIABLES ####################
private:
	Extents_Ptr m_dimensions;

	//#################### PUBLIC METHODS ####################
public:
	const Extents& dimensions() const;
	void fit();
	void render() const;
	void set_dimensions(const Extents& dimensions);
	void set_ortho_viewport(const Extents& extents);
	void set_persp_viewport(const Extents& extents, double fovY, double zNear, double zFar);
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Screen> Screen_Ptr;

}

#endif
