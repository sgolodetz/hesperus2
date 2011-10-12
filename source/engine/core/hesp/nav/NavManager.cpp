/***
 * hesperus: NavManager.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "NavManager.h"

#include <boost/lexical_cast.hpp>
using boost::lexical_cast;

#include <hesp/exceptions/Exception.h>

namespace hesp {

//#################### PUBLIC METHODS ####################
const NavDataset_Ptr& NavManager::dataset(int index)
{
	std::map<int,NavDataset_Ptr>::iterator it = m_datasets.find(index);
	if(it != m_datasets.end()) return it->second;
	else throw Exception("No nav dataset available for index: " + lexical_cast<std::string>(index));
}

NavDataset_CPtr NavManager::dataset(int index) const
{
	std::map<int,NavDataset_Ptr>::const_iterator it = m_datasets.find(index);
	if(it != m_datasets.end()) return it->second;
	else throw Exception("No nav dataset available for index: " + lexical_cast<std::string>(index));
}

std::map<int,NavDataset_CPtr> NavManager::datasets() const
{
	return std::map<int,NavDataset_CPtr>(m_datasets.begin(), m_datasets.end());
}

void NavManager::set_dataset(int index, const NavDataset_Ptr& dataset)
{
	if(dataset != NULL) m_datasets[index] = dataset;
	else throw Exception("Must provide a valid nav dataset for index: " + lexical_cast<std::string>(index));
}

}
