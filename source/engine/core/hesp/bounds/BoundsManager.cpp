/***
 * hesperus: BoundsManager.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "BoundsManager.h"

#include <boost/lexical_cast.hpp>
using boost::lexical_cast;

#include <hesp/exceptions/Exception.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
BoundsManager::BoundsManager(const std::vector<Bounds_CPtr>& bounds, const std::map<std::string,BoundsGroup>& groups, const std::map<std::string,int>& lookup,
							 const boost::dynamic_bitset<>& navFlags)
:	m_bounds(bounds), m_groups(groups), m_lookup(lookup), m_navFlags(navFlags)
{}

//#################### PUBLIC METHODS ####################
const Bounds_CPtr& BoundsManager::bounds(int index) const
{
	if(0 <= index && index < bounds_count()) return m_bounds[index];
	else throw Exception("Bounds index out of range: " + lexical_cast<std::string>(index));
}

const Bounds_CPtr& BoundsManager::bounds(const std::string& groupName, const std::string& posture) const
{
	return bounds(lookup_bounds_index(groupName, posture));
}

int BoundsManager::bounds_count() const
{
	return static_cast<int>(m_bounds.size());
}

int BoundsManager::lookup_bounds_index(const std::string& groupName, const std::string& posture) const
{
	std::map<std::string,BoundsGroup>::const_iterator it = m_groups.find(groupName);
	if(it != m_groups.end())
	{
		const BoundsGroup& group = it->second;
		BoundsGroup::const_iterator jt = group.find(posture);
		if(jt != group.end())
		{
			return lookup_bounds_index(jt->second);
		}
		else throw Exception("No such posture: " + posture + "(in bounds group " + groupName + ")");
	}
	else throw Exception("No such bounds group: " + groupName);
}

const boost::dynamic_bitset<>& BoundsManager::nav_flags() const
{
	return m_navFlags;
}

//#################### PRIVATE METHODS ####################
int BoundsManager::lookup_bounds_index(const std::string& name) const
{
	std::map<std::string,int>::const_iterator it = m_lookup.find(name);
	if(it != m_lookup.end()) return it->second;
	else throw Exception("No such bounds: " + name);
}

}
