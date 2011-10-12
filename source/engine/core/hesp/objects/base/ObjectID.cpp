/***
 * hesperus: ObjectID.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ObjectID.h"

#include <boost/lexical_cast.hpp>

namespace hesp {

//#################### CONSTRUCTORS ####################
ObjectID::ObjectID()
:	m_id(-1)
{}

ObjectID::ObjectID(int id)
:	m_id(id)
{}

//#################### PUBLIC METHODS ####################
std::string ObjectID::to_string() const
{
	return boost::lexical_cast<std::string>(m_id);
}

bool ObjectID::valid() const
{
	return m_id != -1;
}

int ObjectID::value() const
{
	return m_id;
}

//#################### GLOBAL OPERATORS ####################
bool operator==(const ObjectID& lhs, const ObjectID& rhs)
{
	return lhs.value() == rhs.value();
}

bool operator!=(const ObjectID& lhs, const ObjectID& rhs)
{
	return lhs.value() != rhs.value();
}

bool operator<(const ObjectID& lhs, const ObjectID& rhs)
{
	return lhs.value() < rhs.value();
}

}
