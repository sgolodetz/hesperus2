/***
 * hesperus: LaidOutGUIComponent.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_LAIDOUTGUICOMPONENT
#define H_HESP_LAIDOUTGUICOMPONENT

#include "Extents.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class GUIComponent> GUIComponent_Ptr;

struct LaidOutGUIComponent
{
	//#################### PUBLIC VARIABLES ####################
	GUIComponent_Ptr component;
	Extents extents;

	//#################### CONSTRUCTORS ####################
	LaidOutGUIComponent(const GUIComponent_Ptr& component_, const Extents& extents_);
};

}

#endif
