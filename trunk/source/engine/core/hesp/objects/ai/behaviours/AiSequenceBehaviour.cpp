/***
 * hesperus: AiSequenceBehaviour.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "AiSequenceBehaviour.h"

#include <algorithm>

#include <hesp/exceptions/Exception.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
AiSequenceBehaviour::AiSequenceBehaviour()
:	m_current(0), m_status(UNFINISHED)
{}

//#################### PUBLIC METHODS ####################
std::vector<ObjectCommand_Ptr> AiSequenceBehaviour::generate_commands()
{
	if(m_children.empty() || m_current == m_children.size()) return std::vector<ObjectCommand_Ptr>();

	std::vector<ObjectCommand_Ptr> commands;

	for(size_t size=m_children.size(); m_current<size; ++m_current)
	{
		const AiBehaviour_Ptr& child = m_children[m_current];
		switch(child->status())
		{
			case UNFINISHED:
			{
				std::vector<ObjectCommand_Ptr> childCommands = child->generate_commands();
				std::copy(childCommands.begin(), childCommands.end(), std::back_inserter(commands));
				if(child->status() == UNFINISHED) return commands;
				break;
			}
			case SUCCEEDED:
			{
				break;
			}
			case FAILED:
			{
				m_status = FAILED;

				// Note:	There is a design choice here - we could return all the commands generated
				//			by any previous children which succeeded. On the other hand, if the overall
				//			behaviour has failed, then there doesn't seem any point in executing lots
				//			of object commands which won't achieve anything useful. In practice, the
				//			difference probably doesn't matter (but this involves less work being done).
				return std::vector<ObjectCommand_Ptr>();
			}
			default:
			{
				throw Exception("Unexpected child behaviour status");
			}
		}
	}

	m_status = SUCCEEDED;

	return commands;
}

AiBehaviour::Status AiSequenceBehaviour::status() const
{
	if(m_children.empty()) return SUCCEEDED;
	else return m_status;
}

}
