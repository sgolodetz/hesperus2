/***
 * hesperus: PropReader.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <boost/lexical_cast.hpp>

#include <PropSimplifier.h>

namespace hesp {

//#################### MAIN TEMPLATE ####################
template <typename T>
T PropReader<T>::read(std::string& input)
{
	// Default implementation which should be adequate for most simple types.
	return boost::lexical_cast<T>(PropReaderUtil::next_token(input));
}

//#################### SPECIALIZATIONS ####################
template <typename K, typename V>
std::map<K,V> PropReader<std::map<K,V> >::read(std::string& input)
{
	std::map<K,V> ret;

	std::string tok = PropReaderUtil::next_sized_token(input);
	while(!tok.empty())
	{
		ret.insert(PropReader<std::pair<K,V> >::read(tok));
	}

	return ret;
}

template <typename T1, typename T2>
std::pair<T1,T2> PropReader<std::pair<T1,T2> >::read(std::string& input)
{
	std::string tok = PropReaderUtil::next_sized_token(input);
	T1 t1 = PropReader<T1>::read(tok);
	T2 t2 = PropReader<T2>::read(tok);
	return std::make_pair(t1, t2);
}

template <typename T>
std::set<T> PropReader<std::set<T> >::read(std::string& input)
{
	std::set<T> ret;

	std::string tok = PropReaderUtil::next_sized_token(input);
	while(!tok.empty())
	{
		ret.insert(PropReader<T>::read(tok));
	}

	return ret;
}

template <typename T>
std::vector<T> PropReader<std::vector<T> >::read(std::string& input)
{
	std::vector<T> ret;

	std::string tok = PropReaderUtil::next_sized_token(input);
	while(!tok.empty())
	{
		ret.push_back(PropReader<T>::read(tok));
	}

	return ret;
}

//#################### HELPER FUNCTIONS ####################
template <typename T>
T read_property(const std::string& value)
{
	std::string input = propparser::simplify_property(value);
	return PropReader<T>::read(input);
}

}
