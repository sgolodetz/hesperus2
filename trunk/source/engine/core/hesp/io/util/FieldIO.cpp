/***
 * hesperus: FieldIO.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "FieldIO.h"

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <hesp/exceptions/Exception.h>
#include "LineIO.h"

namespace hesp {

//#################### READING METHODS ####################
std::string FieldIO::read_field(std::istream& is, const std::string& expectedFieldName)
{
	std::string field;
	LineIO::read_line(is, field, expectedFieldName);
	return parse_field(field, expectedFieldName).second;
}

std::string FieldIO::read_trimmed_field(std::istream& is, const std::string& expectedFieldName)
{
	std::string field;
	LineIO::read_trimmed_line(is, field, expectedFieldName);
	return parse_field(field, expectedFieldName).second;
}

//#################### HELPER METHODS ####################
std::pair<std::string,std::string> FieldIO::parse_field(const std::string& field, const std::string& expectedName)
{
	size_t i = field.find('=');
	if(i == std::string::npos) throw Exception("Bad field: " + field);

	std::string name = field.substr(0, i-1);
	if(expectedName != "" && name != expectedName) throw Exception("Bad field name " + name + ", expected " + expectedName);

	if(field.length() <= i+2) throw Exception("Missing field value: " + field);
	std::string value = field.substr(i+2);

	return std::make_pair(name, value);
}

}
