/***
 * hesperus: NUVAxes.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_NUVAXES
#define H_HESP_NUVAXES

#include <hesp/math/vectors/Vector3.h>

namespace hesp {

class NUVAxes
{
	//#################### PROTECTED VARIABLES ####################
protected:
	Vector3d m_n, m_u, m_v;

	//#################### CONSTRUCTORS ####################
public:
	NUVAxes(const Vector3d& n, const Vector3d& up);

	//#################### DESTRUCTOR ####################
public:
	virtual ~NUVAxes();

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void rotate(const Vector3d& axis, double angle) = 0;
	virtual void set_n(const Vector3d& n) = 0;

	//#################### PUBLIC METHODS ####################
public:
	const Vector3d& n() const;
	void set_n(const Vector3d& n, const Vector3d& up);
	const Vector3d& u() const;
	const Vector3d& v() const;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<NUVAxes> NUVAxes_Ptr;
typedef shared_ptr<const NUVAxes> NUVAxes_CPtr;

}

#endif
