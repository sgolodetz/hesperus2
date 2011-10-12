/***
 * hcsg: main.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <iostream>
#include <string>
#include <vector>

#include <hesp/csg/CSGUtil.h>
#include <hesp/exceptions/Exception.h>
#include <hesp/io/files/BrushesFile.h>
#include <hesp/io/files/GeometryFile.h>
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
	std::cout << "Usage: hcsg {-r|-c} <input brushes> <output geometry>" << std::endl;
	exit(EXIT_FAILURE);
}

template <typename Poly>
void run_csg(const std::string& inputFilename, const std::string& outputFilename)
{
	typedef typename Poly::Vert Vert;
	typedef typename Poly::AuxData AuxData;

	// Read in the brushes.
	typedef PolyhedralBrush<Poly> PolyBrush;
	typedef shared_ptr<PolyBrush> PolyBrush_Ptr;
	typedef std::vector<PolyBrush_Ptr> PolyBrushVector;
	PolyBrushVector brushes = BrushesFile::load<Poly>(inputFilename);

	// Perform the CSG union.
	typedef shared_ptr<Poly> Poly_Ptr;
	typedef std::list<Poly_Ptr> PolyList;
	typedef shared_ptr<PolyList> PolyList_Ptr;
	PolyList_Ptr fragments = CSGUtil<Vert,AuxData>::union_all(brushes);

	// Write the polygons to disk.
	std::vector<Poly_Ptr> polygons(fragments->begin(), fragments->end());
	GeometryFile::save(outputFilename, polygons);
}

int main(int argc, char *argv[])
try
{
	if(argc != 4) quit_with_usage();
	std::vector<std::string> args(argv, argv + argc);

	if(args[1] == "-r") run_csg<TexturedPolygon>(args[2], args[3]);
	else if(args[1] == "-c") run_csg<CollisionPolygon>(args[2], args[3]);
	else quit_with_usage();

	return 0;
}
catch(Exception& e) { quit_with_error(e.cause()); }
