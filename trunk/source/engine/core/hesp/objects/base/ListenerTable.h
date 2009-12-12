/***
 * hesperus: ListenerTable.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LISTENERTABLE
#define H_HESP_LISTENERTABLE

#include <vector>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
namespace bmi = boost::multi_index;

#include "ObjectID.h"

namespace hesp {

class ListenerTable
{
	//#################### NESTED CLASSES ####################
private:
	struct Entry
	{
		ObjectID m_sourceObject;				// the object containing the component doing the listening
		std::string m_sourceComponent;			// which component of the object is doing the listening
		ObjectID m_destObject;					// the object being listened to (-1 is used to represent "all objects" in this context)

		Entry(const ObjectID& sourceObject, const std::string& sourceComponent, const ObjectID& destObject)
		:	m_sourceObject(sourceObject), m_sourceComponent(sourceComponent), m_destObject(destObject)
		{}

		bool operator<(const Entry& rhs) const
		{
			return	m_sourceObject < rhs.m_sourceObject ||
					(m_sourceObject == rhs.m_sourceObject && m_destObject < rhs.m_destObject) ||
					(m_sourceObject == rhs.m_sourceObject && m_destObject == rhs.m_destObject && m_sourceComponent  < rhs.m_sourceComponent);
		}
	};

	// Tags
	struct source;
	struct dest;

	//#################### TYPEDEFS ####################
private:
	typedef bmi::multi_index_container<
		Entry,
		bmi::indexed_by<
			bmi::ordered_unique<bmi::identity<Entry> >,
			bmi::ordered_non_unique<bmi::tag<source>, bmi::member<Entry,ObjectID,&Entry::m_sourceObject> >,
			bmi::ordered_non_unique<bmi::tag<dest>, bmi::member<Entry,ObjectID,&Entry::m_destObject> >
		>
	> Table;

	//#################### PRIVATE VARIABLES ####################
private:
	Table m_table;

	//#################### PUBLIC METHODS ####################
public:
	void add_listener(const ObjectID& sourceObject, const std::string& sourceComponent, const ObjectID& destObject);
	std::vector<std::pair<ObjectID,std::string> > get_listeners(const ObjectID& destObject) const;
	void remove_listener(const ObjectID& sourceObject, const std::string& sourceComponent, const ObjectID& destObject);
	void remove_listeners_from(const ObjectID& sourceObject);
	void remove_listeners_to(const ObjectID& destObject);
};

}

#endif
