/***
 * test-asx-2.17.2: main.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <cstdlib>
#include <iostream>
#include <string>

#include <ASXEngine.h>

void print(const int& n)
{
	std::cout << n << '\n';
}

struct Pair
{
	int first, second;

	Pair(int first_, int second_) : first(first_), second(second_) {}

	Pair operator+(Pair rhs) const
	{
		return Pair(first + rhs.first, second + rhs.second);
	}

	static void register_for_scripting(ASXEngine& engine);

	static std::string type_string()
	{
		return "Pair";
	}
};

void Pair_Constructor(Pair *p, int first_, int second_)
{
	new (p) Pair(first_, second_);
}

void Pair_Destructor(Pair *p)
{
	p->~Pair();
}

void Pair::register_for_scripting(ASXEngine& engine)
{
	engine.register_pod_type<Pair>();
	engine.register_object_property(&Pair::first, "first");
	engine.register_object_property(&Pair::second, "second");
	engine.register_constructor(Pair_Constructor);
	engine.register_destructor(Pair_Destructor);
	engine.register_object_method(&Pair::operator+, "opAdd");
}

struct X
{
	int n;

	explicit X(int n_) : n(n_) {}

	static void register_for_scripting(ASXEngine& engine);
};

void X_Constructor(X *p, int n_)
{
	new (p) X(n_);
}

void X::register_for_scripting(ASXEngine& engine)
{
	engine.register_pod_type<X>("FunkyX");
	engine.register_object_property(&X::n, "n", "FunkyX");
	engine.register_constructor(X_Constructor, "FunkyX");
}

int main()
try
{
	ASXEngine engine;

	engine.register_global_function(print, "print");
	Pair::register_for_scripting(engine);
	X::register_for_scripting(engine);

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

	ASXFunction<std::string(std::string)> identity = module->get_global_function("identity", identity);
	std::string from = "Blah";
	std::string to = identity(from);
	std::cout << to << '\n';

	ASXFunction<int(Pair)> first = module->get_global_function("first", first);
	std::cout << first(Pair(23,9)) << '\n';

	ASXFunction<void(X)> print_n_of_x = module->get_global_function_ex("void print_n_of_x(FunkyX)", print_n_of_x);
	print_n_of_x(X(17));

	ASXFunction<Pair(Pair,Pair)> add_pairs = module->get_global_function("add_pairs", add_pairs);
	std::cout << first(add_pairs(Pair(3,4), Pair(5,6))) << '\n';

	ASXFunction<Pair(Pair,Pair)> sub_pairs = module->get_global_function("sub_pairs", sub_pairs);
	std::cout << first(sub_pairs(Pair(3,4), Pair(5,6))) << '\n';

	ASXFunction<X()> make_x = module->get_global_function_ex("FunkyX make_x()", make_x);
	std::cout << make_x().n << '\n';

	return 0;
}
catch(ASXException& e)
{
	std::cout << e.cause() << '\n';
}
