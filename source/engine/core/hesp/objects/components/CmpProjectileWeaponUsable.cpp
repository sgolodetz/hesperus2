/***
 * hesperus: CmpProjectileWeaponUsable.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpProjectileWeaponUsable.h"

#include <hesp/objects/messages/MsgTimeElapsed.h>
#include <hesp/util/Properties.h>
#include "ICmpInventory.h"
#include "ICmpOwnable.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpProjectileWeaponUsable::CmpProjectileWeaponUsable(const std::string& ammoType, int firingInterval, const std::string& usableGroup,
													 const std::vector<std::string>& hotspots, double muzzleSpeed, int timeTillCanFire)
:	CmpUsable(usableGroup, hotspots), m_ammoType(ammoType), m_firingInterval(firingInterval), m_muzzleSpeed(muzzleSpeed), m_timeTillCanFire(timeTillCanFire)
{}

//#################### STATIC FACTORY METHODS ####################
IObjectComponent_Ptr CmpProjectileWeaponUsable::load(const Properties& properties)
{
	return IObjectComponent_Ptr(new CmpProjectileWeaponUsable(properties.get<std::string>("AmmoType"),
															  properties.get<int>("FiringInterval"),
															  properties.get<std::string>("Group"),
															  properties.get<std::vector<std::string> >("Hotspots"),
															  properties.get<double>("MuzzleSpeed"),
															  properties.get<int>("TimeTillCanFire")));
}

//#################### PUBLIC METHODS ####################
const std::string& CmpProjectileWeaponUsable::ammo_type() const
{
	return m_ammoType;
}

void CmpProjectileWeaponUsable::check_dependencies() const
{
	CmpUsable::check_dependencies();
	check_dependency<ICmpOwnable>();
}

void CmpProjectileWeaponUsable::process_message(const MsgTimeElapsed& msg)
{
	m_timeTillCanFire -= msg.milliseconds();
	if(m_timeTillCanFire < 0) m_timeTillCanFire = 0;
}

Properties CmpProjectileWeaponUsable::save() const
{
	Properties properties;
	properties.set("AmmoType", m_ammoType);
	properties.set("FiringInterval", m_firingInterval);
	properties.set("Group", usable_group());
	properties.set("Hotspots", hotspots());
	properties.set("MuzzleSpeed", m_muzzleSpeed);
	properties.set("TimeTillCanFire", m_timeTillCanFire);
	return properties;
}

void CmpProjectileWeaponUsable::use()
{
	if(m_timeTillCanFire == 0)
	{
		// Determine the character which is firing the projectile (the owner of the weapon).
		ICmpOwnable_CPtr cmpOwnable = m_objectManager->get_component(m_objectID, cmpOwnable);
		ObjectID firer = cmpOwnable->owner();
		if(!firer.valid()) throw Exception("Can't use a projectile weapon when it's not owned");

		// Check that there's enough ammo.
		ICmpInventory_Ptr cmpFirerInventory = m_objectManager->get_component(firer, cmpFirerInventory);
		if(!cmpFirerInventory) throw Exception("The firer must have an inventory component");
		if(cmpFirerInventory->consumables_count(m_ammoType) > 0)
		{
			// Fire a bullet from each hotspot of the weapon (note that this in principle makes it easy to implement things like double-barrelled shotguns).
			const std::vector<std::string>& spots = hotspots();
			for(size_t i=0, size=spots.size(); i<size; ++i)
			{
				boost::optional<Vector3d> pos = hotspot_position(spots[i]);
				boost::optional<Vector3d> ori = hotspot_orientation(spots[i]);
				if(pos && ori)
				{
					ObjectSpecification specification = m_objectManager->get_archetype(m_ammoType);
					specification.set_component_property("Projectile", "Firer", firer);
					specification.set_component_property("Simulation", "Position", *pos);
					specification.set_component_property("Simulation", "Velocity", m_muzzleSpeed * *ori);
					m_objectManager->queue_for_construction(specification);
				}
			}

			cmpFirerInventory->destroy_consumables(m_ammoType, 1);
			m_timeTillCanFire = m_firingInterval;
		}
	}
}

}
