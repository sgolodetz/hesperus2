/***
 * asx: ASXEngine.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ASXException.h"
#include "ASXMethodTypeString.h"
#include "ASXStructorTypeString.h"

//#################### PUBLIC METHODS ####################
template <typename F>
void ASXEngine::register_constructor(F f, std::string obj, std::string decl)
{
	register_structor(f, asBEHAVE_CONSTRUCT, obj, decl);
}

template <typename F>
void ASXEngine::register_destructor(F f, std::string obj, std::string decl)
{
	register_structor(f, asBEHAVE_DESTRUCT, obj, decl);
}

template <typename F>
void ASXEngine::register_global_function(F f, const std::string& name)
{
	register_global_function<F>(f, name, ASXTypeString<F>(name)());
}

template <typename F>
void ASXEngine::register_global_function(F f, const std::string& name, const std::string& decl)
{
	int result = m_engine->RegisterGlobalFunction(decl.c_str(), asFUNCTION(f), asCALL_CDECL);
	if(result < 0) throw ASXException("Global function " + name + " could not be registered");
}

template <typename F>
void ASXEngine::register_global_operator(F f, asEBehaviours behaviour)
{
	register_global_operator<F>(f, behaviour, ASXTypeString<F>("f")());
}

template <typename F>
void ASXEngine::register_global_operator(F f, asEBehaviours behaviour, const std::string& decl)
{
	int result = m_engine->RegisterGlobalBehaviour(behaviour, decl.c_str(), asFUNCTION(f), asCALL_CDECL);
	if(result < 0) throw ASXException("Global operator could not be registered");
}

template <typename T, typename F>
void ASXEngine::register_instantiable_ref_type(F factory)
{
	register_instantiable_ref_type<T,F>(factory, ASXTypeString<T>()());
}

template <typename T, typename F>
void ASXEngine::register_instantiable_ref_type(F factory, const std::string& obj)
{
	register_uninstantiable_ref_type<T>();

	std::string factorySig = obj + "@ f()";
	int result = m_engine->RegisterObjectBehaviour(obj.c_str(), asBEHAVE_FACTORY, factorySig.c_str(), asFUNCTION(factory), asCALL_CDECL);
	if(result < 0) throw ASXException("Unable to register factory for reference type " + obj);
}

template <typename F>
void ASXEngine::register_object_method(F f, const std::string& name)
{
	ASXMethodTypeString<F> typeString(name);
	register_object_method(f, name, typeString.object_type(), typeString.function_type());
}

template <typename F>
void ASXEngine::register_object_method(F f, const std::string& name, const std::string& obj)
{
	ASXMethodTypeString<F> typeString(name);
	register_object_method(f, name, obj, typeString.function_type());
}

template <typename F>
void ASXEngine::register_object_method(F f, const std::string& name, const std::string& obj, const std::string& decl)
{
	int result = m_engine->RegisterObjectMethod(obj.c_str(), decl.c_str(), asSMethodPtr<sizeof(f)>::Convert(f), asCALL_THISCALL);
	if(result < 0) throw ASXException("Object method " + name + " could not be registered");
}

template <typename F>
void ASXEngine::register_object_operator(F f, asEBehaviours behaviour, std::string obj, std::string decl)
{
	if(obj == "" || decl == "")
	{
		ASXMethodTypeString<F> typeString("f");
		if(obj == "") obj = typeString.object_type();
		if(decl == "") decl = typeString.function_type();
	}

	int result = m_engine->RegisterObjectBehaviour(obj.c_str(), behaviour, decl.c_str(), asSMethodPtr<sizeof(f)>::Convert(f), asCALL_THISCALL);
	if(result < 0) throw ASXException("Object operator could not be registered");
}

template <typename T, typename M>
void ASXEngine::register_object_property(M T::*m, const std::string& name, std::string obj, std::string decl, int byteOffset)
{
	if(obj == "") obj = ASXTypeString<T>()();
	if(decl == "") decl = ASXTypeString<M>(name)();
	if(byteOffset == -1)
	{
		// Note: This is the equivalent of offsetof (but is technically not portable).
		byteOffset = (int)((ptrdiff_t)&(((T*)0)->*m));
	}

	int result = m_engine->RegisterObjectProperty(obj.c_str(), decl.c_str(), byteOffset);
	if(result < 0) throw ASXException("Object property " + name + " could not be registered");
}

template <typename T>
void ASXEngine::register_pod_type()
{
	register_pod_type<T>(ASXTypeString<T>()());
}

template <typename T>
void ASXEngine::register_pod_type(const std::string& obj)
{
	int result = m_engine->RegisterObjectType(obj.c_str(), sizeof(T), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS);
	if(result < 0) throw ASXException("Unable to register value type " + obj);
}

template <typename T>
void ASXEngine::register_uninstantiable_ref_type()
{
	register_uninstantiable_ref_type<T>(ASXTypeString<T>()());
}

template <typename T>
void ASXEngine::register_uninstantiable_ref_type(const std::string& obj)
{
	int result = m_engine->RegisterObjectType(obj.c_str(), 0, asOBJ_REF);
	if(result < 0) throw ASXException("Unable to register reference type " + obj);

	result = m_engine->RegisterObjectBehaviour(obj.c_str(), asBEHAVE_ADDREF, "void f()", asMETHOD(T,add_ref), asCALL_THISCALL);
	if(result < 0) throw ASXException("Unable to register addref for reference type " + obj);

	result = m_engine->RegisterObjectBehaviour(obj.c_str(), asBEHAVE_RELEASE, "void f()", asMETHOD(T,release), asCALL_THISCALL);
	if(result < 0) throw ASXException("Unable to register release for reference type " + obj);
}

//#################### PRIVATE METHODS ####################
template <typename F>
void ASXEngine::register_structor(F f, asEBehaviours behaviour, std::string obj, std::string decl)
{
	if(obj == "" || decl == "")
	{
		ASXStructorTypeString<F> structorTypeString;
		if(obj == "") obj = structorTypeString.object_type();
		if(decl == "") decl = structorTypeString.function_type();
	}

	int result = m_engine->RegisterObjectBehaviour(obj.c_str(), behaviour, decl.c_str(), asFUNCTION(f), asCALL_CDECL_OBJFIRST);
	if(result < 0) throw ASXException("'structor could not be registered");
}
