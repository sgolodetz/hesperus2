/***
 * mef2input: TexturePlane.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_MEF2INPUT_TEXTUREPLANE
#define H_MEF2INPUT_TEXTUREPLANE

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <hesp/math/vectors/TexCoords.h>
#include <hesp/math/vectors/Vector2d.h>
#include <hesp/math/vectors/Vector3.h>

namespace hesp {

class TexturePlane
{
	//#################### CONSTANTS ####################
private:
	// FIXME:	These should technically be read from the texture, but MapEditor does it this way and
	//			we need to be consistent. (It doesn't make any difference in practice, so it's not
	//			even worth changing in MapEditor.)
	enum
	{
		TEXTURE_WIDTH = 128,
		TEXTURE_HEIGHT = 128
	};

	//#################### PRIVATE VARIABLES ####################
private:
	double m_scaleU, m_scaleV;
	double m_offsetU, m_offsetV;	// the ou and ov texture offsets, in the texture coordinate system
	Vector2d m_uAxis;				// the u texture axis (note that |m_uAxis| is always 1), in the texture coordinate system
	Vector2d m_vAxis;				// the v texture axis (note that |m_vAxis| is always 1), in the texture coordinate system
	Vector3d m_horizAxis;			// the base horizontal axis of the texture plane (e.g. (0,1,0) when the face normal's
									// x component is largest), in the world coordinate system
	Vector3d m_vertAxis;			// the base vertical axis of the texture plane (e.g. (0,0,-1) when the face normal's
									// x component is largest), in the world coordinate system

	//#################### CONSTRUCTORS ####################
public:
	TexturePlane(double offsetU, double offsetV, double scaleU, double scaleV, double angleDegrees);

	//#################### PUBLIC METHODS ####################
public:
	TexCoords calculate_coordinates(const Vector3d& p3D) const;
	void determine_axis_vectors(const Vector3d& faceNormal);

	//#################### PRIVATE METHODS ####################
private:
	Vector2d project_to_texture_plane(const Vector3d& p3D) const;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<TexturePlane> TexturePlane_Ptr;

}

#endif
