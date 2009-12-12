/***
 * hobsp: main.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <iostream>
#include <string>
#include <vector>

#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <hesp/io/files/GeometryFile.h>
#include <hesp/io/files/OnionTreeFile.h>
#include <hesp/io/files/TreeFile.h>
#include <hesp/trees/OnionCompiler.h>
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
	std::cout << "Usage: hobsp {-r|-c} <input geom 1> <input tree 1> ... <input geom n> <input tree n> <output tree> [-w<number>]" << std::endl;
	exit(EXIT_FAILURE);
}

template <typename Poly>
void run_compiler(const std::vector<std::string>& geomFilenames, const std::vector<std::string>& treeFilenames,
				  const std::string& outputFilename, double weight)
{
	typedef shared_ptr<Poly> Poly_Ptr;
	typedef std::vector<Poly_Ptr> PolyVector;

	// Read in the input maps.
	size_t mapCount = geomFilenames.size();
	std::vector<PolyVector> maps(mapCount);
	for(size_t i=0; i<mapCount; ++i)
	{
		GeometryFile::load(geomFilenames[i], maps[i]);
	}

	// Read in the input trees.
	std::vector<BSPTree_CPtr> mapTrees;
	for(size_t i=0; i<mapCount; ++i)	// note: there's guaranteed to be exactly one tree per map
	{
		PolyVector polygons;
		BSPTree_Ptr mapTree;
		TreeFile::load(treeFilenames[i], polygons, mapTree);
		mapTrees.push_back(mapTree);
	}

	// Compile them into an onion tree.
	OnionCompiler<Poly> compiler(maps, mapTrees, weight);
	compiler.build_tree();

	// Write the output polygons and onion tree to disk.
	OnionTreeFile::save(outputFilename, *compiler.polygons(), compiler.tree());
}

int main(int argc, char *argv[])
try
{
	std::vector<std::string> args(argv, argv + argc);

	// If an optional weight argument has been supplied, parse it and remove it to simplify further processing.
	double weight = 4;
	const std::string& lastArg = args[args.size()-1];
	if(lastArg.length() >= 3 && lastArg.substr(0,2) == "-w")
	{
		try							{ weight = lexical_cast<double>(lastArg.substr(2)); }
		catch(bad_lexical_cast&)	{ quit_with_usage(); }

		args.pop_back();
	}

	// If we don't have (at a minimum) 'hobsp {-r|-c} <input geom 1> <input tree 1> <output tree>', then the command is ill-formed.
	if(args.size() < 5) quit_with_usage();

	std::vector<std::string> geomFilenames;
	std::vector<std::string> treeFilenames;
	int argCount = static_cast<int>(args.size());
	if(argCount % 2 == 0) quit_with_usage();		// we should have n (geom,tree) pairs plus an output tree
	for(int i=2; i<argCount-1; i+=2)
	{
		geomFilenames.push_back(args[i]);
		treeFilenames.push_back(args[i+1]);
	}
	const std::string& outputFilename = args[argCount-1];

	size_t len = outputFilename.length();
	if(len > 4)
	{
		std::string outputExtension = outputFilename.substr(len-4);
		if(outputExtension == ".cg2" || outputExtension == ".ct2")
		{
			quit_with_error("The extension " + outputExtension + " is disallowed for the output tree filename to help prevent errors - sorry!");
		}
	}

	if(args[1] == "-r") run_compiler<TexturedPolygon>(geomFilenames, treeFilenames, outputFilename, weight);
	else if(args[1] == "-c") run_compiler<CollisionPolygon>(geomFilenames, treeFilenames, outputFilename, weight);
	else quit_with_usage();

	return 0;
}
catch(Exception& e) { quit_with_error(e.cause()); }
