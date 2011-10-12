/***
 * hesperus: Material.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MATERIAL
#define H_HESP_MATERIAL

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

class Material
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~Material() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void apply() const = 0;
	virtual bool uses_texcoords() const = 0;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Material> Material_Ptr;
typedef shared_ptr<const Material> Material_CPtr;

}

#endif
