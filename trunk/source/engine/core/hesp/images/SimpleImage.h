/***
 * hesperus: SimpleImage.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_SIMPLEIMAGE
#define H_HESP_SIMPLEIMAGE

#include <boost/shared_array.hpp>
using boost::shared_array;

#include "Image.h"

namespace hesp {

/**
This class template provides a simple implementation of Image.
*/
template <typename Pixel>
class SimpleImage : public Image<Pixel>
{
	//#################### PRIVATE VARIABLES ####################
private:
	shared_array<Pixel> m_pixels;

	//#################### CONSTRUCTORS ####################
public:
	SimpleImage(int width, int height);
	SimpleImage(Pixel *pixels, int width, int height);
	SimpleImage(const shared_array<Pixel>& pixels, int width, int height);
	explicit SimpleImage(const Image_CPtr& rhs);

	//#################### COPY CONSTRUCTOR & ASSIGNMENT OPERATOR ####################
private:
	SimpleImage(const SimpleImage&);
	SimpleImage& operator=(const SimpleImage&);

	//#################### PUBLIC OPERATORS ####################
public:
	const Pixel operator()(int n) const;
	const Pixel operator()(int x, int y) const;

	//#################### PUBLIC METHODS ####################
public:
	void set(int n, const Pixel& pixel);
	void set(int x, int y, const Pixel& pixel);
};

//#################### TYPEDEFS ####################
typedef SimpleImage<Pixel8> SimpleImage8;
typedef SimpleImage<Pixel24> SimpleImage24;
typedef SimpleImage<Pixel32> SimpleImage32;

}

#include "SimpleImage.tpp"

#endif
