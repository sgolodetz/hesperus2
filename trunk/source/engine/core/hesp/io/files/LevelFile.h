/***
 * hesperus: LevelFile.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVELFILE
#define H_HESP_LEVELFILE

#include <hesp/images/Image.h>
#include <hesp/level/Level.h>

namespace hesp {

class LevelFile
{
	//#################### LOADING METHODS ####################
public:
	static Level_Ptr load(const std::string& filename);

	//#################### SAVING METHODS ####################
public:
	static void save_lit(const std::string& filename,
						 const std::vector<TexturedLitPolygon_Ptr>& polygons, const BSPTree_CPtr& tree,
						 const std::vector<Portal_Ptr>& portals,
						 const LeafVisTable_CPtr& leafVis,
						 const std::vector<Image24_Ptr>& lightmaps,
						 const std::vector<CollisionPolygon_Ptr>& onionPolygons, const OnionTree_CPtr& onionTree,
						 const std::vector<OnionPortal_Ptr>& onionPortals,
						 const NavManager_CPtr& navManager,
						 const std::string& definitionsFilename,
						 const ObjectManager_Ptr& objectManager);
	static void save_unlit(const std::string& filename,
						   const std::vector<TexturedPolygon_Ptr>& polygons, const BSPTree_CPtr& tree,
						   const std::vector<Portal_Ptr>& portals,
						   const LeafVisTable_CPtr& leafVis,
						   const std::vector<CollisionPolygon_Ptr>& onionPolygons, const OnionTree_CPtr& onionTree,
						   const std::vector<OnionPortal_Ptr>& onionPortals,
						   const NavManager_CPtr& navManager,
						   const std::string& definitionsFilename,
						   const ObjectManager_Ptr& objectManager);

	//#################### LOADING SUPPORT METHODS ####################
private:
	static Level_Ptr load_lit(std::istream& is);
	static Level_Ptr load_unlit(std::istream& is);
};

}

#endif
