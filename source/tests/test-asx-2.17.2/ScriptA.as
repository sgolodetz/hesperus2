/***
 * test-asx-2.17.2: ScriptA.as
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "sub/ScriptB.as"

int f()
{
	return 23;
}

int first(Pair p)
{
	return p.first;
}

Pair add_pairs(Pair p1, Pair p2)
{
	return p1 + p2;
}

Pair sub_pairs(Pair p1, Pair p2)
{
	return Pair(p1.first - p2.first, p1.second - p2.second);
}

void print_n_of_x(FunkyX x)
{
	print(x.n);
}

FunkyX make_x()
{
	return FunkyX(51);
}
