/***
 * hesperus: Light.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_LIGHT
#define H_HESP_LIGHT

#include <hesp/colours/Colour3d.h>
#include <hesp/math/vectors/Vector3.h>

namespace hesp {

/**
This struct represents point light sources.
*/
struct Light
{
	//#################### PUBLIC VARIABLES ####################
	Colour3d colour;
	double falloffRadius;
	Vector3d position;

	//#################### CONSTRUCTORS ####################
public:
	Light(const Vector3d& position_, const Colour3d& colour_, double falloffRadius_)
	:	position(position_), colour(colour_), falloffRadius(falloffRadius_)
	{}
};

}

#endif
