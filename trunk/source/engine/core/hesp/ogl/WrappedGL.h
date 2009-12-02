/***
 * hesperus: WrappedGL.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifdef _WIN32
	#define NOMINMAX		// prevent the min and max macros in windows.h being defined (they interfere with the Standard C++ equivalents)
	#include <windows.h>
#endif

#include <gl/gl.h>
