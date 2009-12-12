/***
 * hesperus: ForceGeneratorRegistry.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ForceGeneratorRegistry.h"

#include <boost/lexical_cast.hpp>

#include <hesp/exceptions/Exception.h>

namespace hesp {

//#################### PUBLIC METHODS ####################
void ForceGeneratorRegistry::deregister_id(int id)
{
	// FIXME:	Need to remove force generators which act on objects other than the one with the specified ID but
	//			rely on the given object to function (e.g. spring force generators where the object being destroyed
	//			is referenced as the other endpoint of the spring).
	std::map<int,ForceGenerators>::iterator it = m_generators.find(id);
	if(it != m_generators.end()) m_generators.erase(it);
	else throw Exception("No such ID in force generator registry: " + boost::lexical_cast<std::string>(id));
}

const ForceGeneratorRegistry::ForceGenerators& ForceGeneratorRegistry::generators(int id) const
{
	std::map<int,ForceGenerators>::const_iterator it = m_generators.find(id);
	if(it != m_generators.end()) return it->second;
	else throw Exception("No such ID in force generator registry: " + boost::lexical_cast<std::string>(id));
}

void ForceGeneratorRegistry::register_id(int id)
{
	// Note: Doing a lookup for the ID will add it to the map if it's not already there.
	m_generators[id];
}

void ForceGeneratorRegistry::remove_generator(int id, const std::string& forceName)
{
	std::map<int,ForceGenerators>::iterator it = m_generators.find(id);
	if(it != m_generators.end())
	{
		it->second.erase(forceName);
	}
	else throw Exception("No such ID in force generator registry: " + boost::lexical_cast<std::string>(id));
}

void ForceGeneratorRegistry::set_generator(int id, const std::string& forceName, const ForceGenerator_CPtr& generator)
{
	m_generators[id][forceName] = generator;
}

}
