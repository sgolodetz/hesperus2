/***
 * hesperus: CmpBasicProjectile.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpBasicProjectile.h"

#include <hesp/objects/messages/MsgObjectDestroyed.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpBasicProjectile::CmpBasicProjectile(int damageAmount, const ObjectID& firer)
:	m_damageAmount(damageAmount), m_firer(firer)
{}

//#################### STATIC FACTORY METHODS ####################
IObjectComponent_Ptr CmpBasicProjectile::load(const Properties& properties)
{
	return IObjectComponent_Ptr(new CmpBasicProjectile(properties.get<int>("DamageAmount"), properties.get<ObjectID>("Firer")));
}

//#################### PUBLIC METHODS ####################
int CmpBasicProjectile::damage_amount() const
{
	return m_damageAmount;
}

const ObjectID& CmpBasicProjectile::firer() const
{
	return m_firer;
}

void CmpBasicProjectile::process_message(const MsgObjectDestroyed& msg)
{
	if(msg.object_id() == m_firer)
	{
		m_firer = ObjectID();	// invalidate the ID we're holding: the specified object no longer exists
	}
}

void CmpBasicProjectile::register_listening()
{
	m_objectManager->add_listener(this, m_firer);
}

Properties CmpBasicProjectile::save() const
{
	Properties properties;
	properties.set("DamageAmount", m_damageAmount);
	properties.set("Firer", m_firer);
	return properties;
}

}
