/***
 * hesperus: ResourceNamesSection.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <hesp/io/util/LineIO.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
template <typename Manager>
ResourceNamesSection<Manager>::ResourceNamesSection(const std::string& sectionName, const std::string& resourceType)
:	m_sectionName(sectionName), m_resourceType(resourceType)
{}

//#################### LOADING METHODS ####################
template <typename Manager>
shared_ptr<Manager> ResourceNamesSection<Manager>::load(std::istream& is)
{
	shared_ptr<Manager> manager(new Manager);

	LineIO::read_checked_line(is, m_sectionName);
	LineIO::read_checked_line(is, "{");

	std::string line;
	for(;;)
	{
		LineIO::read_trimmed_line(is, line, m_resourceType + " name");
		if(line == "}") break;
		if(line.length() == 0) continue;

		manager->register_resource(line);
	}

	return manager;
}

//#################### SAVING METHODS ####################
template <typename Manager>
void ResourceNamesSection<Manager>::save(std::ostream& os, const shared_ptr<const Manager>& manager)
{
	os << m_sectionName << '\n';
	os << "{\n";

	std::set<std::string> resourceNames = manager->resource_names();
	for(std::set<std::string>::const_iterator it=resourceNames.begin(), iend=resourceNames.end(); it!=iend; ++it)
	{
		os << '\t' << *it << '\n';
	}

	os << "}\n";
}

}
