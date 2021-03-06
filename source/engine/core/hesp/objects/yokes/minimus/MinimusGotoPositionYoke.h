/***
 * hesperus: MinimusGotoPositionYoke.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MINIMUSGOTOPOSITIONYOKE
#define H_HESP_MINIMUSGOTOPOSITIONYOKE

#include <list>

#include <hesp/math/vectors/Vector3.h>
#include <hesp/objects/base/IYoke.h>
#include <hesp/objects/base/ObjectID.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class NavLink> NavLink_Ptr;
class ObjectManager;

/**
This class represents a goto position yoke for the Minimus bot.
*/
class MinimusGotoPositionYoke : public IYoke
{
	//#################### PRIVATE VARIABLES ####################
private:
	ObjectID m_objectID;
	const ObjectManager *m_objectManager;

	Vector3d m_dest;
	std::vector<NavLink_Ptr> m_links;
	shared_ptr<std::list<int> > m_path;

	//#################### CONSTRUCTORS ####################
public:
	MinimusGotoPositionYoke(const ObjectID& objectID, const ObjectManager *objectManager, const Vector3d& dest);

	//#################### PUBLIC METHODS ####################
public:
	std::vector<ObjectCommand_Ptr> generate_commands(InputState& input);
};

}

#endif
