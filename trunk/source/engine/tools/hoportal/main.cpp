/***
 * hoportal: main.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <iostream>
#include <string>
#include <vector>

#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <hesp/io/files/OnionPortalsFile.h>
#include <hesp/io/files/OnionTreeFile.h>
#include <hesp/portals/OnionPortalGenerator.h>
#include <hesp/trees/OnionTree.h>
#include <hesp/util/PolygonTypes.h>
using namespace hesp;

//#################### FUNCTIONS ####################
void quit_with_error(const std::string& error)
{
	std::cout << "Error: " << error << std::endl;
	exit(EXIT_FAILURE);
}

void quit_with_usage()
{
	std::cout << "Usage: hoportal {-r|-c} <input filename> <output filename>" << std::endl;
	exit(EXIT_FAILURE);
}

template <typename Poly>
void run_generator(const std::string& inputFilename, const std::string& outputFilename)
{
	typedef shared_ptr<Poly> Poly_Ptr;
	typedef std::vector<Poly_Ptr> PolyVector;

	// Read in the polygons and onion tree.
	PolyVector polygons;
	OnionTree_Ptr tree;
	OnionTreeFile::load(inputFilename, polygons, tree);

	// Generate the onion portals.
	shared_ptr<std::list<OnionPortal_Ptr> > portals = OnionPortalGenerator().generate_portals(tree);

	// Save the onion portals to the output file.
	std::vector<OnionPortal_Ptr> vec(portals->begin(), portals->end());
	OnionPortalsFile::save(outputFilename, vec);
}

int main(int argc, char *argv[])
try
{
	if(argc != 4) quit_with_usage();

	const std::vector<std::string> args(argv, argv+argc);

	std::string inputFilename = args[2];
	std::string outputFilename = args[3];

	if(args[1] == "-r") run_generator<TexturedPolygon>(inputFilename, outputFilename);
	else if(args[1] == "-c") run_generator<CollisionPolygon>(inputFilename, outputFilename);
	else quit_with_usage();

	return 0;
}
catch(Exception& e) { quit_with_error(e.cause()); }
