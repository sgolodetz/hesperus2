/***
 * hesperus: CmpProjectileWeaponUsable.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMPPROJECTILEWEAPONUSABLE
#define H_HESP_CMPPROJECTILEWEAPONUSABLE

#ifdef _MSC_VER
	#pragma warning(disable:4250)	// Disable the spurious VC++ warning about inheritance via the dominance rule: that's what I intend here
#endif

#include <boost/optional.hpp>

#include "CmpUsable.h"
#include "ICmpAmmoNeedingUsable.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
class MsgTimeElapsed;

class CmpProjectileWeaponUsable
:	public ICmpAmmoNeedingUsable,
	public CmpUsable,
	public MessageHandler<MsgTimeElapsed>
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::string m_ammoType;
	int m_firingInterval;				// the interval between firing one shot and the next (in milliseconds)
	double m_muzzleSpeed;
	int m_timeTillCanFire;				// the time remaining until the weapon can fire (in milliseconds)

	//#################### CONSTRUCTORS ####################
public:
	CmpProjectileWeaponUsable(const std::string& ammoType, int firingInterval, const std::string& usableGroup, const std::vector<std::string>& hotspots, double muzzleSpeed, int timeTillCanFire);

	//#################### STATIC FACTORY METHODS ####################
public:
	static IObjectComponent_Ptr load(const Properties& properties);

	//#################### PUBLIC METHODS ####################
public:
	const std::string& ammo_type() const;
	void check_dependencies() const;
	void process_message(const MsgTimeElapsed& msg);
	Properties save() const;
	void use();

	std::string own_type() const			{ return "ProjectileWeaponUsable"; }
	static std::string static_own_type()	{ return "ProjectileWeaponUsable"; }
};

}

#endif
