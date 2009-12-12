/***
 * hesperus: CmpConsumeActivatable.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMPCONSUMEACTIVATABLE
#define H_HESP_CMPCONSUMEACTIVATABLE

#include "ICmpActivatable.h"

namespace hesp {

class CmpConsumeActivatable : public ICmpActivatable
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::string m_type;
	int m_amount;

	//#################### CONSTRUCTORS ####################
public:
	CmpConsumeActivatable(const std::string& type, int amount);

	//#################### STATIC FACTORY METHODS ####################
public:
	static IObjectComponent_Ptr load(const Properties& properties);

	//#################### PUBLIC METHODS ####################
public:
	void activated_by(const ObjectID& activator);
	Properties save() const;

	std::string own_type() const			{ return "ConsumeActivatable"; }
	static std::string static_own_type()	{ return "ConsumeActivatable"; }
};

}

#endif
