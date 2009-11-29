/***
 * hesperus: PixelTypes.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_PIXELTYPES
#define H_HESP_PIXELTYPES

namespace hesp {

/**
This class represents 8-bit pixels.
*/
class Pixel8
{
	//#################### PRIVATE VARIABLES ####################
private:
	unsigned char m_data;

	//#################### CONSTRUCTORS ####################
public:
	// Note: Pixel types must be default constructible to make blank image construction easy.
	Pixel8() : m_data(0) {}

	Pixel8(unsigned char data) : m_data(data)	{}
	Pixel8(int data)							{ set_from_int(data); }

	//#################### ASSIGNMENT OPERATOR ####################
public:
	Pixel8& operator=(unsigned char data)
	{
		m_data = data;
		return *this;
	}

	Pixel8& operator=(int data)
	{
		set_from_int(data);
		return *this;
	}

	//#################### PUBLIC OPERATORS ####################
public:
	operator const unsigned char() const		{ return m_data; }
	bool operator==(const Pixel8& rhs) const	{ return m_data == rhs.m_data; }

	//#################### PUBLIC METHODS ####################
public:
	static int maximum()	{ return 255; }
	static int minimum()	{ return 0; }

	//#################### PRIVATE METHODS ####################
private:
	void set_from_int(int data)
	{
		if(data < 0) data = 0;
		else if(data > 255) data = 255;
		m_data = static_cast<unsigned char>(data);
	}
};

/**
This class represents 24-bit RGB pixels.
*/
class Pixel24
{
	//#################### PRIVATE VARIABLES ####################
private:
	unsigned char m_r, m_g, m_b;

	//#################### CONSTRUCTORS ####################
public:
	// Note: Pixel types must be default constructible to make blank image construction easy.
	Pixel24() : m_r(0), m_g(0), m_b(0) {}

	Pixel24(unsigned char r, unsigned char g, unsigned char b) : m_r(r), m_g(g), m_b(b) {}

	//#################### PUBLIC OPERATORS ####################
public:
	bool operator==(const Pixel24& rhs) const
	{
		return m_r == rhs.m_r && m_g == rhs.m_g && m_b == rhs.m_b;
	}

	//#################### PUBLIC METHODS ####################
public:
	const unsigned char r() const	{ return m_r; }
	const unsigned char g() const	{ return m_g; }
	const unsigned char b() const	{ return m_b; }
};

/**
This class represents 32-bit RGBA pixels.
*/
class Pixel32
{
	//#################### PRIVATE VARIABLES ####################
private:
	unsigned char m_r, m_g, m_b, m_a;

	//#################### CONSTRUCTORS ####################
public:
	// Note: Pixel types must be default constructible to make blank image construction easy.
	Pixel32() : m_r(0), m_g(0), m_b(0), m_a(255) {}

	Pixel32(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : m_r(r), m_g(g), m_b(b), m_a(a) {}

	//#################### PUBLIC OPERATORS ####################
public:
	bool operator==(const Pixel32& rhs) const
	{
		return m_r == rhs.m_r && m_g == rhs.m_g && m_b == rhs.m_b && m_a == rhs.m_a;
	}

	//#################### PUBLIC METHODS ####################
public:
	const unsigned char r() const	{ return m_r; }
	const unsigned char g() const	{ return m_g; }
	const unsigned char b() const	{ return m_b; }
	const unsigned char a() const	{ return m_a; }
};

}

#endif
