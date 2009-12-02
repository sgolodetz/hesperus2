/***
 * hesperus: PropFormatter.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <sstream>

#include <boost/lexical_cast.hpp>

namespace hesp {

//#################### MAIN TEMPLATE ####################
template <typename T>
std::string PropFormatter<T>::format(const T& input)
{
	// Default implementation which should be adequate for most simple types.
	return boost::lexical_cast<std::string>(input);
}

//#################### SPECIALIZATIONS ####################
template <typename K, typename V>
std::string PropFormatter<std::map<K,V> >::format(const std::map<K,V>& input)
{
	std::ostringstream os;

	os << '<';
	for(std::map<K,V>::const_iterator it=input.begin(), iend=input.end(); it!=iend;)
	{
		os << '(' << format_property(it->first) << ',' << format_property(it->second) << ')';
		++it;
		if(it != iend) os << ';';
	}
	os << '>';

	return os.str();
}

template <typename T>
std::string PropFormatter<std::set<T> >::format(const std::set<T>& input)
{
	std::ostringstream os;

	os << '[';
	for(typename std::set<T>::const_iterator it=input.begin(), iend=input.end(); it!=iend;)
	{
		os << format_property(*it);
		++it;
		if(it != iend) os << ',';
	}
	os << ']';

	return os.str();
}

template <typename T>
std::string PropFormatter<std::vector<T> >::format(const std::vector<T>& input)
{
	std::ostringstream os;

	os << '[';
	for(size_t i=0, size=input.size(); i<size; ++i)
	{
		os << format_property(input[i]);
		if(i < size-1) os << ',';
	}
	os << ']';

	return os.str();
}

//#################### HELPER FUNCTIONS ####################
template <typename T>
std::string format_property(const T& input)
{
	return PropFormatter<T>::format(input);
}

}
