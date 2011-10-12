/***
 * hesperus: CmpSpriteRender.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMPSPRITERENDER
#define H_HESP_CMPSPRITERENDER

#include "ICmpRender.h"

namespace hesp {

class CmpSpriteRender : public ICmpRender
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::string m_spriteName;
	double m_width, m_height;

	//#################### CONSTRUCTORS ####################
public:
	CmpSpriteRender(const std::string& spriteName, double width, double height);

	//#################### STATIC FACTORY METHODS ####################
public:
	static IObjectComponent_Ptr load(const Properties& properties);

	//#################### PUBLIC METHODS ####################
public:
	void check_dependencies() const;
	void render() const;
	Properties save() const;

	std::string own_type() const			{ return "SpriteRender"; }
	static std::string static_own_type()	{ return "SpriteRender"; }
};

}

#endif
