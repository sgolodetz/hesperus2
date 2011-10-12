/***
 * hesperus: PropReader.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_PROPREADER
#define H_HESP_PROPREADER

#include <map>
#include <set>
#include <string>
#include <vector>

#include <hesp/math/vectors/Vector3.h>
#include <hesp/objects/base/ObjectID.h>
#include <hesp/physics/PhysicsMaterial.h>

namespace hesp {

//#################### HELPER CLASSES ####################
struct PropReaderUtil
{
	static std::string next_sized_token(std::string& input);
	static std::string next_token(std::string& input);
};

//#################### MAIN TEMPLATE ####################
template <typename T> struct PropReader
{
	static T read(std::string& input);
};

//#################### SPECIALIZATIONS ####################
template <typename K, typename V> struct PropReader<std::map<K,V> >
{
	static std::map<K,V> read(std::string& input);
};

template <> struct PropReader<ObjectID>
{
	static ObjectID read(std::string& input);
};

template <typename T1, typename T2> struct PropReader<std::pair<T1,T2> >
{
	static std::pair<T1,T2> read(std::string& input);
};

template <> struct PropReader<PhysicsMaterial>
{
	static PhysicsMaterial read(std::string& input);
};

template <typename T> struct PropReader<std::set<T> >
{
	static std::set<T> read(std::string& input);
};

template <> struct PropReader<std::string>
{
	static std::string read(std::string& input);
};

template <typename T> struct PropReader<std::vector<T> >
{
	static std::vector<T> read(std::string& input);
};

template <> struct PropReader<Vector3d>
{
	static Vector3d read(std::string& input);
};

//#################### HELPER FUNCTIONS ####################
template <typename T> T read_property(const std::string& value);

}

#include "PropReader.tpp"

#endif
