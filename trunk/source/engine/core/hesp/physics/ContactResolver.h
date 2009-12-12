/***
 * hesperus: ContactResolver.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CONTACTRESOLVER
#define H_HESP_CONTACTRESOLVER

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
class Contact;
typedef shared_ptr<const class OnionTree> OnionTree_CPtr;

class ContactResolver
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~ContactResolver() {}

	//#################### PRIVATE ABSTRACT METHODS ####################
private:
	virtual void resolve_object_object(const Contact& contact, const OnionTree_CPtr& tree) const = 0;
	virtual void resolve_object_world(const Contact& contact) const = 0;

	//#################### PUBLIC METHODS ####################
public:
	virtual void resolve_contact(const Contact& contact, const OnionTree_CPtr& tree) const;
};

}

#endif
