/***
 * hesperus: XMLLexer.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "XMLLexer.h"

#include <hesp/exceptions/Exception.h>
#include "XMLToken.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
XMLLexer::XMLLexer(const std::string& filename)
:	m_is(filename.c_str()), m_eof(false), m_state(LEX_START)
{
	if(m_is.fail()) throw Exception("Could not open " + filename + " for reading");
}

//#################### PUBLIC METHODS ####################
XMLToken_Ptr XMLLexer::next_token()
{
	if(m_state == LEX_EOF) return XMLToken_Ptr();
	else m_state = LEX_START;

	std::string value;

	for(;;)
	{
		switch(m_state)
		{
			case LEX_START:
			{
				unsigned char c = next_char();
				if(m_eof)													{ m_state = LEX_EOF; }
				else if(c == '=')											{ m_state = LEX_EQUALS; }
				else if(c == '/')											{ m_state = LEX_HALF_RSLASH; }
				else if(c == '"')											{ m_state = LEX_HALF_VALUE_D; }
				else if(c == '\'')											{ m_state = LEX_HALF_VALUE_S; }
				else if(c == '<')											{ m_state = LEX_LBRACKET; }
				else if(c == '>')											{ m_state = LEX_RBRACKET; }
				else if(isalpha(c) || isdigit(c) || c == '.')				{ m_state = LEX_IDENT; value += c; }
				break;
			}
			case LEX_EOF:
			{
				return XMLToken_Ptr();
			}
			case LEX_EQUALS:
			{
				return XMLToken_Ptr(new XMLToken(XMLT_EQUALS, ""));
			}
			case LEX_HALF_RSLASH:
			{
				unsigned char c = next_char();
				if(m_eof)													{ m_state = LEX_BAD; value = ">"; }
				else if(c == '>')											{ m_state = LEX_RSLASH; }
				else														{ m_state = LEX_BAD; value = ">"; }
				break;
			}
			case LEX_HALF_VALUE_D:
			{
				unsigned char c = next_char();
				if(m_eof)													{ m_state = LEX_BAD; value = "\""; }
				else if(c == '"')											{ m_state = LEX_VALUE; }
				else														{ value += c; }
				break;
			}
			case LEX_HALF_VALUE_S:
			{
				unsigned char c = next_char();
				if(m_eof)													{ m_state = LEX_BAD; value = "\""; }
				else if(c == '\'')											{ m_state = LEX_VALUE; }
				else														{ value += c; }
				break;
			}
			case LEX_IDENT:
			{
				unsigned char c = next_char();
				if(m_eof)													{ m_state = LEX_EOF; return XMLToken_Ptr(new XMLToken(XMLT_IDENT, value)); }
				else if(isalpha(c) || isdigit(c) || c == '.' || c == '_')	{ value += c; }
				else														{ m_lookahead.push_back(c); return XMLToken_Ptr(new XMLToken(XMLT_IDENT, value)); }
				break;
			}
			case LEX_LBRACKET:
			{
				unsigned char c = next_char();
				if(m_eof)													{ m_state = LEX_EOF; return XMLToken_Ptr(new XMLToken(XMLT_LBRACKET, "")); }
				else if(c == '/')											{ m_state = LEX_LSLASH; }
				else														{ m_lookahead.push_back(c); return XMLToken_Ptr(new XMLToken(XMLT_LBRACKET, "")); }
				break;
			}
			case LEX_LSLASH:
			{
				return XMLToken_Ptr(new XMLToken(XMLT_LSLASH, ""));
			}
			case LEX_RBRACKET:
			{
				return XMLToken_Ptr(new XMLToken(XMLT_RBRACKET, ""));
			}
			case LEX_RSLASH:
			{
				return XMLToken_Ptr(new XMLToken(XMLT_RSLASH, ""));
			}
			case LEX_VALUE:
			{
				return XMLToken_Ptr(new XMLToken(XMLT_VALUE, value));
			}
			default:	// case LEX_BAD
			{
				throw Exception("Error: Expected " + value);
			}
		}
	}
}

//#################### PRIVATE METHODS ####################
unsigned char XMLLexer::next_char()
{
	if(m_lookahead.empty())
	{
		unsigned char c = m_is.get();
		if(m_is.eof()) m_eof = true;
		return c;
	}
	else
	{
		unsigned char c = m_lookahead.front();
		m_lookahead.pop_front();
		m_eof = false;
		return c;
	}
}

}
