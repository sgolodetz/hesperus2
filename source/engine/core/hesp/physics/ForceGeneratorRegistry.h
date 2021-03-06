/***
 * hesperus: ForceGeneratorRegistry.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_FORCEGENERATORREGISTRY
#define H_HESP_FORCEGENERATORREGISTRY

#include <map>
#include <set>
#include <string>
#include <utility>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<const class ForceGenerator> ForceGenerator_CPtr;

class ForceGeneratorRegistry
{
	//#################### TYPEDEFS ####################
public:
	typedef std::map<std::string,ForceGenerator_CPtr> ForceGenerators;
private:
	typedef std::pair<int,std::string> ForceGeneratorHandle;

	//#################### PRIVATE VARIABLES ####################
private:
	std::map<int,ForceGenerators> m_generators;
	std::map<int,std::set<ForceGeneratorHandle> > m_references;

	//#################### PUBLIC METHODS ####################
public:
	void deregister_id(int id);
	const ForceGenerators& generators(int id) const;
	void register_id(int id);
	void remove_generator(int id, const std::string& forceName);
	void set_generator(int id, const std::string& forceName, const ForceGenerator_CPtr& generator);
};

}

#endif
