/***
 * TexturedLitVector3d.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_TEXTUREDLITVECTOR3D
#define H_HESP_TEXTUREDLITVECTOR3D

#include <iosfwd>

#include "Vector3.h"

namespace hesp {

/**
This struct represents 3D vectors which can also store (u,v) texture coordinates
and (lu,lv) lightmap coordinates.
*/
struct TexturedLitVector3d
{
	//#################### PUBLIC VARIABLES ####################
	double x, y, z, u, v, lu, lv;

	//#################### CONSTRUCTORS ####################
	TexturedLitVector3d(double x_, double y_, double z_, double u_, double v_, double lu_, double lv_);
	TexturedLitVector3d(const std::vector<std::string>& components);

	//#################### PUBLIC OPERATORS ####################
	operator Vector3d() const;
};

//#################### GLOBAL OPERATORS ####################
std::ostream& operator<<(std::ostream& os, const TexturedLitVector3d& v);

}

#endif
