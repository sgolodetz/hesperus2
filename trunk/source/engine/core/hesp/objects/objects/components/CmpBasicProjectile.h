/***
 * hesperus: CmpBasicProjectile.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMPBASICPROJECTILE
#define H_HESP_CMPBASICPROJECTILE

#include "ICmpProjectile.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
class MsgObjectDestroyed;

class CmpBasicProjectile
:	public ICmpProjectile,
	public MessageHandler<MsgObjectDestroyed>
{
	//#################### PRIVATE VARIABLES ####################
private:
	int m_damageAmount;
	ObjectID m_firer;

	//#################### CONSTRUCTORS ####################
public:
	explicit CmpBasicProjectile(int damageAmount, const ObjectID& firer);

	//#################### STATIC FACTORY METHODS ####################
public:
	static IObjectComponent_Ptr load(const Properties& properties);

	//#################### PUBLIC METHODS ####################
public:
	int damage_amount() const;
	const ObjectID& firer() const;
	void process_message(const MsgObjectDestroyed& msg);
	void register_listening();
	Properties save() const;

	std::string own_type() const			{ return "BasicProjectile"; }
	static std::string static_own_type()	{ return "BasicProjectile"; }
};

}

#endif
