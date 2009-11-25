/***
 * asx: ASXMessage.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_ASX_ASXMESSAGE
#define H_ASX_ASXMESSAGE

#include <iosfwd>
#include <string>

struct ASXMessage
{
	//#################### PUBLIC VARIABLES ####################
	std::string type;
	std::string section;
	int row, col;
	std::string message;

	//#################### CONSTRUCTORS ####################
	ASXMessage(const std::string& type_, const std::string& section_, int row_, int col_, const std::string& message_);

	//#################### PUBLIC METHODS ####################
	void output(std::ostream& os) const;
};

#endif
