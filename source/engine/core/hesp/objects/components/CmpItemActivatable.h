/***
 * hesperus: CmpItemActivatable.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMPITEMACTIVATABLE
#define H_HESP_CMPITEMACTIVATABLE

#include "ICmpActivatable.h"

namespace hesp {

class CmpItemActivatable : public ICmpActivatable
{
	//#################### STATIC FACTORY METHODS ####################
public:
	static IObjectComponent_Ptr load(const Properties&);

	//#################### PUBLIC METHODS ####################
public:
	void activated_by(const ObjectID& activator);
	void check_dependencies() const;
	Properties save() const;

	std::string own_type() const			{ return "ItemActivatable"; }
	static std::string static_own_type()	{ return "ItemActivatable"; }
};

}

#endif
