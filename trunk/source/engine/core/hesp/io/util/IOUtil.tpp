/***
 * hesperus: IOUtil.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "LineIO.h"

namespace hesp {

//#################### READING METHODS ####################
/**
Reads a sequence of polygons of known length from a std::istream, one per line.
The polygon count is stored on a separate line before the polygons.

@param is		The std::istream
@param polygons	The std::vector into which to write the read polygons
*/
template <typename Poly>
void IOUtil::read_counted_polygons(std::istream& is, std::vector<shared_ptr<Poly> >& polygons)
{
	std::string line;

	LineIO::read_line(is, line, "polygon count");
	try
	{
		int polyCount = boost::lexical_cast<int,std::string>(line);
		read_polygons(is, polygons, polyCount);
	}
	catch(boost::bad_lexical_cast&) { throw Exception("The polygon count is not an integer"); }
}

/**
Reads a polygon from a std::string (generally a line of text taken from a file).

@param line	The std::string containing the polygon definition
@param n	The number of the line in the file (if available), as a std::string
@return		The polygon
*/
template <typename Vert, typename AuxData>
shared_ptr<Polygon<Vert,AuxData> > IOUtil::read_polygon(const std::string& line, const std::string& n)
{
	typedef Polygon<Vert,AuxData> Poly;
	typedef shared_ptr<Poly> Poly_Ptr;

	// Read the vertex count.
	std::string::size_type L = line.find(' ');
	if(L == std::string::npos) throw Exception("Bad input on line " + n);
	std::string vertCountString = line.substr(0,L);
	int vertCount;
	try								{ vertCount = boost::lexical_cast<int,std::string>(vertCountString); }
	catch(boost::bad_lexical_cast&)	{ throw Exception("Bad vertex count on line " + n); }

	// Read the auxiliary data.
	std::string::size_type R = line.find_last_of(')');
	if(R == std::string::npos || R+2 >= line.length()) throw Exception("Bad input on line " + n);
	std::string auxDataString = line.substr(R+2);
	boost::trim(auxDataString);
	AuxData auxData;
	try								{ auxData = boost::lexical_cast<AuxData,std::string>(auxDataString); }
	catch(boost::bad_lexical_cast&)	{ throw Exception("Bad auxiliary data on line " + n); }

	// Read the vertices.
	std::string verticesString = line.substr(L+1, R-L);
	std::vector<Vert> vertices;

	typedef boost::char_separator<char> sep;
	typedef boost::tokenizer<sep> tokenizer;
	tokenizer tok(verticesString.begin(), verticesString.end(), sep(" "));
	std::vector<std::string> tokens(tok.begin(), tok.end());
	int tokensPerVert = static_cast<int>(tokens.size()) / vertCount;
	if(tokensPerVert < 3) throw Exception("Bad vertex data on line " + n);

	for(int i=0; i<vertCount; ++i)
	{
		int offset = i*tokensPerVert;
		if(tokens[offset] != "(" || tokens[offset+tokensPerVert-1] != ")") throw Exception("Bad vertex data on line " + n);

		std::vector<std::string> components(&tokens[offset+1], &tokens[offset+tokensPerVert-1]);
		try					{ vertices.push_back(Vert(components)); }
		catch(Exception& e)	{ throw Exception(e.cause()); }
	}

	return Poly_Ptr(new Poly(vertices, auxData));
}

/**
Reads a sequence of polygons from a stream, one per line.

@param is		The stream from which to load
@param polygons	The std::vector into which to write the loaded polygons
*/
template <typename Vert, typename AuxData>
void IOUtil::read_polygons(std::istream& is, std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons, int maxToRead)
{
	typedef Polygon<Vert,AuxData> Poly;
	typedef shared_ptr<Poly> Poly_Ptr;

	std::string line;
	int n = 1;
	while(std::getline(is, line))
	{
		boost::trim(line);
		if(line != "")
		{
			polygons.push_back(read_polygon<Vert,AuxData>(line, boost::lexical_cast<std::string,int>(n)));
		}

		++n;
		if(n > maxToRead) break;
	}
}

/**
Reads a polyhedral brush from a std::istream.

@param is			The std::istream
@return				The polyhedral brush, or NULL if EOF was encountered
@throws Exception	If anything else goes wrong whilst trying to read the polyhedral brush
*/
template <typename Poly>
shared_ptr<PolyhedralBrush<Poly> > IOUtil::read_polyhedral_brush(std::istream& is)
{
	typedef PolyhedralBrush<Poly> PolyBrush;
	typedef shared_ptr<PolyBrush> PolyBrush_Ptr;

	std::string line;

	if(!std::getline(is,line)) return PolyBrush_Ptr();
	if(line != "{") throw Exception("Expected {");

	// Read brush function.
	LineIO::read_line(is, line, "brush function");
	BrushFunction function = boost::lexical_cast<BrushFunction,std::string>(line);

	// Read bounds.
	LineIO::read_line(is, line, "bounds");
	AABB3d bounds = read_aabb<Vector3d>(line);

	// Read faces.
	typedef shared_ptr<Poly> Poly_Ptr;
	std::vector<Poly_Ptr> faces;
	read_counted_polygons(is, faces);

	LineIO::read_checked_line(is, "}");

	return PolyBrush_Ptr(new PolyBrush(bounds, faces, function));
}

/**
Reads a sequence of polygons of unknown length from a std::istream, one per line.

@param is		The std::istream
@param polygons	The std::vector into which to write the read polygons
*/
template <typename Poly>
void IOUtil::read_uncounted_polygons(std::istream& is, std::vector<shared_ptr<Poly> >& polygons)
{
	return read_polygons(is, polygons, INT_MAX);
}

//#################### SAVING SUPPORT METHODS ####################
/**
Writes a sequence of polygons to a stream, one per line.

@param os			The stream to which to write
@param polygons		The polygons to write to the stream
@param writeCount	Whether the polygon count should be output to the stream first
*/
template <typename Vert, typename AuxData>
void IOUtil::write_polygons(std::ostream& os, const std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons, bool writeCount)
{
	typedef Polygon<Vert,AuxData> Poly;
	typedef shared_ptr<Poly> Poly_Ptr;
	typedef std::vector<Poly_Ptr> PolyVector;

	if(writeCount) os << polygons.size() << '\n';
	for(typename PolyVector::const_iterator it=polygons.begin(), iend=polygons.end(); it!=iend; ++it)
	{
		const Poly& curPoly = **it;
		int vertCount = curPoly.vertex_count();
		os << vertCount << ' ';
		for(int j=0; j<vertCount; ++j)
		{
			os << curPoly.vertex(j) << ' ';
		}
		os << curPoly.auxiliary_data() << '\n';
	}
}

/**
Writes a polyhedral brush to a std::ostream.

@param os		The std::ostream
@param brush	The polyhedral brush
*/
template <typename Poly>
void IOUtil::write_polyhedral_brush(std::ostream& os, const PolyhedralBrush<Poly>& brush)
{
	os << "{\n";
	os << brush.function() << '\n';
	os << brush.bounds().minimum() << ' ' << brush.bounds().maximum() << '\n';
	write_polygons(os, brush.faces(), true);
	os << "}\n";
}

}
