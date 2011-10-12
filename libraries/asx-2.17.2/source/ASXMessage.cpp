/***
 * asx: ASXMessage.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ASXMessage.h"

#include <iostream>

//#################### CONSTRUCTORS ####################
ASXMessage::ASXMessage(const std::string& type_, const std::string& section_, int row_, int col_, const std::string& message_)
:	type(type_), section(section_), row(row_), col(col_), message(message_)
{}

//#################### PUBLIC METHODS ####################
void ASXMessage::output(std::ostream& os) const
{
	os << section << '(' << row << ',' << col << ") : " << type << " : " << message << '\n';
}
