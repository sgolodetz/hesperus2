/***
 * hesperus: Button.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_BUTTON
#define H_HESP_BUTTON

#include <string>

#include <boost/function.hpp>
#include <boost/optional.hpp>

#include "GUIComponent.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class Texture> Texture_Ptr;

class Button : public GUIComponent
{
	//#################### ENUMERATIONS ####################
private:
	enum State
	{
		HOVERED,		// the user is hovering their mouse over the button
		PRESSED,
		PRESSED_MOVED,	// the user pressed the button, but then dragged the mouse off it
		RELEASED,
	};

	//#################### TYPEDEFS ####################
private:
	typedef boost::function<void()> Handler;

	//#################### PRIVATE VARIABLES ####################
private:
	State m_state;
	Texture_Ptr m_inactiveTexture;
	Texture_Ptr m_activeTexture;

	boost::optional<Handler> m_mousePressedHandler;
	boost::optional<Handler> m_mouseReleasedHandler;

	//#################### CONSTRUCTORS ####################
public:
	Button(const std::string& inactiveFilename, const std::string& activeFilename);
	Button(const std::string& inactiveFilename, const std::string& activeFilename, const Handler& mousePressedHandler);
	Button(const std::string& inactiveFilename, const std::string& activeFilename, const boost::none_t&, const Handler& mouseReleasedHandler);
	Button(const std::string& inactiveFilename, const std::string& activeFilename, const Handler& mousePressedHandler, const Handler& mouseReleasedHandler);

	//#################### PUBLIC METHODS ####################
public:
	void handle_input(InputState& input);
	void render() const;

	//#################### PRIVATE METHODS ####################
private:
	void construct(const std::string& inactiveFilename, const std::string& activeFilename,
				   const boost::optional<Handler>& mousePressedHandler,
				   const boost::optional<Handler>& mouseReleasedHandler);
};

}

#endif
