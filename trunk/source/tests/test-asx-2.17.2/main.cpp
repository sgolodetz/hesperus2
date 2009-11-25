/***
 * test-asx-2.17.2: main.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <cstdlib>
#include <iostream>

#include <ASXEngine.h>

int main()
try
{
	ASXEngine engine;
	if(!engine.load_and_build_script("../resources/ScriptA.as", "Test"))
	{
		engine.output_messages(std::cout);
		return EXIT_FAILURE;
	}

	ASXModule_Ptr module = engine.get_module("Test");
	ASXFunction<int()> f = module->get_global_function("f", f);
	std::cout << f() << '\n';

	ASXFunction<double(double)> square = module->get_global_function("square", square);
	std::cout << square(9) << '\n';

	return 0;
}
catch(ASXException& e)
{
	std::cout << e.cause() << '\n';
}
