/***
 * asx: ASXGetReturnValue.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_ASX_ASXGETRETURNVALUE
#define H_ASX_ASXGETRETURNVALUE

#include "ASXContext.h"

//#################### MAIN TEMPLATE ####################
template <typename T> struct ASXGetReturnValue
{
	T operator()(const ASXContext& context) const
	{
		return *static_cast<T*>(context->GetReturnObject());
	}
};

//#################### SPECIALIZATIONS ####################
template <typename T> struct ASXGetReturnValue<T*>
{
	T *operator()(const ASXContext& context) const
	{
		return static_cast<T*>(context->GetReturnObject());
	}
};

template <> struct ASXGetReturnValue<double>
{ double operator()(const ASXContext& context) const { return context->GetReturnDouble(); } };

template <> struct ASXGetReturnValue<float>
{ float operator()(const ASXContext& context) const { return context->GetReturnFloat(); } };

template <> struct ASXGetReturnValue<int>
{ int operator()(const ASXContext& context) const { return context->GetReturnDWord(); } };

template <> struct ASXGetReturnValue<void>
{ void operator()(const ASXContext&) const {} };

// TODO: Other return value types - IMPORTANT!

#endif
