/***
 * hesperus: LightsSection.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "LightsSection.h"

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <hesp/exceptions/Exception.h>
#include <hesp/io/util/LineIO.h>

namespace hesp {

//#################### LOADING METHODS ####################
/**
Loads an array of lights from the specified std::istream.

@param is	The std::istream
@return		The array of lights
*/
std::vector<Light> LightsSection::load(std::istream& is)
{
	std::vector<Light> lights;

	std::string line;

	LineIO::read_checked_line(is, "Lights");
	LineIO::read_checked_line(is, "{");

	// Read in the light count.
	int lightCount;
	try
	{
		LineIO::read_line(is, line, "light count");
		lightCount = lexical_cast<int>(line);
		lights.reserve(lightCount);
	}
	catch(bad_lexical_cast&) { throw Exception("The light count was not an integer"); }

	// Read in the lights, one per line.
	for(int i=0; i<lightCount; ++i)
	{
		if(!LineIO::portable_getline(is, line))
		{
			std::ostringstream oss;
			oss << "Unexpected EOF at line " << i << " in the lights file";
			throw Exception(oss.str());
		}

		// Parse the line.
		typedef boost::char_separator<char> sep;
		typedef boost::tokenizer<sep> tokenizer;
		tokenizer tok(line.begin(), line.end(), sep(" "));
		std::vector<std::string> tokens(tok.begin(), tok.end());

		if(tokens.size() != 11) throw Exception("Bad light data at line " + lexical_cast<std::string>(i));

		std::vector<std::string> positionComponents(&tokens[1], &tokens[4]);
		std::vector<std::string> colourComponents(&tokens[6], &tokens[9]);

		Vector3d position(positionComponents);
		Colour3d colour(colourComponents);
		double falloffRadius;
		try							{ falloffRadius = lexical_cast<double>(tokens[10]); }
		catch(bad_lexical_cast&)	{ throw Exception("The falloff radius was not a number at line " + lexical_cast<std::string>(i)); }
		lights.push_back(Light(position, colour, falloffRadius));
	}

	LineIO::read_checked_line(is, "}");

	return lights;
}

//#################### SAVING METHODS ####################
/**
Saves an array of lights to the specified std::ostream.

@param os		The std::ostream
@param lights	The lights
*/
void LightsSection::save(std::ostream& os, const std::vector<Light>& lights)
{
	os << "Lights\n";
	os << "{\n";

	int lightCount = static_cast<int>(lights.size());
	os << lightCount << '\n';

	for(int i=0; i<lightCount; ++i)
	{
		os << lights[i].position << ' ' << lights[i].colour << ' ' << lights[i].falloffRadius << '\n';
	}

	os << "}\n";
}

}
