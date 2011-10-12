/***
 * hesperus: FieldIO.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_FIELDIO
#define H_HESP_FIELDIO

#include <iosfwd>
#include <string>
#include <utility>
#include <vector>

namespace hesp {

struct FieldIO
{
	//#################### READING METHODS ####################
	static std::string read_field(std::istream& is, const std::string& expectedFieldName);
	static std::string read_trimmed_field(std::istream& is, const std::string& expectedFieldName);
	template <typename T> static T read_typed_field(std::istream& is, const std::string& expectedFieldName);
	template <typename T> static T read_typed_trimmed_field(std::istream& is, const std::string& expectedFieldName);

	//#################### WRITING METHODS ####################
	template <typename T> static void write_typed_field(std::ostream& os, const std::string& fieldName, const T& value);

	//#################### HELPER METHODS ####################
	static std::pair<std::string,std::string> parse_field(const std::string& field, const std::string& expectedName = "");
};

}

#include "FieldIO.tpp"

#endif
