/***
 * hesperus: PropertyIO.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_PROPERTYIO
#define H_HESP_PROPERTYIO

#include <iosfwd>
#include <string>

#include <hesp/util/Properties.h>

namespace hesp {

struct PropertyIO
{
	//#################### LOADING METHODS ####################
	static void load_property(Properties& properties, const std::string& name, const std::string& type, const std::string& value);
	template <typename T> static void load_typed_property(Properties& properties, const std::string& name, const std::string& value);

	//#################### SAVING METHODS ####################
	static void save_property(std::ostream& os, const Properties& properties, const std::string& name, const std::string& type);
	template <typename T> static void save_typed_property(std::ostream& os, const Properties& properties, const std::string& name);
};

}

#include "PropertyIO.tpp"

#endif
