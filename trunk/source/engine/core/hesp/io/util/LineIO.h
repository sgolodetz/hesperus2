/***
 * hesperus: LineIO.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LINEIO
#define H_HESP_LINEIO

#include <iosfwd>
#include <string>

namespace hesp {

struct LineIO
{
	//#################### READING METHODS ####################
	static void read_checked_line(std::istream& is, const std::string& expected);
	static void read_checked_trimmed_line(std::istream& is, const std::string& expected);
	static void read_line(std::istream& is, std::string& line, const std::string& description);
	static void read_trimmed_line(std::istream& is, std::string& line, const std::string& description);
};

}

#endif
