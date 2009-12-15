/***
 * hesperus: Database.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Database.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
bool Database::has(const std::string& name) const
{
	return m_db.has(name);
}

}
