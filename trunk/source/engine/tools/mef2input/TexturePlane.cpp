/***
 * mef2input: TexturePlane.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "TexturePlane.h"

#include <cmath>

#include <hesp/math/Constants.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
TexturePlane::TexturePlane(double offsetU, double offsetV, double scaleU, double scaleV, double angleDegrees)
:	m_offsetU(offsetU), m_offsetV(offsetV), m_scaleU(scaleU), m_scaleV(scaleV)
{
	double angleRadians = angleDegrees * PI / 180;
	m_uAxis = Vector2d(cos(angleRadians), sin(angleRadians));
	m_vAxis = Vector2d(-sin(angleRadians), cos(angleRadians));
}

//#################### PUBLIC METHODS ####################
TexCoords TexturePlane::calculate_coordinates(const Vector3d& p3D) const
{
	Vector2d p = project_to_texture_plane(p3D);
	return TexCoords((p.dot(m_uAxis)/m_scaleU + m_offsetU)/TEXTURE_WIDTH, (p.dot(m_vAxis)/m_scaleV + m_offsetV)/TEXTURE_HEIGHT);
}

void TexturePlane::determine_axis_vectors(const Vector3d& faceNormal)
{
	double xAbs = fabs(faceNormal.x), yAbs = fabs(faceNormal.y), zAbs = fabs(faceNormal.z);
	if(xAbs >= yAbs && xAbs >= zAbs)
	{
		// The x component of the face normal has the largest (absolute) value.
		m_horizAxis = Vector3d(0,1,0);
		m_vertAxis = Vector3d(0,0,-1);
	}	
	else if(yAbs >= zAbs)
	{
		// The y component of the face normal has the largest (absolute) value.
		m_horizAxis = Vector3d(1,0,0);
		m_vertAxis = Vector3d(0,0,-1);
	}
	else
	{
		// The z component of the face normal has the largest (absolute) value.
		m_horizAxis = Vector3d(1,0,0);
		m_vertAxis = Vector3d(0,-1,0);
	}
}

//#################### PRIVATE METHODS ####################
Vector2d TexturePlane::project_to_texture_plane(const Vector3d& p3D) const
{
	return Vector2d(m_horizAxis.dot(p3D), m_vertAxis.dot(p3D));
}

}
