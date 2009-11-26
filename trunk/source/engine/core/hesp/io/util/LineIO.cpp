/***
 * hesperus: LineIO.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "LineIO.h"

#include <boost/algorithm/string/trim.hpp>

#include <hesp/exceptions/Exception.h>

namespace hesp {

//#################### READING METHODS ####################
/**
Attempts to read a line from a std::istream into a string and check its validity.

@param is			The std::istream
@param expected		What we expect the line to be
@throws Exception	If EOF is encountered
*/
void LineIO::read_checked_line(std::istream& is, const std::string& expected)
{
	std::string line;
	read_line(is, line, expected);
	if(line != expected) throw Exception("Expected " + expected);
}

/**
A version of read_checked_line which trims the read line before checking it.
*/
void LineIO::read_checked_trimmed_line(std::istream& is, const std::string& expected)
{
	std::string line;
	read_trimmed_line(is, line, expected);
	if(line != expected) throw Exception("Expected " + expected);
}

/**
Attempts to read a line from a std::istream into a string.

@param is			The std::istream
@param line			The string into which to read
@param description	A description of what we were trying to read (in case EOF is encountered)
@throws Exception	If EOF is encountered
*/
void LineIO::read_line(std::istream& is, std::string& line, const std::string& description)
{
	if(!std::getline(is, line)) throw Exception("Unexpected EOF whilst trying to read " + description);
}

/**
A version of read_line which trims the line after reading it in.
*/
void LineIO::read_trimmed_line(std::istream& is, std::string& line, const std::string& description)
{
	read_line(is, line, description);
	boost::trim(line);
}

}
