/***
 * hesperus: IDAllocator.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_IDALLOCATOR
#define H_HESP_IDALLOCATOR

#include <set>

namespace hesp {

class IDAllocator
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::set<int> m_free;
	std::set<int> m_used;

	//#################### PUBLIC METHODS ####################
public:
	int allocate();
	void deallocate(int n);
	void reset();

	//#################### PRIVATE METHODS ####################
private:
	int max_used() const;
};

}

#endif
