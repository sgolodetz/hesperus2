/***
 * hdivide: main.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <iostream>
#include <string>
#include <vector>

#include <hesp/io/files/BrushesFile.h>
#include <hesp/io/files/GeometryFile.h>
#include <hesp/io/util/IOUtil.h>
#include <hesp/level/brushes/PolyhedralBrush.h>
#include <hesp/math/geom/GeomUtil.h>
#include <hesp/math/geom/Plane.h>
#include <hesp/util/PolygonTypes.h>
using namespace hesp;

//#################### TYPEDEFS ####################
typedef std::vector<CollisionPolygon_Ptr> ColPolyVector;
typedef PolyhedralBrush<CollisionPolygon> ColPolyBrush;
typedef shared_ptr<ColPolyBrush> ColPolyBrush_Ptr;
typedef std::vector<ColPolyBrush_Ptr> ColPolyBrushVector;

typedef std::vector<Plane> PlaneVector;

typedef std::vector<TexturedPolygon_Ptr> TexPolyVector;
typedef PolyhedralBrush<TexturedPolygon> TexPolyBrush;
typedef shared_ptr<TexPolyBrush> TexPolyBrush_Ptr;
typedef std::vector<TexPolyBrush_Ptr> TexPolyBrushVector;

//#################### FUNCTIONS ####################
void quit_with_error(const std::string& error)
{
	std::cout << "Error: " << error << std::endl;
	exit(EXIT_FAILURE);
}

void quit_with_usage()
{
	std::cout << "Usage: hdivide <input brushes> <output normal brushes> <output collision brushes> <output detail polygons> <output hint planes> <output special brushes>" << std::endl;
	exit(EXIT_FAILURE);
}

ColPolyAuxData make_aux_data(const Vector3d& faceNormal)
{
	// TODO: CPAuxData will eventually store more interesting things (see its definition).

	const double MAX_ANGLE_TO_VERTICAL = 45 * PI/180;	// i.e. 45 degrees
	double angleToVertical = acos(faceNormal.dot(Vector3d(0,0,1)));
	bool walkable = fabs(angleToVertical) <= MAX_ANGLE_TO_VERTICAL;

	return ColPolyAuxData(walkable);
}

ColPolyBrush_Ptr convert_brush(const TexPolyBrush_Ptr& texBrush)
{
	AABB3d bounds = texBrush->bounds();
	const TexPolyVector& texFaces = texBrush->faces();

	int faceCount = static_cast<int>(texFaces.size());
	ColPolyVector colFaces(faceCount);
	for(int i=0; i<faceCount; ++i)
	{
		int vertCount = texFaces[i]->vertex_count();
		std::vector<Vector3d> colVertices;
		colVertices.reserve(vertCount);
		for(int j=0; j<vertCount; ++j)
		{
			// Note: There is an implicit conversion from TexturedVector3d -> Vector3d taking place here.
			colVertices.push_back(texFaces[i]->vertex(j));
		}
		CollisionPolygon::AuxData colAuxData = make_aux_data(texFaces[i]->normal());
		colFaces[i].reset(new CollisionPolygon(colVertices, colAuxData));
	}

	return ColPolyBrush_Ptr(new ColPolyBrush(bounds, colFaces, texBrush->function()));
}

TexPolyVector extract_hint_polygons(const TexPolyBrushVector& hintBrushes)
{
	TexPolyVector ret;
	for(TexPolyBrushVector::const_iterator it=hintBrushes.begin(), iend=hintBrushes.end(); it!=iend; ++it)
	{
		const TexPolyVector& brushFaces = (*it)->faces();
		for(TexPolyVector::const_iterator jt=brushFaces.begin(), jend=brushFaces.end(); jt!=jend; ++jt)
		{
			const TexturedPolygon_Ptr& brushFace = *jt;
			if(brushFace->auxiliary_data() == "HINT") ret.push_back(brushFace);
		}
	}
	return ret;
}

void run_divider(const std::string& inputBrushesFilename, const std::string& renderingBrushesFilename,
				 const std::string& collisionBrushesFilename, const std::string& detailBrushesFilename,
				 const std::string& hintPolygonsFilename, const std::string& specialBrushesFilename)
{
	// Read in the rendering brushes.
	TexPolyBrushVector inputBrushes = BrushesFile::load<TexturedPolygon>(inputBrushesFilename);

	// Separate the brushes according to their functions.
	TexPolyBrushVector collisionBrushes, detailBrushes, hintBrushes, renderingBrushes, specialBrushes;
	for(size_t i=0, size=inputBrushes.size(); i<size; ++i)
	{
		switch(inputBrushes[i]->function())
		{
			case BF_COLLISION:
				collisionBrushes.push_back(inputBrushes[i]);
				break;
			case BF_DETAIL:
				detailBrushes.push_back(inputBrushes[i]);
				break;
			case BF_HINT:
				hintBrushes.push_back(inputBrushes[i]);
				break;
			case BF_NORMAL:
				renderingBrushes.push_back(inputBrushes[i]);
				collisionBrushes.push_back(inputBrushes[i]);	// normal brushes are also used as collision brushes
				break;
			case BF_WATER:
				specialBrushes.push_back(inputBrushes[i]);
				break;
			default:
				throw Exception("Forgot to handle one of the brush function cases");
		}
	}

	// Convert the collision brushes to the right format and write them to disk.
	int collisionBrushCount = static_cast<int>(collisionBrushes.size());
	ColPolyBrushVector convertedCollisionBrushes(collisionBrushCount);
	for(int i=0; i<collisionBrushCount; ++i)
	{
		convertedCollisionBrushes[i] = convert_brush(collisionBrushes[i]);
	}
	BrushesFile::save(collisionBrushesFilename, convertedCollisionBrushes);

	// Write the detail brushes to disk.
	BrushesFile::save(detailBrushesFilename, detailBrushes);

	// Extract the hint polygons from the hint brushes and write them to disk.
	TexPolyVector hintPolygons = extract_hint_polygons(hintBrushes);
	GeometryFile::save(hintPolygonsFilename, hintPolygons);

	// Write the rendering brushes to disk.
	BrushesFile::save(renderingBrushesFilename, renderingBrushes);

	// Write the special brushes to disk.
	BrushesFile::save(specialBrushesFilename, specialBrushes);
}

int main(int argc, char *argv[])
try
{
	if(argc != 7) quit_with_usage();
	std::vector<std::string> args(argv, argv + argc);
	run_divider(args[1], args[2], args[3], args[4], args[5], args[6]);
	return 0;
}
catch(Exception& e) { quit_with_error(e.cause()); }
