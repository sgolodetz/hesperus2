/***
 * asx: ASXEngine.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_ASX_ASXENGINE
#define H_ASX_ASXENGINE

#include <vector>

#include "ASXMessage.h"
#include "ASXModule.h"

class ASXEngine
{
	//#################### PRIVATE VARIABLES ####################
private:
	asIScriptEngine *m_engine;
	std::vector<ASXMessage> m_messages;

	//#################### CONSTRUCTORS ####################
public:
	ASXEngine();

	//#################### DESTRUCTOR ####################
public:
	~ASXEngine();

	//#################### COPY CONSTRUCTOR & ASSIGNMENT OPERATOR ####################
private:
	ASXEngine(const ASXEngine&);
	ASXEngine& operator=(const ASXEngine&);

	//#################### PUBLIC METHODS ####################
public:
	void clear_messages();

	ASXModule_Ptr get_module(const std::string& moduleName) const;

	bool load_and_build_script(const std::string& filename, const std::string& moduleName);

	const std::vector<ASXMessage>& messages() const;

	void output_messages(std::ostream& os) const;

	template <typename F> void register_constructor(F f, std::string obj = "", std::string decl = "");

	template <typename F> void register_destructor(F f, std::string obj = "", std::string decl = "");

	template <typename F> void register_global_function(F f, const std::string& name);
	template <typename F> void register_global_function(F f, const std::string& name, const std::string& decl);

	template <typename T, typename F> void register_instantiable_ref_type(F factory);
	template <typename T, typename F> void register_instantiable_ref_type(F factory, const std::string& obj);

	template <typename F> void register_object_method(F f, const std::string& name);
	template <typename F> void register_object_method(F f, const std::string& name, const std::string& obj);
	template <typename F> void register_object_method(F f, const std::string& name, const std::string& obj, const std::string& decl);

	template <typename F> void register_object_operator(F f, asEBehaviours behaviour, std::string obj = "", std::string decl = "");

	template <typename T, typename M> void register_object_property(M T::*m, const std::string& name, std::string obj = "", std::string decl = "", int byteOffset = -1);

	template <typename T> void register_pod_type();
	template <typename T> void register_pod_type(const std::string& obj);

	template <typename T> void register_uninstantiable_ref_type();
	template <typename T> void register_uninstantiable_ref_type(const std::string& obj);

	//#################### PRIVATE METHODS ####################
private:
	void message_callback(const asSMessageInfo *msg);
	template <typename F> void register_structor(F f, asEBehaviours behaviour, std::string obj = "", std::string decl = "");
};

//#################### TYPEDEFS ####################
typedef boost::shared_ptr<ASXEngine> ASXEngine_Ptr;
typedef boost::shared_ptr<const ASXEngine> ASXEngine_CPtr;

#include "ASXEngine.tpp"

#endif
