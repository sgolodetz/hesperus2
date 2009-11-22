/***
 * test-angelscript-2.17.2: main.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <angelscript.h>

int main()
{
	asIScriptEngine *engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
	engine->Release();
	return 0;
}
