/***
 * hesperus: ConfiguredPose.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CONFIGUREDPOSE
#define H_HESP_CONFIGUREDPOSE

#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<const class ConfiguredBone> ConfiguredBone_CPtr;

class ConfiguredPose
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::vector<ConfiguredBone_CPtr> m_bones;

	//#################### CONSTRUCTORS ####################
public:
	explicit ConfiguredPose(const std::vector<ConfiguredBone_CPtr>& bones);

	//#################### COPY CONSTRUCTOR & ASSIGNMENT OPERATOR ####################
private:
	ConfiguredPose(const ConfiguredPose&);
	ConfiguredPose& operator=(const ConfiguredPose&);

	//#################### PUBLIC METHODS ####################
public:
	const ConfiguredBone_CPtr& bones(int i) const;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ConfiguredPose> ConfiguredPose_Ptr;
typedef shared_ptr<const ConfiguredPose> ConfiguredPose_CPtr;

}

#endif
