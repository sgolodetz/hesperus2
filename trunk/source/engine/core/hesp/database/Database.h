/***
 * hesperus: Database.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_DATABASE
#define H_HESP_DATABASE

#include <hesp/util/Properties.h>

namespace hesp {

class Database
{
	//#################### PRIVATE VARIABLES ####################
private:
	Properties m_db;

	//#################### PUBLIC METHODS ####################
public:
	template <typename T> shared_ptr<T> get(const std::string& name, const shared_ptr<T>& = shared_ptr<T>()) const;
	template <typename T> shared_ptr<const T> get(const std::string& name, const shared_ptr<const T>& = shared_ptr<const T>()) const;
	bool has(const std::string& name) const;
	template <typename T> void set(const std::string& name, const shared_ptr<T>& value);
	template <typename T> void set(const std::string& name, const shared_ptr<const T>& value);
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Database> Database_Ptr;
typedef shared_ptr<const Database> Database_CPtr;

}

#include "Database.tpp"

#endif
