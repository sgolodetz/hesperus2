/***
 * hesperus: BasicMaterial.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_BASICMATERIAL
#define H_HESP_BASICMATERIAL

#include <hesp/colours/Colour3d.h>
#include "Material.h"

namespace hesp {

class BasicMaterial : public Material
{
	//#################### PRIVATE VARIABLES ####################
private:
	Colour3d m_ambient;
	Colour3d m_diffuse;
	Colour3d m_emissive;
	Colour3d m_specular;
	double m_specularExponent;
	bool m_wireframe;

	//#################### CONSTRUCTORS ####################
public:
	BasicMaterial(const Colour3d& ambient, const Colour3d& diffuse, const Colour3d& specular, double specularExponent, const Colour3d& emissive,
				  bool wireframe = false);

	//#################### PUBLIC METHODS ####################
public:
	const Colour3d& ambient() const;
	void apply() const;
	const Colour3d& diffuse() const;
	const Colour3d& emissive() const;
	const Colour3d& specular() const;
	double specular_exponent() const;
	bool uses_texcoords() const;
};

}

#endif
