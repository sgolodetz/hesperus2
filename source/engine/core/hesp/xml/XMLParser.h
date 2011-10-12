/***
 * hesperus: XMLParser.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_XMLPARSER
#define H_HESP_XMLPARSER

#include <vector>

#include "XMLElement.h"
#include "XMLLexer.h"
#include "XMLToken.h"

namespace hesp {

class XMLParser
{
	//#################### PRIVATE VARIABLES ####################
private:
	XMLLexer_Ptr m_lexer;
	std::list<XMLToken_Ptr> m_lookahead;	// sometimes we have to read ahead to parse properly - this contains previously read tokens to re-read

	//#################### CONSTRUCTORS ####################
public:
	XMLParser(const XMLLexer_Ptr& lexer);

	//#################### PUBLIC METHODS ####################
public:
	XMLElement_CPtr parse();

	//#################### PRIVATE METHODS ####################
private:
	void check_token_type(const XMLToken_Ptr& token, XMLTokenType expectedType);
	XMLElement_Ptr parse_element();
	std::vector<XMLElement_Ptr> parse_elements();
	XMLToken_Ptr read_checked_token(XMLTokenType expectedType);
	XMLToken_Ptr read_token();
};

}

#endif
