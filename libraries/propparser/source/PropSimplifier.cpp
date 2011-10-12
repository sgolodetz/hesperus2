/***
 * propparser: PropSimplifier.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "PropSimplifier.h"

#include <iostream>
#include <sstream>

namespace {

std::ostringstream os;

class ParserException
{
private:
	std::string m_cause;

public:
	explicit ParserException(const std::string& cause)
	:	m_cause("Parse Error: " + cause)
	{}

	const std::string& cause() const	{ return m_cause; }
};

}

extern "C"
{
	typedef struct yy_buffer_state yy_buffer_state;

	void yypropsimplifier_delete_buffer(yy_buffer_state *);
	int yypropsimplifierparse(void);
	yy_buffer_state *yypropsimplifier_scan_string(const char *);

	void yypropsimplifiererror(char *s)
	{
		//fprintf(stderr, "%s\n", s);
		throw ParserException(s);
	}

	void yypropsimplifieroutput(const char *s)
	{
		if(s) os << s;
	}
}

namespace propparser {

std::string simplify_property(const std::string& propValue)
{
	os.clear();
	os.str("");

	yy_buffer_state *buffer = yypropsimplifier_scan_string(propValue.c_str());
	
	try							{ yypropsimplifierparse(); }
	catch(ParserException& e)	{ std::cout << e.cause() << std::endl; }

	yypropsimplifier_delete_buffer(buffer);

	return os.str();
}

}
