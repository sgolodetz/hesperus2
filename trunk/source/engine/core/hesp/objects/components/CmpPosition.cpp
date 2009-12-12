/***
 * hesperus: CmpPosition.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpPosition.h"

#include <hesp/util/Properties.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpPosition::CmpPosition(const Vector3d& position)
:	m_position(position)
{}

//#################### STATIC FACTORY METHODS ####################
IObjectComponent_Ptr CmpPosition::load(const Properties& properties)
{
	return IObjectComponent_Ptr(new CmpPosition(properties.get<Vector3d>("Position")));
}

//#################### PUBLIC METHODS ####################
const Vector3d& CmpPosition::position() const
{
	return m_position;
}

Properties CmpPosition::save() const
{
	Properties properties;
	properties.set("Position", m_position);
	return properties;
}

void CmpPosition::set_position(const Vector3d& position)
{
	m_position = position;
}

}
