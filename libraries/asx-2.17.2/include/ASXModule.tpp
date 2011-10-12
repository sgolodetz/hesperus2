/***
 * asx: ASXModule.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ASXTypeString.h"

//#################### PUBLIC METHODS ####################
template <typename F>
ASXFunction<F> ASXModule::get_global_function(const std::string& name, const ASXFunction<F>&) const
{
	std::string decl = ASXTypeString<F>(name)();
	int funcID = m_module->GetFunctionIdByDecl(decl.c_str());
	if(funcID < 0) throw ASXException("Could not find function with declaration " + decl);
	asIScriptContext *context = m_module->GetEngine()->CreateContext();
	return ASXFunction<F>(ASXContext(context, funcID));
}

template <typename F>
ASXFunction<F> ASXModule::get_global_function_ex(const std::string& decl, const ASXFunction<F>&) const
{
	int funcID = m_module->GetFunctionIdByDecl(decl.c_str());
	if(funcID < 0) throw ASXException("Could not find function with declaration " + decl);
	asIScriptContext *context = m_module->GetEngine()->CreateContext();
	return ASXFunction<F>(ASXContext(context, funcID));
}

template <typename T>
T& ASXModule::get_global_variable(const std::string& name) const
{
	std::string decl = ASXTypeString<T>(name)();
	return *static_cast<T*>(m_module->GetAddressOfGlobalVar(m_module->GetGlobalVarIndexByDecl(decl.c_str())));
}

template <typename T>
ASXVariable<T> ASXModule::get_global_variable(const std::string& name, const ASXVariable<T>&) const
{
	std::string decl = ASXTypeString<T>(name)();
	T *obj = static_cast<T*>(m_module->GetAddressOfGlobalVar(m_module->GetGlobalVarIndexByDecl(decl.c_str())));
	return ASXVariable<T>(obj);
}

template <typename T>
T& ASXModule::get_global_variable_ex(const std::string& decl) const
{
	return *static_cast<T*>(m_module->GetAddressOfGlobalVar(m_module->GetGlobalVarIndexByDecl(decl.c_str())));
}

template <typename T>
ASXVariable<T> ASXModule::get_global_variable_ex(const std::string& decl, const ASXVariable<T>&) const
{
	T *obj = static_cast<T*>(m_module->GetAddressOfGlobalVar(m_module->GetGlobalVarIndexByDecl(decl.c_str())));
	return ASXVariable<T>(obj);
}
