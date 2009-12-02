/***
 * hesperus: XMLLexer.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_XMLLEXER
#define H_HESP_XMLLEXER

#include <fstream>
#include <list>
#include <string>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class XMLToken> XMLToken_Ptr;

class XMLLexer
{
	//#################### ENUMERATIONS ####################
private:
	enum State	// the lexer is implemented as a finite-state machine
	{
		LEX_START,
		LEX_BAD,
		LEX_EOF,
		LEX_EQUALS,
		LEX_HALF_RSLASH,
		LEX_HALF_VALUE_D,
		LEX_HALF_VALUE_S,
		LEX_IDENT,
		LEX_LBRACKET,
		LEX_LSLASH,
		LEX_RBRACKET,
		LEX_RSLASH,
		LEX_VALUE,
	};

	//#################### PRIVATE VARIABLES ####################
private:
	std::ifstream m_is;
	bool m_eof;								// did we try and read past the end of the file?
	std::list<unsigned char> m_lookahead;	// sometimes we need to read ahead to lex properly - this contains previously read characters to re-read
	State m_state;

	//#################### CONSTRUCTORS ####################
public:
	XMLLexer(const std::string& filename);

	//#################### PUBLIC METHODS ####################
public:
	XMLToken_Ptr next_token();

	//#################### PRIVATE METHODS ####################
private:
	unsigned char next_char();
};

//#################### TYPEDEFS ####################
typedef shared_ptr<XMLLexer> XMLLexer_Ptr;

}

#endif
