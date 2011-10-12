/***
 * hesperus: VerticallyConstrainedNUVAxes.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_VERTICALLYCONSTRAINEDNUVAXES
#define H_HESP_VERTICALLYCONSTRAINEDNUVAXES

#include "NUVAxes.h"

namespace hesp {

class VerticallyConstrainedNUVAxes : public NUVAxes
{
	//#################### CONSTRUCTORS ####################
public:
	explicit VerticallyConstrainedNUVAxes(const Vector3d& n);

	//#################### PUBLIC METHODS ####################
public:
	void rotate(const Vector3d& axis, double angle);
	void set_n(const Vector3d& n);
};

}

#endif
