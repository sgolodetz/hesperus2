/***
 * hesperus: XMLElement.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_XMLELEMENT
#define H_HESP_XMLELEMENT

#include <map>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### TYPEDEFS ####################
typedef shared_ptr<class XMLElement> XMLElement_Ptr;
typedef shared_ptr<const class XMLElement> XMLElement_CPtr;

class XMLElement
{
	//#################### TYPEDEFS ####################
public:
	typedef std::map<std::string,std::vector<XMLElement_CPtr> > ChildMap;

	//#################### PRIVATE VARIABLES ####################
private:
	std::string m_name;
	std::map<std::string,std::string> m_attributes;
	ChildMap m_children;

	//#################### CONSTRUCTORS ####################
public:
	XMLElement(const std::string& name);

	//#################### PUBLIC METHODS ####################
public:
	void add_child(const XMLElement_Ptr& child);
	const std::string& attribute(const std::string& name) const;
	std::vector<XMLElement_CPtr> find_children(const std::string& name) const;
	XMLElement_CPtr find_unique_child(const std::string& name) const;
	bool has_attribute(const std::string& name) const;
	bool has_child(const std::string& name) const;
	const std::string& name() const;
	void set_attribute(const std::string& name, const std::string& value);
};

}

#endif
