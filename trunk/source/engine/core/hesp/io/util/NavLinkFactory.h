/***
 * hesperus: NavLinkFactory.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_NAVLINKFACTORY
#define H_HESP_NAVLINKFACTORY

#include <map>
#include <string>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class NavLink> NavLink_Ptr;

class NavLinkFactory
{
	//#################### TYPEDEFS ####################
private:
	// A NavLinkLoader is a pointer to a function taking a const std::string& and returning a NavLink_Ptr
	typedef NavLink_Ptr (*NavLinkLoader)(const std::string&);

	//#################### PRIVATE VARIABLES ####################
private:
	std::map<std::string,NavLinkLoader> m_loaders;

	//#################### CONSTRUCTORS ####################
public:
	NavLinkFactory();

	//#################### PUBLIC METHODS ####################
public:
	NavLink_Ptr construct_navlink(const std::string& line) const;
};

}

#endif
