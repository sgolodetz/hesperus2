/***
 * hesperus: SteeringPipeline.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_STEERINGPIPELINE
#define H_HESP_STEERINGPIPELINE

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

class SteeringPipeline
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~SteeringPipeline();

	// TODO
};

//#################### TYPEDEFS ####################
typedef shared_ptr<SteeringPipeline> SteeringPipeline_Ptr;
typedef shared_ptr<const SteeringPipeline> SteeringPipeline_CPtr;

}

#endif
