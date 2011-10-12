/***
 * hesperus: PropertyIO.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "FieldIO.h"
#include "PropFormatter.h"
#include "PropReader.h"

namespace hesp {

//#################### LOADING METHODS ####################
template <typename T>
void PropertyIO::load_typed_property(Properties& properties, const std::string& name, const std::string& value)
{
	properties.set(name, read_property<T>(value));
}

//#################### SAVING METHODS ####################
template <typename T>
void PropertyIO::save_typed_property(std::ostream& os, const Properties& properties, const std::string& name)
{
	FieldIO::write_typed_field(os, name, format_property(properties.get<T>(name)));
}

}
