/***
 * hesperus: CmpOrientation.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMPORIENTATION
#define H_HESP_CMPORIENTATION

#include <hesp/math/vectors/Vector3.h>
#include "ICmpOrientation.h"

namespace hesp {

class CmpOrientation : public ICmpOrientation
{
	//#################### PRIVATE VARIABLES ####################
private:
	NUVAxes_Ptr m_nuvAxes;

	//#################### CONSTRUCTORS ####################
public:
	explicit CmpOrientation(const Vector3d& look);

	//#################### STATIC FACTORY METHODS ####################
public:
	static IObjectComponent_Ptr load(const Properties& properties);

	//#################### PUBLIC METHODS ####################
public:
	NUVAxes_Ptr nuv_axes();
	NUVAxes_CPtr nuv_axes() const;
	Properties save() const;
};

}

#endif
