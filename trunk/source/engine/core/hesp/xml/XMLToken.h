/***
 * hesperus: XMLToken.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_XMLTOKEN
#define H_HESP_XMLTOKEN

#include <string>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

enum XMLTokenType
{
	XMLT_EQUALS,		// =
	XMLT_IDENT,			// identifier
	XMLT_LBRACKET,		// <
	XMLT_LSLASH,		// </
	XMLT_RBRACKET,		// >
	XMLT_RSLASH,		// />
	XMLT_VALUE,			// "attribute value"
};

class XMLToken
{
	//#################### PRIVATE VARIABLES ####################
private:
	XMLTokenType m_type;
	std::string m_value;

	//#################### CONSTRUCTORS ####################
public:
	XMLToken(XMLTokenType type, const std::string& value)
	:	m_type(type), m_value(value)
	{}

	//#################### PUBLIC METHODS ####################
public:
	XMLTokenType type() const			{ return m_type; }
	const std::string& value() const	{ return m_value; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<XMLToken> XMLToken_Ptr;

}

#endif
