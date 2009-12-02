/***
 * hesperus: PropertyIO.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "PropertyIO.h"

#include <set>

#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;

#include <hesp/level/objects/base/ObjectID.h>
#include <hesp/math/vectors/Vector3.h>

namespace hesp {

//#################### LOADING METHODS ####################
void PropertyIO::load_property(Properties& properties, const std::string& name, const std::string& type, const std::string& value)
{
	typedef void (*LoadFunc)(Properties&, const std::string&, const std::string&);
	static std::map<std::string,LoadFunc> loadFuncs;
	static bool done = false;
	if(!done)
	{
		loadFuncs["double"] = load_typed_property<double>;
		loadFuncs["int"] = load_typed_property<int>;
		loadFuncs["ObjectID"] = load_typed_property<ObjectID>;
		loadFuncs["PhysicsMaterial"] = load_typed_property<PhysicsMaterial>;
		loadFuncs["string"] = load_typed_property<std::string>;
		loadFuncs["Vector3d"] = load_typed_property<Vector3d>;
		loadFuncs["[int]"] = load_typed_property<std::vector<int> >;
		loadFuncs["[string]"] = load_typed_property<std::vector<std::string> >;
		loadFuncs["{ObjectID}"] = load_typed_property<std::set<ObjectID> >;
		loadFuncs["string -> int"] = load_typed_property<std::map<std::string,int> >;
		loadFuncs["string -> (string -> Vector3d)"] = load_typed_property<std::map<std::string,std::map<std::string,Vector3d> > >;
		done = true;
	}

	try
	{
		std::map<std::string,LoadFunc>::iterator loadFunc = loadFuncs.find(type);
		if(loadFunc != loadFuncs.end()) (*(loadFunc->second))(properties, name, value);
		else throw Exception("The type " + type + " is not currently supported");
	}
	catch(bad_lexical_cast&)
	{
		throw Exception("The value " + value + " for " + name + " was not of the right type");
	}
}

//#################### SAVING METHODS ####################
void PropertyIO::save_property(std::ostream& os, const Properties& properties, const std::string& name, const std::string& type)
{
	typedef void (*SaveFunc)(std::ostream&, const Properties&, const std::string&);
	static std::map<std::string,SaveFunc> saveFuncs;
	static bool done = false;
	if(!done)
	{
		saveFuncs["double"] = save_typed_property<double>;
		saveFuncs["int"] = save_typed_property<int>;
		saveFuncs["ObjectID"] = save_typed_property<ObjectID>;
		saveFuncs["PhysicsMaterial"] = save_typed_property<PhysicsMaterial>;
		saveFuncs["string"] = save_typed_property<std::string>;
		saveFuncs["Vector3d"] = save_typed_property<Vector3d>;
		saveFuncs["[int]"] = save_typed_property<std::vector<int> >;
		saveFuncs["[string]"] = save_typed_property<std::vector<std::string> >;
		saveFuncs["{ObjectID}"] = save_typed_property<std::set<ObjectID> >;
		saveFuncs["string -> int"] = save_typed_property<std::map<std::string,int> >;
		saveFuncs["string -> (string -> Vector3d)"] = save_typed_property<std::map<std::string,std::map<std::string,Vector3d> > >;
		done = true;
	}

	std::map<std::string,SaveFunc>::iterator saveFunc = saveFuncs.find(type);
	if(saveFunc != saveFuncs.end()) (*(saveFunc->second))(os, properties, name);
	else throw Exception("The type " + type + " is not currently supported");
}

}
