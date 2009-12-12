/***
 * hesperus: ListenerTable.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ListenerTable.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
void ListenerTable::add_listener(const ObjectID& sourceObject, const std::string& sourceComponent, const ObjectID& destObject)
{
	m_table.insert(Entry(sourceObject, sourceComponent, destObject));
}

std::vector<std::pair<ObjectID,std::string> > ListenerTable::get_listeners(const ObjectID& destObject) const
{
	typedef Table::index<dest>::type DestTable;
	typedef DestTable::iterator DestIter;

	const DestTable& destTable = m_table.get<dest>();
	std::pair<DestIter,DestIter> p = destTable.equal_range(destObject);

	std::vector<std::pair<ObjectID,std::string> > ret;
	for(DestIter it=p.first, iend=p.second; it!=iend; ++it)
	{
		const Entry& e = *it;
		ret.push_back(std::make_pair(e.m_sourceObject, e.m_sourceComponent));
	}
	return ret;
}

void ListenerTable::remove_listener(const ObjectID& sourceObject, const std::string& sourceComponent, const ObjectID& destObject)
{
	m_table.erase(Entry(sourceObject, sourceComponent, destObject));
}

void ListenerTable::remove_listeners_from(const ObjectID& sourceObject)
{
	typedef Table::index<source>::type SourceTable;
	typedef SourceTable::iterator SourceIter;

	SourceTable& sourceTable = m_table.get<source>();
	sourceTable.erase(sourceObject);
}

void ListenerTable::remove_listeners_to(const ObjectID& destObject)
{
	typedef Table::index<dest>::type DestTable;
	typedef DestTable::iterator DestIter;

	DestTable& destTable = m_table.get<dest>();
	destTable.erase(destObject);
}

}
