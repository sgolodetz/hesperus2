/***
 * hesperus: XMLParser.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "XMLParser.h"

#include <hesp/exceptions/Exception.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
XMLParser::XMLParser(const XMLLexer_Ptr& lexer)
:	m_lexer(lexer)
{}

//#################### PUBLIC METHODS ####################
XMLElement_CPtr XMLParser::parse()
{
	XMLElement_Ptr root(new XMLElement("<root>"));

	std::vector<XMLElement_Ptr> children = parse_elements();
	for(std::vector<XMLElement_Ptr>::const_iterator it=children.begin(), iend=children.end(); it!=iend; ++it)
	{
		root->add_child(*it);
	}

	return root;
}

//#################### PRIVATE METHODS ####################
void XMLParser::check_token_type(const XMLToken_Ptr& token, XMLTokenType expectedType)
{
	if(!token)
	{
		throw Exception("Token unexpectedly missing");
	}

	if(token->type() != expectedType)
	{
		throw Exception("Unexpected token type");
	}
}

XMLElement_Ptr XMLParser::parse_element()
{
	XMLToken_Ptr token;

	token = read_token();
	if(!token)
	{
		// If there are no tokens left, we're done.
		return XMLElement_Ptr();
	}

	if(token->type() != XMLT_LBRACKET)
	{
		// If the token isn't '<', we're reading something other than an element.
		m_lookahead.push_back(token);
		return XMLElement_Ptr();
	}

	token = read_checked_token(XMLT_IDENT);

	XMLElement_Ptr element(new XMLElement(token->value()));

	token = read_token();
	while(token && token->type() == XMLT_IDENT)						// while there are attributes to be processed
	{
		std::string attribName = token->value();
		read_checked_token(XMLT_EQUALS);
		token = read_checked_token(XMLT_VALUE);
		std::string attribValue = token->value();
		element->set_attribute(attribName, attribValue);

		token = read_token();
	}

	if(!token) throw Exception("Token unexpectedly missing");

	switch(token->type())
	{
		case XMLT_RBRACKET:
		{
			// This element has sub-elements, so parse them recursively and add them to the current element.
			std::vector<XMLElement_Ptr> children = parse_elements();
			for(std::vector<XMLElement_Ptr>::const_iterator it=children.begin(), iend=children.end(); it!=iend; ++it)
			{
				element->add_child(*it);
			}

			// Read the element closing tag.
			read_checked_token(XMLT_LSLASH);
			token = read_checked_token(XMLT_IDENT);
			if(token->value() != element->name()) throw Exception("Mismatched element tags: expected " + element->name() + " not " + token->value());
			read_checked_token(XMLT_RBRACKET);

			break;
		}
		case XMLT_RSLASH:
		{
			// The element is complete, so just break and return it.
			break;
		}
		default:
		{
			throw Exception("Unexpected token type");
		}
	}

	return element;
}

std::vector<XMLElement_Ptr> XMLParser::parse_elements()
{
	std::vector<XMLElement_Ptr> elements;

	XMLElement_Ptr element;
	while(element = parse_element())
	{
		elements.push_back(element);
	}

	return elements;
}

XMLToken_Ptr XMLParser::read_checked_token(XMLTokenType expectedType)
{
	XMLToken_Ptr token = read_token();
	check_token_type(token, expectedType);
	return token;
}

XMLToken_Ptr XMLParser::read_token()
{
	if(m_lookahead.empty())
	{
		return m_lexer->next_token();
	}
	else
	{
		XMLToken_Ptr token = m_lookahead.front();
		m_lookahead.pop_front();
		return token;
	}
}

}
