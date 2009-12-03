/***
 * hesperus: ConfigOptions.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

namespace hesp {

//#################### PUBLIC METHODS ####################
template <typename T>
const T& ConfigOptions::get(const std::string& name) const
{
	return m_properties.get<T>(name);
}

template <typename T>
void ConfigOptions::set(const std::string& name, const T& value)
{
	m_properties.set(name, value);
}

}
