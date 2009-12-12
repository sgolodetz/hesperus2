/***
 * hesperus: CmpHealth.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpHealth.h"

#include <hesp/util/Properties.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpHealth::CmpHealth(int health, int maxHealth)
:	m_health(health), m_maxHealth(maxHealth)
{}

//#################### STATIC FACTORY METHODS ####################
IObjectComponent_Ptr CmpHealth::load(const Properties& properties)
{
	return IObjectComponent_Ptr(new CmpHealth(properties.get<int>("Health"), properties.get<int>("MaxHealth")));
}

//#################### PUBLIC METHODS ####################
void CmpHealth::damage(int amount)
{
	// Note: More sophisticated implementations are possible (e.g. involving damage resistance, etc.).
	if(status() == ALIVE)
	{
		m_health -= amount;
		if(m_health < 0) m_health = 0;
	}
}

int CmpHealth::health() const
{
	return m_health;
}

void CmpHealth::kill()
{
	m_health = -1;
}

int CmpHealth::max_health() const
{
	return m_maxHealth;
}

Properties CmpHealth::save() const
{
	Properties properties;
	properties.set("Health", m_health);
	properties.set("MaxHealth", m_maxHealth);
	return properties;
}

CmpHealth::HealthStatus CmpHealth::status() const
{
	if(m_health > 0)		return ALIVE;
	else if(m_health == 0)	return DYING;
	else					return DEAD;
}

}
