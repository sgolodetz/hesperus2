/***
 * hesperus: NavLinkFactory.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "NavLinkFactory.h"

#include <hesp/exceptions/Exception.h>
#include <hesp/nav/StepDownLink.h>
#include <hesp/nav/StepUpLink.h>
#include <hesp/nav/WalkLink.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
NavLinkFactory::NavLinkFactory()
{
	m_loaders["StepDown"] = &StepDownLink::load;
	m_loaders["StepUp"] = &StepUpLink::load;
	m_loaders["Walk"] = &WalkLink::load;
}

//#################### PUBLIC METHODS ####################
NavLink_Ptr NavLinkFactory::construct_navlink(const std::string& line) const
{
	size_t n = line.find(' ');
	if(n == std::string::npos) throw Exception("Bad nav link: " + line);

	std::string type = line.substr(0,n);
	std::string data = line.substr(n+1);

	std::map<std::string,NavLinkLoader>::const_iterator it = m_loaders.find(type);
	if(it == m_loaders.end()) throw Exception("Unknown nav link type: " + type);

	NavLinkLoader loader = it->second;
	return loader(data);
}

}
