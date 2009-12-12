/***
 * hesperus: CmpUsable.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMPUSABLE
#define H_HESP_CMPUSABLE

#include "ICmpUsable.h"

namespace hesp {

class CmpUsable : public virtual ICmpUsable
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::string m_usableGroup;
	std::vector<std::string> m_hotspots;
	std::map<std::string,Vector3d> m_hotspotOrientations;
	std::map<std::string,Vector3d> m_hotspotPositions;

	//#################### CONSTRUCTORS ####################
public:
	CmpUsable(const std::string& usableGroup, const std::vector<std::string>& hotspots);

	//#################### PUBLIC METHODS ####################
public:
	void check_dependencies() const;
	const std::vector<std::string>& hotspots() const;
	void set_hotspot_orientation(const std::string& name, const Vector3d& orientation);
	void set_hotspot_position(const std::string& name, const Vector3d& position);
	std::string usable_group() const;

	//#################### PROTECTED METHODS ####################
protected:
	boost::optional<Vector3d> hotspot_orientation(const std::string& name) const;
	boost::optional<Vector3d> hotspot_position(const std::string& name) const;

	//#################### PRIVATE METHODS ####################
private:
	void verify_hotspot_name(const std::string& name) const;
};

}

#endif
