/***
 * hesperus: LightmapsSection.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "LightmapsSection.h"

#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <hesp/exceptions/Exception.h>
#include <hesp/images/PNGLoader.h>
#include <hesp/images/PNGSaver.h>
#include <hesp/io/util/LineIO.h>

namespace hesp {

//#################### LOADING METHODS ####################
/**
Loads an array of lightmaps from the specified std::istream.

@param is			The std::istream
@return				The lightmaps
@throws Exception	If EOF is encountered whilst trying to read the lightmaps
*/
std::vector<Image24_Ptr> LightmapsSection::load(std::istream& is)
{
	std::vector<Image24_Ptr> lightmaps;

	LineIO::read_checked_line(is, "Lightmaps");
	LineIO::read_checked_line(is, "{");

	std::string line;
	LineIO::read_line(is, line, "lightmap count");
	int lightmapCount;
	try							{ lightmapCount = lexical_cast<int,std::string>(line); }
	catch(bad_lexical_cast&)	{ throw Exception("The lightmap count was not an integer"); }

	lightmaps.resize(lightmapCount);
	for(int i=0; i<lightmapCount; ++i)
	{
		lightmaps[i] = PNGLoader::load_streamed_image24(is);
	}

	if(is.get() != '\n') throw Exception("Expected newline after lightmaps");

	LineIO::read_checked_line(is, "}");

	return lightmaps;
}

//#################### SAVING METHODS ####################
/**
Saves an array of lightmaps to the specified std::ostream.

@param os			The std::ostream
@param lightmaps	The lightmaps
*/
void LightmapsSection::save(std::ostream& os, const std::vector<Image24_Ptr>& lightmaps)
{
	os << "Lightmaps\n";
	os << "{\n";

	int lightmapCount = static_cast<int>(lightmaps.size());
	os << lightmapCount << '\n';

	for(int i=0; i<lightmapCount; ++i)
	{
		PNGSaver::save_streamed_image24(os, lightmaps[i]);
	}

	os << "\n}\n";
}

}
