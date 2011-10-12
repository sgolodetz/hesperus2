/***
 * hesperus: ForceGeneratorRegistry.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ForceGeneratorRegistry.h"

#include <boost/lexical_cast.hpp>

#include <hesp/exceptions/Exception.h>
#include "ForceGenerator.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
void ForceGeneratorRegistry::deregister_id(int id)
{
	// Remove generators which act directly on the specified object.
	std::map<int,ForceGenerators>::iterator it = m_generators.find(id);
	if(it != m_generators.end()) m_generators.erase(it);
	else throw Exception("No such ID in force generator registry: " + boost::lexical_cast<std::string>(id));

	// Remove generators which reference the specified object.
	std::map<int,std::set<ForceGeneratorHandle> >::iterator jt = m_references.find(id);
	if(jt != m_references.end())
	{
		// Remove the referenced generators.
		for(std::set<ForceGeneratorHandle>::const_iterator kt=jt->second.begin(), kend=jt->second.end(); kt!=kend; ++kt)
		{
			const ForceGeneratorHandle& handle = *kt;
			std::map<int,ForceGenerators>::iterator mt = m_generators.find(handle.first);
			if(mt != m_generators.end()) mt->second.erase(handle.second);
			else throw Exception("Bad reference: No such force generator");
		}

		// Erase the set of references.
		m_references.erase(jt);
	}
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
	if(it == m_generators.end()) throw Exception("No such ID in force generator registry: " + boost::lexical_cast<std::string>(id));
	
	ForceGenerators::iterator jt = it->second.find(forceName);
	if(jt == it->second.end()) throw Exception("No such force name: " + forceName);

	ForceGenerator_CPtr generator = jt->second;

	// Remove all references to this generator.
	ForceGeneratorHandle handle = std::make_pair(id, forceName);
	std::vector<PhysicsObjectID> referencedObjects = generator->referenced_objects();
	for(std::vector<PhysicsObjectID>::const_iterator kt=referencedObjects.begin(), kend=referencedObjects.end(); kt!=kend; ++kt)
	{
		m_references[*kt].erase(handle);
	}

	// Remove the generator itself.
	it->second.erase(jt);
}

void ForceGeneratorRegistry::set_generator(int id, const std::string& forceName, const ForceGenerator_CPtr& generator)
{
	// Add the generator itself.
	m_generators[id][forceName] = generator;

	// Make a note of the objects referenced by the generator so that it can be removed if they are destroyed.
	ForceGeneratorHandle handle = std::make_pair(id, forceName);
	std::vector<PhysicsObjectID> referencedObjects = generator->referenced_objects();
	for(std::vector<PhysicsObjectID>::const_iterator it=referencedObjects.begin(), iend=referencedObjects.end(); it!=iend; ++it)
	{
		m_references[*it].insert(handle);
	}
}

}
