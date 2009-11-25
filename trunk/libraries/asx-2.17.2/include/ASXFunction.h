/***
 * asx: ASXFunction.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_ASX_ASXFUNCTION
#define H_ASX_ASXFUNCTION

#include "ASXContext.h"
#include "ASXException.h"
#include "ASXGetReturnValue.h"
#include "ASXSetArgValue.h"

//#################### MAIN TEMPLATE ####################
template <typename F> class ASXFunction;

//#################### PARTIAL SPECIALIZATIONS ####################

// No arguments
template <typename R> class ASXFunction<R ()>
{
	//#################### PRIVATE VARIABLES ####################
private:
	ASXContext m_context;

	//#################### CONSTRUCTORS ####################
public:
	ASXFunction(const ASXContext& context)
	:	m_context(context)
	{}

	//#################### PUBLIC OPERATORS ####################
public:
	R operator()()
	{
		int err = m_context.prepare();
		if(err < 0) throw ASXException("Error preparing script function context");

		err = m_context.execute();
		if(err < 0) throw ASXException("Error executing script function");

		return ASXGetReturnValue<R>()(m_context);
	}
};

// 1 argument
template <typename R, typename Arg0> class ASXFunction<R (Arg0)>
{
	//#################### PRIVATE VARIABLES ####################
private:
	ASXContext m_context;

	//#################### CONSTRUCTORS ####################
public:
	ASXFunction(const ASXContext& context)
	:	m_context(context)
	{}

	//#################### PUBLIC OPERATORS ####################
public:
	R operator()(Arg0 value0)
	{
		int err = m_context.prepare();
		if(err < 0) throw ASXException("Error preparing script function context");

		ASXSetArgValue<Arg0>()(m_context, 0, value0);

		err = m_context.execute();
		if(err < 0) throw ASXException("Error executing script function");

		return ASXGetReturnValue<R>()(m_context);
	}
};

// 2 arguments
template <typename R, typename Arg0, typename Arg1> class ASXFunction<R (Arg0,Arg1)>
{
	//#################### PRIVATE VARIABLES ####################
private:
	ASXContext m_context;

	//#################### CONSTRUCTORS ####################
public:
	ASXFunction(const ASXContext& context)
	:	m_context(context)
	{}

	//#################### PUBLIC OPERATORS ####################
public:
	R operator()(Arg0 value0, Arg1 value1)
	{
		int err = m_context.prepare();
		if(err < 0) throw ASXException("Error preparing script function context");

		ASXSetArgValue<Arg0>()(m_context, 0, value0);
		ASXSetArgValue<Arg1>()(m_context, 1, value1);

		err = m_context.execute();
		if(err < 0) throw ASXException("Error executing script function");

		return ASXGetReturnValue<R>()(m_context);
	}
};

// TODO: Functions with more arguments

#endif
