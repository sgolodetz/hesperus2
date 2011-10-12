/***
 * hesperus: Properties.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_PROPERTIES
#define H_HESP_PROPERTIES

#include <map>
#include <string>

#include <boost/any.hpp>
#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

class Properties
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::map<std::string,boost::any> m_properties;

	//#################### PUBLIC METHODS ####################
public:
	template <typename T> const T& get(const std::string& name) const;
	bool has(const std::string& name) const;
	template <typename T> void set(const std::string& name, const T& value);

	//#################### PRIVATE METHODS ####################
private:
	template <typename T> shared_ptr<T> get_ptr(const std::string& name) const;
};

}

#include "Properties.tpp"

#endif
