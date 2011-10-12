/***
 * asx: ASXContext.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_ASX_ASXCONTEXT
#define H_ASX_ASXCONTEXT

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <angelscript.h>

class ASXContext
{
	//#################### PRIVATE VARIABLES ####################
private:
	shared_ptr<asIScriptContext> m_context;
	int m_funcID;

	//#################### CONSTRUCTORS ####################
public:
	ASXContext(asIScriptContext *context, int funcID);

	//#################### PUBLIC OPERATORS ####################
public:
	asIScriptContext *operator->() const;

	//#################### PUBLIC METHODS ####################
public:
	int execute();
	int prepare();

	//#################### PRIVATE METHODS ####################
private:
	void exception_callback(asIScriptContext *context);
};

#endif
