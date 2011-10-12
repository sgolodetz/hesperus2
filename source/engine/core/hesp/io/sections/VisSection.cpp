/***
 * hesperus: VisSection.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "VisSection.h"

#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <hesp/exceptions/Exception.h>
#include <hesp/io/util/LineIO.h>

namespace hesp {

//#################### LOADING METHODS ####################
/**
Loads a leaf visibility table from the specified std::istream.

@param is	The std::istream
@return		The visibility table
*/
LeafVisTable_Ptr VisSection::load(std::istream& is)
{
	LeafVisTable_Ptr leafVis;

	std::string line;

	LineIO::read_checked_line(is, "VisTable");
	LineIO::read_checked_line(is, "{");

	// Read in the size of the vis table.
	LineIO::read_line(is, line, "vis table size");
	int size;
	try							{ size = lexical_cast<int>(line); }
	catch(bad_lexical_cast&)	{ throw Exception("The vis table size was not an integer"); }

	// Construct an empty vis table of the right size.
	leafVis.reset(new LeafVisTable(size));

	// Read in the vis table itself.
	for(int i=0; i<size; ++i)
	{
		LineIO::read_line(is, line, "vis table row " + lexical_cast<std::string>(i));
		if(line.length() != size) throw Exception("Bad vis table row " + lexical_cast<std::string>(i));

		for(int j=0; j<size; ++j)
		{
			if(line[j] == '0') (*leafVis)(i,j) = LEAFVIS_NO;
			else if(line[j] == '1') (*leafVis)(i,j) = LEAFVIS_YES;
			else throw Exception("Bad vis table value in row " + lexical_cast<std::string>(i));
		}
	}

	LineIO::read_checked_line(is, "}");

	return leafVis;
}

//#################### SAVING METHODS ####################
/**
Saves a leaf visibility table to a std::ostream.

@param os		The std::ostream
@param leafVis	The leaf visibility table
*/
void VisSection::save(std::ostream& os, const LeafVisTable_CPtr& leafVis)
{
	const LeafVisTable& table = *leafVis;

	os << "VisTable\n";
	os << "{\n";

	int size = table.size();
	os << size << '\n';
	for(int i=0; i<size; ++i)
	{
		for(int j=0; j<size; ++j)
		{
			switch(table(i,j))
			{
				case LEAFVIS_NO:
				{
					os << '0';
					break;
				}
				case LEAFVIS_YES:
				{
					os << '1';
					break;
				}
			}
		}
		os << '\n';
	}

	os << "}\n";
}

}
