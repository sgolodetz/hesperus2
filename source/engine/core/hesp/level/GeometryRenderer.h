/***
 * hesperus: GeometryRenderer.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_GEOMETRYRENDERER
#define H_HESP_GEOMETRYRENDERER

#include <map>
#include <set>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class Texture> Texture_Ptr;

class GeometryRenderer
{
	//#################### PROTECTED VARIABLES ####################
protected:
	std::map<std::string,Texture_Ptr> m_textures;

	//#################### DESTRUCTOR ####################
public:
	virtual ~GeometryRenderer() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void render(const std::vector<int>& polyIndices) const = 0;

	//#################### PROTECTED METHODS ####################
protected:
	void load_textures(const std::set<std::string>& textureNames);
};

//#################### TYPEDEFS ####################
typedef shared_ptr<GeometryRenderer> GeometryRenderer_Ptr;
typedef shared_ptr<const GeometryRenderer> GeometryRenderer_CPtr;

}

#endif
