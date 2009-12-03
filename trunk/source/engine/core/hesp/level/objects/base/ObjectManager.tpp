/***
 * hesperus: ObjectManager.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <boost/lexical_cast.hpp>
#include <boost/pointer_cast.hpp>

#include <hesp/exceptions/Exception.h>

namespace hesp {

//#################### PUBLIC METHODS ####################
template <typename T>
shared_ptr<T> ObjectManager::get_component(const ObjectID& id, const shared_ptr<T>&)
{
	return get_component<T>(id, T::static_group_type());
}

template <typename T>
shared_ptr<const T> ObjectManager::get_component(const ObjectID& id, const shared_ptr<const T>&) const
{
	return get_component<const T>(id, T::static_group_type());
}

//#################### PRIVATE METHODS ####################
template <typename T>
shared_ptr<T> ObjectManager::get_component(const ObjectID& id, const std::string& group)
{
	return boost::const_pointer_cast<T,const T>(const_cast<const ObjectManager *>(this)->get_component<const T>(id, group));
}

template <typename T>
shared_ptr<const T> ObjectManager::get_component(const ObjectID& id, const std::string& group) const
{
	std::map<ObjectID,Object>::const_iterator it = m_objects.find(id);
	if(it == m_objects.end()) throw Exception("Invalid object ID: " + id.to_string());

	const Object& object = it->second;

	Object::const_iterator jt = object.find(group);
	if(jt == object.end()) return shared_ptr<const T>();

	// Note that there's an implicit T -> const T conversion done when returning here.
	return boost::dynamic_pointer_cast<T,IObjectComponent>(jt->second);
}

}
