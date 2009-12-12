/***
 * hesperus: CmpUsable.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpUsable.h"

#include "ICmpModelRender.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpUsable::CmpUsable(const std::string& usableGroup, const std::vector<std::string>& hotspots)
:	m_usableGroup(usableGroup), m_hotspots(hotspots)
{}

//#################### PUBLIC METHODS ####################
void CmpUsable::check_dependencies() const
{
	check_dependency<ICmpModelRender>();
}

const std::vector<std::string>& CmpUsable::hotspots() const
{
	return m_hotspots;
}

void CmpUsable::set_hotspot_orientation(const std::string& name, const Vector3d& orientation)
{
	verify_hotspot_name(name);
	m_hotspotOrientations[name] = orientation;
}

void CmpUsable::set_hotspot_position(const std::string& name, const Vector3d& position)
{
	verify_hotspot_name(name);
	m_hotspotPositions[name] = position;
}

std::string CmpUsable::usable_group() const
{
	return m_usableGroup;
}

//#################### PROTECTED METHODS ####################
boost::optional<Vector3d> CmpUsable::hotspot_orientation(const std::string& name) const
{
	verify_hotspot_name(name);
	std::map<std::string,Vector3d>::const_iterator it = m_hotspotOrientations.find(name);
	if(it != m_hotspotOrientations.end()) return it->second;
	else return boost::none;
}

boost::optional<Vector3d> CmpUsable::hotspot_position(const std::string& name) const
{
	verify_hotspot_name(name);
	std::map<std::string,Vector3d>::const_iterator it = m_hotspotPositions.find(name);
	if(it != m_hotspotPositions.end()) return it->second;
	else return boost::none;
}

//#################### PRIVATE METHODS ####################
void CmpUsable::verify_hotspot_name(const std::string& name) const
{
	if(std::find(m_hotspots.begin(), m_hotspots.end(), name) == m_hotspots.end())
	{
		throw Exception("No such hotspot: " + name);
	}
}

}
