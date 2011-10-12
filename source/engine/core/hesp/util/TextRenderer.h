/***
 * hesperus: TextRenderer.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_TEXTRENDERER
#define H_HESP_TEXTRENDERER

#include <string>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <hesp/ogl/WrappedGL.h>

#include <hesp/colours/Colour3d.h>

namespace hesp {

class TextRenderer
{
	//#################### CONSTANTS ####################
public:
	enum HAlignFlag
	{
		HALIGN_LEFT		= 0,
		HALIGN_RIGHT	= 1,
		HALIGN_CENTRE	= 2,
	};

	enum VAlignFlag
	{
		VALIGN_TOP		= 0,
		VALIGN_BOTTOM	= 1,
		VALIGN_CENTRE	= 2,
	};

	//#################### NESTED CLASSES ####################
public:
	struct TextExtents
	{
		int width, height;
		TextExtents(int width_, int height_) : width(width_), height(height_) {}
	};

	//#################### PRIVATE VARIABLES ####################
private:
	GLuint m_base;
	int m_pixelHeight;

	//#################### CONSTRUCTORS ####################
public:
	TextRenderer(const std::string& typeface, int pointSize);

	//#################### DESTRUCTOR ####################
public:
	~TextRenderer();

	//#################### COPY CONSTRUCTOR & ASSIGNMENT OPERATOR ####################
private:
	// Note: Both left deliberately unimplemented.
	TextRenderer(const TextRenderer&);
	TextRenderer& operator=(const TextRenderer&);

	//#################### PUBLIC METHODS ####################
public:
	TextExtents text_extents(const std::string& text) const;
	int text_height_in_pixels() const;
	void write(const std::string& text, double x, double y, const Colour3d& colour = Colour3d(0,0,0)) const;
	void write_aligned(const std::string& text, double x, double y, HAlignFlag hflag, VAlignFlag vflag, const Colour3d& colour = Colour3d(0,0,0)) const;
	void write_centred(const std::string& text, double x, double y, const Colour3d& colour = Colour3d(0,0,0)) const;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<const TextRenderer> TextRenderer_CPtr;

}

#endif
