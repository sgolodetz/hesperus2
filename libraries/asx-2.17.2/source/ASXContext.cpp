/***
 * asx: ASXContext.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ASXContext.h"

#include <iostream>

struct ASXContextReleaser
{
	void operator()(asIScriptContext *context)
	{
		context->Release();
	}
};

//#################### CONSTRUCTORS ####################
ASXContext::ASXContext(asIScriptContext *context, int funcID)
:	m_context(context, ASXContextReleaser()), m_funcID(funcID)
{
	m_context->SetExceptionCallback(asMETHOD(ASXContext,exception_callback), this, asCALL_THISCALL);
}

//#################### PUBLIC OPERATORS ####################
asIScriptContext *ASXContext::operator->() const
{
	return m_context.get();
}

//#################### PUBLIC METHODS ####################
int ASXContext::execute()
{
	return m_context->Execute();
}

int ASXContext::prepare()
{
	return m_context->Prepare(m_funcID);
}

//#################### PRIVATE METHODS ####################
void ASXContext::exception_callback(asIScriptContext *context)
{
	int col;
	int row = context->GetExceptionLineNumber(&col);

	std::cout	<< "A script exception occurred: "
				<< context->GetExceptionString() << " at position (" << row << ',' << col << ')'
				<< std::endl;
}
