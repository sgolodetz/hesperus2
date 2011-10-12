/***
 * hesperus: PropFormatter.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_PROPFORMATTER
#define H_HESP_PROPFORMATTER

#include <map>
#include <set>
#include <string>
#include <vector>

#include <hesp/objects/base/ObjectID.h>
#include <hesp/physics/PhysicsMaterial.h>

namespace hesp {

//#################### MAIN TEMPLATE ####################
template <typename T> struct PropFormatter
{
	static std::string format(const T& input);
};

//#################### SPECIALIZATIONS ####################
template <> struct PropFormatter<std::string>
{
	static std::string format(const std::string& input);
};

template <typename K, typename V> struct PropFormatter<std::map<K,V> >
{
	static std::string format(const std::map<K,V>& input);
};

template <> struct PropFormatter<ObjectID>
{
	static std::string format(const ObjectID& input);
};

template <> struct PropFormatter<PhysicsMaterial>
{
	static std::string format(PhysicsMaterial input);
};

template <typename T> struct PropFormatter<std::set<T> >
{
	static std::string format(const std::set<T>& input);
};

template <typename T> struct PropFormatter<std::vector<T> >
{
	static std::string format(const std::vector<T>& input);
};

//#################### HELPER FUNCTIONS ####################
template <typename T> std::string format_property(const T& input);

}

#include "PropFormatter.tpp"

#endif
