/***
 * hvis: main.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include <iostream>
#include <string>
#include <vector>

#include <hesp/io/files/PortalsFile.h>
#include <hesp/io/files/VisFile.h>
#include <hesp/level/vis/VisCalculator.h>
using namespace hesp;

//#################### FUNCTIONS ####################
void quit_with_error(const std::string& error)
{
	std::cout << "Error: " << error << std::endl;
	exit(EXIT_FAILURE);
}

void quit_with_usage()
{
	std::cout << "Usage: hvis <input filename> <output filename>" << std::endl;
	exit(EXIT_FAILURE);
}

void run_calculator(const std::string& inputFilename, const std::string& outputFilename)
try
{
	// Read in the empty leaf count and portals.
	int emptyLeafCount;
	std::vector<Portal_Ptr> portals;
	PortalsFile::load(inputFilename, emptyLeafCount, portals);

	// Run the visibility calculator.
	VisCalculator visCalc(emptyLeafCount, portals);
	LeafVisTable_Ptr leafVis = visCalc.calculate_leaf_vis_table();

	// Write the leaf visibility table to the output file.
	VisFile::save(outputFilename, leafVis);
}
catch(Exception& e) { quit_with_error(e.cause()); }

int main(int argc, char *argv[])
{
	if(argc != 3) quit_with_usage();
	std::vector<std::string> args(argv, argv + argc);
	run_calculator(args[1], args[2]);
	return 0;
}
