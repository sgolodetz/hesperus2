/***
 * hesperus: BindingFile.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "BindingFile.h"

#include <map>
#include <vector>

#include <boost/lexical_cast.hpp>
using boost::lexical_cast;

#include <SDL.h>

#include <hesp/exceptions/Exception.h>
#include <hesp/input/InputBinding.h>
#include <hesp/input/KeyInputter.h>
#include <hesp/input/MouseButtonInputter.h>
#include <hesp/xml/XMLParser.h>

namespace hesp {

//#################### LOADING METHODS ####################
InputBinding_CPtr BindingFile::load(const std::string& filename)
{
	std::map<InputAction,Inputter_CPtr> inputters;

	// Construct the string -> key map.
	std::map<std::string,SDLKey> stringToKey;
	for(int i=0; i<SDLK_LAST; ++i)
	{
		SDLKey key = SDLKey(i);
		std::string name = SDL_GetKeyName(key);
		stringToKey.insert(std::make_pair(name,key));
	}

	XMLLexer_Ptr lexer(new XMLLexer(filename));
	XMLParser parser(lexer);
	XMLElement_CPtr root = parser.parse();
	XMLElement_CPtr bindingElt = root->find_unique_child("binding");

	std::vector<XMLElement_CPtr> actionElts = bindingElt->find_children("action");
	for(size_t i=0, size=actionElts.size(); i<size; ++i)
	{
		const XMLElement_CPtr& actionElt = actionElts[i];
		const std::string& actionName = actionElt->attribute("name");
		const std::string& actionInput = actionElt->attribute("input");

		InputAction action = lexical_cast<InputAction>(actionName);
		Inputter_CPtr inputter;

		std::map<std::string,SDLKey>::const_iterator jt = stringToKey.find(actionInput);
		if(jt != stringToKey.end())
		{
			inputter.reset(new KeyInputter(jt->second));
		}
		else if(actionInput == "left mouse") inputter.reset(new MouseButtonInputter(MOUSE_BUTTON_LEFT));
		else if(actionInput == "middle mouse") inputter.reset(new MouseButtonInputter(MOUSE_BUTTON_MIDDLE));
		else if(actionInput == "right mouse") inputter.reset(new MouseButtonInputter(MOUSE_BUTTON_RIGHT));
		else throw Exception("Unknown input action: " + actionInput);

		inputters.insert(std::make_pair(action, inputter));
	}

	return InputBinding_CPtr(new InputBinding(inputters));
}

}
