/***
 * asx: ASXSetArgValue.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_ASX_ASXSETARGVALUE
#define H_ASX_ASXSETARGVALUE

#include "ASXContext.h"

//#################### MAIN TEMPLATE ####################
template <typename T> struct ASXSetArgValue
{ void operator()(const ASXContext& context, int arg, T& value) const { context->SetArgObject(arg, &value); } };

//#################### SPECIALIZATIONS ####################
template <typename T> struct ASXSetArgValue<T*>
{ void operator()(const ASXContext& context, int arg, T *value) const { context->SetArgObject(arg, value); } };

template <> struct ASXSetArgValue<double>
{ void operator()(const ASXContext& context, int arg, double value) const { context->SetArgDouble(arg, value); } };

template <> struct ASXSetArgValue<float>
{ void operator()(const ASXContext& context, int arg, float value) const { context->SetArgFloat(arg, value); } };

template <> struct ASXSetArgValue<int>
{ void operator()(const ASXContext& context, int arg, int value) const { context->SetArgDWord(arg, value); } };

// TODO: Other argument types - IMPORTANT!

#endif
