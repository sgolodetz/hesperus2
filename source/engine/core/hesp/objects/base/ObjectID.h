/***
 * hesperus: ObjectID.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_OBJECTID
#define H_HESP_OBJECTID

#include <string>

namespace hesp {

class ObjectID
{
	//#################### PRIVATE VARIABLES ####################
private:
	int m_id;

	//#################### CONSTRUCTORS ####################
public:
	ObjectID();
	explicit ObjectID(int id);

	//#################### PUBLIC METHODS ####################
public:
	std::string to_string() const;
	bool valid() const;
	int value() const;
};

//#################### GLOBAL OPERATORS ####################
bool operator==(const ObjectID& lhs, const ObjectID& rhs);
bool operator!=(const ObjectID& lhs, const ObjectID& rhs);
bool operator<(const ObjectID& lhs, const ObjectID& rhs);

}

#endif
