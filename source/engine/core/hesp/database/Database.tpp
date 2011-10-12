/***
 * hesperus: Database.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <hesp/exceptions/Exception.h>

namespace hesp {

//#################### PUBLIC METHODS ####################
template <typename T>
shared_ptr<T> Database::get(const std::string& name, const shared_ptr<T>&) const
{
	return m_db.get<shared_ptr<T> >(name);
}

template <typename T>
shared_ptr<const T> Database::get(const std::string& name, const shared_ptr<const T>&) const
{
	// The entry may have been stored as a pointer to const or non-const - we need to check its const flag to find out which.
	bool constFlag = m_db.get<bool>("constflag://" + name.substr(5));
	if(constFlag) return m_db.get<shared_ptr<const T> >(name);
	else return m_db.get<shared_ptr<T> >(name);
}

template <typename T>
void Database::set(const std::string& name, const shared_ptr<T>& value)
{
	if(name.substr(0,5) != "db://")
	{
		// Note: This is to make it easy to search for them in the code.
		throw Exception("Names of database entries must start with db://");
	}

	m_db.set(name, value);
	m_db.set("constflag://" + name.substr(5), false);
}

template <typename T>
void Database::set(const std::string& name, const shared_ptr<const T>& value)
{
	if(name.substr(0,5) != "db://")
	{
		// Note: This is to make it easy to search for them in the code.
		throw Exception("Names of database entries must start with db://");
	}

	m_db.set(name, value);
	m_db.set("constflag://" + name.substr(5), true);
}

}
