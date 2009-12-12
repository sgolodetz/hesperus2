/***
 * hesperus: CmpOwnable.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpOwnable.h"

#include <hesp/util/Properties.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpOwnable::CmpOwnable(const std::string& animExtension, const std::string& attachPoint, const ObjectID& owner)
:	m_animExtension(animExtension), m_attachPoint(attachPoint), m_owner(owner)
{}

//#################### STATIC FACTORY METHODS ####################
IObjectComponent_Ptr CmpOwnable::load(const Properties& properties)
{
	return IObjectComponent_Ptr(new CmpOwnable(properties.get<std::string>("AnimExtension"),
											   properties.get<std::string>("AttachPoint"),
											   properties.get<ObjectID>("Owner")));
}

//#################### PUBLIC METHODS ####################
const std::string& CmpOwnable::anim_extension() const
{
	return m_animExtension;
}

const std::string& CmpOwnable::attach_point() const
{
	return m_attachPoint;
}

void CmpOwnable::clear_owner()
{
	m_owner = ObjectID();
}

const ObjectID& CmpOwnable::owner() const
{
	return m_owner;
}

Properties CmpOwnable::save() const
{
	Properties properties;
	properties.set("AnimExtension", m_animExtension);
	properties.set("AttachPoint", m_attachPoint);
	properties.set("Owner", m_owner);
	return properties;
}

void CmpOwnable::set_owner(const ObjectID& owner)
{
	m_owner = owner;
}

}
