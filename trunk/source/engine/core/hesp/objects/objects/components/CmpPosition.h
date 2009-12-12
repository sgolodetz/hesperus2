/***
 * hesperus: CmpPosition.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMPPOSITION
#define H_HESP_CMPPOSITION

#include "ICmpPosition.h"

namespace hesp {

class CmpPosition : public ICmpPosition
{
	//#################### PRIVATE VARIABLES ####################
private:
	Vector3d m_position;

	//#################### CONSTRUCTORS ####################
public:
	explicit CmpPosition(const Vector3d& position);

	//#################### STATIC FACTORY METHODS ####################
public:
	static IObjectComponent_Ptr load(const Properties& properties);

	//#################### PUBLIC METHODS ####################
public:
	const Vector3d& position() const;
	Properties save() const;
	void set_position(const Vector3d& position);
};

}

#endif
