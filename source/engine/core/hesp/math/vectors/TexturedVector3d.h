/***
 * hesperus: TexturedVector3d.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_TEXTUREDVECTOR3D
#define H_HESP_TEXTUREDVECTOR3D

#include "Vector3.h"

namespace hesp {

/**
This struct represents 3D vectors which can also store (u,v) texture coordinates.
*/
struct TexturedVector3d
{
	//#################### PUBLIC VARIABLES ####################
	double x, y, z, u, v;

	//#################### CONSTRUCTORS ####################
	TexturedVector3d();
	TexturedVector3d(double x_, double y_, double z_, double u_, double v_);
	TexturedVector3d(const std::vector<std::string>& components);

	//#################### PUBLIC OPERATORS ####################
	TexturedVector3d& operator+=(const TexturedVector3d& rhs);
	TexturedVector3d& operator-=(const TexturedVector3d& rhs);
	TexturedVector3d& operator*=(double factor);
	operator Vector3d() const;
};

//#################### GLOBAL OPERATORS ####################
TexturedVector3d operator+(const TexturedVector3d& lhs, const TexturedVector3d& rhs);
TexturedVector3d operator-(const TexturedVector3d& lhs, const TexturedVector3d& rhs);
TexturedVector3d operator*(double factor, const TexturedVector3d& v);
std::ostream& operator<<(std::ostream& os, const TexturedVector3d& v);

}

#endif
