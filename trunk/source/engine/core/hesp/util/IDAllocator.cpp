/***
 * hesperus: IDAllocator.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "IDAllocator.h"

#include <boost/lexical_cast.hpp>
using boost::lexical_cast;

#include <hesp/exceptions/Exception.h>

namespace hesp {

//#################### PUBLIC METHODS ####################
int IDAllocator::allocate()
{
	int n;

	if(!m_free.empty())
	{
		n = *m_free.begin();
		m_free.erase(m_free.begin());
	}
	else
	{
		n = static_cast<int>(m_used.size());
	}

	m_used.insert(n);
	return n;
}

void IDAllocator::deallocate(int n)
{
	std::set<int>::iterator it = m_used.find(n);
	if(it == m_used.end()) throw Exception("ID " + lexical_cast<std::string,int>(n) + " is not currently allocated");

	// Note: If we get here, m_used is guaranteed to be non-empty (as it contains n).
	if(n == *m_used.rbegin())
	{
		// We're removing the last element of the used set.
		m_used.erase(it);
		m_free.erase(m_free.upper_bound(max_used()), m_free.end());
	}
	else
	{
		m_used.erase(it);
		m_free.insert(n);
	}
}

void IDAllocator::reset()
{
	m_free.clear();
	m_used.clear();
}

//#################### PRIVATE METHODS ####################
int IDAllocator::max_used() const
{
	if(m_used.empty()) return -1;
	else return *m_used.rbegin();
}

}
