/***
 * test-boost_1_41_0: main.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <boost/shared_ptr.hpp>
#include <boost/filesystem/operations.hpp>
using boost::shared_ptr;
using boost::filesystem::path;

int main()
{
	shared_ptr<path> blah(new path("./blah"));
	return 0;
}
