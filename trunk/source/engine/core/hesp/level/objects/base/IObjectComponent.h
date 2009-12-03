/***
 * hesperus: IObjectComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_IOBJECTCOMPONENT
#define H_HESP_IOBJECTCOMPONENT

#include <string>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include "MessageHandler.h"
#include "ObjectID.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
class ObjectManager;
class Properties;

class IObjectComponent : public virtual MessageHandlerBase
{
	//#################### FRIENDS ####################
	friend class ObjectManager;

	//#################### PROTECTED VARIABLES ####################
protected:
	ObjectID m_objectID;
	ObjectManager *m_objectManager;

	//#################### DESTRUCTOR ####################
public:
	virtual ~IObjectComponent();

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual std::string group_type() const = 0;
	virtual std::string own_type() const = 0;
	virtual Properties save() const = 0;

	//#################### PUBLIC METHODS ####################
public:
	virtual void check_dependencies() const;
	const ObjectID& object_id() const;
	virtual void register_listening();

	//#################### PROTECTED METHODS ####################
protected:
	void set_object_id(const ObjectID& objectID);
	virtual void set_object_manager(ObjectManager *objectManager);
};

//#################### TYPEDEFS ####################
typedef shared_ptr<IObjectComponent> IObjectComponent_Ptr;
typedef shared_ptr<const IObjectComponent> IObjectComponent_CPtr;

}

#endif
