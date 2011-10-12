/***
 * asx: ASXModule.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_ASX_ASXMODULE
#define H_ASX_ASXMODULE

#include <string>

#include <boost/shared_ptr.hpp>

#include <angelscript.h>

#include "ASXFunction.h"
#include "ASXVariable.h"

class ASXModule
{
	//#################### PRIVATE VARIABLES ####################
private:
	asIScriptModule *m_module;

	//#################### CONSTRUCTORS ####################
public:
	explicit ASXModule(asIScriptModule *module);

	//#################### PUBLIC METHODS ####################
public:
	template <typename F> ASXFunction<F> get_global_function(const std::string& name, const ASXFunction<F>&) const;
	template <typename F> ASXFunction<F> get_global_function_ex(const std::string& decl, const ASXFunction<F>&) const;
	template <typename T> T& get_global_variable(const std::string& name) const;
	template <typename T> ASXVariable<T> get_global_variable(const std::string& name, const ASXVariable<T>&) const;
	template <typename T> T& get_global_variable_ex(const std::string& decl) const;
	template <typename T> ASXVariable<T> get_global_variable_ex(const std::string& decl, const ASXVariable<T>&) const;
};

//#################### TYPEDEFS ####################
typedef boost::shared_ptr<ASXModule> ASXModule_Ptr;
typedef boost::shared_ptr<const ASXModule> ASXModule_CPtr;

#include "ASXModule.tpp"

#endif
