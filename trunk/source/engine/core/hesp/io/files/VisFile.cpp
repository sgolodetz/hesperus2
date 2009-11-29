/***
 * hesperus: VisFile.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "VisFile.h"

#include <fstream>

#include <hesp/exceptions/Exception.h>
#include <hesp/io/sections/VisSection.h>

namespace hesp {

//#################### LOADING METHODS ####################
/**
Loads a leaf visibility table from the specified file.

@param filename	The name of the file from which to load the visibility table
@return			The visibility table
*/
LeafVisTable_Ptr VisFile::load(const std::string& filename)
{
	std::ifstream is(filename.c_str(), std::ios_base::binary);
	if(is.fail()) throw Exception("The vis file could not be read");
	return VisSection::load(is);
}

//#################### SAVING METHODS ####################
/**
Saves a vis table file.

@param filename	The name of the file to which to save the visibility table
@param leafVis	The visibility table
*/
void VisFile::save(const std::string& filename, const LeafVisTable_CPtr& leafVis)
{
	std::ofstream os(filename.c_str(), std::ios_base::binary);
	if(os.fail()) throw Exception("Could not open " + filename + " for writing");
	VisSection::save(os, leafVis);
}

}
