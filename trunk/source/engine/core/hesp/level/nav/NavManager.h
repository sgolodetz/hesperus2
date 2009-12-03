/***
 * hesperus: NavManager.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_NAVMANAGER
#define H_HESP_NAVMANAGER

#include <map>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class NavDataset> NavDataset_Ptr;
typedef shared_ptr<const class NavDataset> NavDataset_CPtr;

class NavManager
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::map<int,NavDataset_Ptr> m_datasets;

	//#################### PUBLIC METHODS ####################
public:
	const NavDataset_Ptr& dataset(int index);
	NavDataset_CPtr dataset(int index) const;
	std::map<int,NavDataset_CPtr> datasets() const;
	void set_dataset(int index, const NavDataset_Ptr& dataset);
};

}

#endif
