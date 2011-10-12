/***
 * hesperus: BoundsManager.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_BOUNDSMANAGER
#define H_HESP_BOUNDSMANAGER

#include <map>
#include <vector>

#include <boost/dynamic_bitset.hpp>
#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<const class Bounds> Bounds_CPtr;

class BoundsManager
{
	//#################### TYPEDEFS ####################
public:
	typedef std::map<std::string,std::string> BoundsGroup;

	//#################### PRIVATE VARIABLES ####################
private:
	std::vector<Bounds_CPtr> m_bounds;
	std::map<std::string,BoundsGroup> m_groups;
	std::map<std::string,int> m_lookup;
	boost::dynamic_bitset<> m_navFlags;

	//#################### CONSTRUCTORS ####################
public:
	BoundsManager(const std::vector<Bounds_CPtr>& bounds, const std::map<std::string,BoundsGroup>& groups, const std::map<std::string,int>& lookup, const boost::dynamic_bitset<>& navFlags);

	//#################### PUBLIC METHODS ####################
public:
	const Bounds_CPtr& bounds(int index) const;
	const Bounds_CPtr& bounds(const std::string& groupName, const std::string& posture) const;
	int bounds_count() const;
	int lookup_bounds_index(const std::string& groupName, const std::string& posture) const;
	const boost::dynamic_bitset<>& nav_flags() const;

	//#################### PRIVATE METHODS ####################
private:
	int lookup_bounds_index(const std::string& name) const;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<BoundsManager> BoundsManager_Ptr;
typedef shared_ptr<const BoundsManager> BoundsManager_CPtr;

}

#endif
