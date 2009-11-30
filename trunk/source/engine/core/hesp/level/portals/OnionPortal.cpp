/***
 * hesperus: OnionPortal.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "OnionPortal.h"

#include <ostream>

#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

namespace hesp {

//#################### GLOBAL OPERATORS ####################
std::ostream& operator<<(std::ostream& os, const OnionPortalInfo& rhs)
{
	os << "[ ";
	os << rhs.fromLeaf << ' ' << rhs.toLeaf << ' ';

	// Output the indices of the maps in which the portal appears.
	os << "{ ";
	for(std::set<int>::const_iterator it=rhs.mapIndices.begin(), iend=rhs.mapIndices.end(); it!=iend; ++it)
	{
		os << *it << ' ';
	}
	os << "}";

	os << " ]";
	return os;
}

std::istream& operator>>(std::istream& is, OnionPortalInfo& rhs)
{
	is >> std::skipws;

	std::string temp;
	is >> temp >> rhs.fromLeaf >> rhs.toLeaf >> temp;	// read [ fromLeaf toLeaf {
	for(;;)
	{
		is >> temp;
		if(temp == "}") break;

		try	
		{
			int mapIndex = lexical_cast<int>(temp);
			rhs.mapIndices.insert(mapIndex);
		}
		catch(bad_lexical_cast&)
		{
			throw Exception("One of the map indices was not an integer");
		}
	}
	is >> temp;		// read the trailing ]

	is >> std::noskipws;
	return is;
}

}
