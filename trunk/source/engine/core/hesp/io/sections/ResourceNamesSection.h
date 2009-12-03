/***
 * hesperus: ResourceNamesSection.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_RESOURCENAMESSECTION
#define H_HESP_RESOURCENAMESSECTION

#include <string>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

template <typename Manager>
class ResourceNamesSection
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::string m_sectionName;
	std::string m_resourceType;

	//#################### CONSTRUCTORS ####################
public:
	ResourceNamesSection(const std::string& sectionName, const std::string& resourceType);

	//#################### LOADING METHODS ####################
public:
	shared_ptr<Manager> load(std::istream& is);

	//#################### SAVING METHODS ####################
public:
	void save(std::ostream& os, const shared_ptr<const Manager>& manager);
};

}

#include "ResourceNamesSection.tpp"

#endif
