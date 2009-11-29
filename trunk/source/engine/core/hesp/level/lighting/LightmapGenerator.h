/***
 * hesperus: LightmapGenerator.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_LIGHTMAPGENERATOR
#define H_HESP_LIGHTMAPGENERATOR

#include <vector>

#include <hesp/level/vis/VisTable.h>
#include <hesp/util/PolygonTypes.h>
#include "Light.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class BSPTree> BSPTree_Ptr;
typedef shared_ptr<class Lightmap> Lightmap_Ptr;
typedef shared_ptr<class LightmapGrid> LightmapGrid_Ptr;

class LightmapGenerator
{
	//#################### TYPEDEFS ####################
private:
	typedef std::vector<LightmapGrid_Ptr> LightmapGridVector;
	typedef std::vector<Lightmap_Ptr> LightmapVector;
	typedef shared_ptr<LightmapVector> LightmapVector_Ptr;
	typedef shared_ptr<const LightmapVector> LightmapVector_CPtr;
	typedef std::vector<TexturedPolygon_Ptr> TexPolyVector;
	typedef std::vector<TexturedLitPolygon_Ptr> TexLitPolyVector;
	typedef shared_ptr<TexLitPolyVector> TexLitPolyVector_Ptr;
	typedef shared_ptr<const TexLitPolyVector> TexLitPolyVector_CPtr;

	//#################### PRIVATE VARIABLES ####################
private:
	// Input data
	TexPolyVector m_inputPolygons;
	std::vector<Light> m_lights;
	BSPTree_Ptr m_tree;
	LeafVisTable_Ptr m_leafVis;

	// Intermediate data
	LightmapGridVector m_grids;

	// Output data
	TexLitPolyVector_Ptr m_outputPolygons;
	LightmapVector_Ptr m_lightmaps;

	//#################### CONSTRUCTORS ####################
public:
	LightmapGenerator(const TexPolyVector& inputPolygons, const std::vector<Light>& lights, const BSPTree_Ptr& tree, const LeafVisTable_Ptr& leafVis);

	//#################### PUBLIC METHODS ####################
public:
	void generate_lightmaps();
	LightmapVector_CPtr lightmaps() const;
	TexLitPolyVector_CPtr lit_polygons() const;

	//#################### PRIVATE METHODS ####################
private:
	void clean_intermediate();
	void construct_ambient_lightmaps();
	void construct_grid(int n);
	void construct_grids();
	void process_light(int n);
	void process_lights();
};

}

#endif
