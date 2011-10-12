/***
 * hesperus: BounceContactResolver.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_BOUNCECONTACTRESOLVER
#define H_HESP_BOUNCECONTACTRESOLVER

#include <hesp/physics/ContactResolver.h>

namespace hesp {

class BounceContactResolver : public ContactResolver
{
	//#################### PRIVATE VARIABLES ####################
private:
	double m_restitutionCoefficient;

	//#################### CONSTRUCTORS ####################
public:
	explicit BounceContactResolver(double restitutionCoefficient);

	//#################### PRIVATE METHODS ####################
private:
	void resolve_object_object(const Contact& contact, const OnionTree_CPtr& tree) const;
	void resolve_object_world(const Contact& contact) const;
};

}

#endif
