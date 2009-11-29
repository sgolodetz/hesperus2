/***
 * hesperus: VisTable.tpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

namespace hesp {

//#################### CONSTRUCTORS ####################
template <typename T>
VisTable<T>::VisTable(int n, const T& initialValue)
:	m_table(n)
{
	for(int i=0; i<n; ++i)
	{
		m_table[i].resize(n, initialValue);
	}
}

//#################### PUBLIC OPERATORS ####################
template <typename T>
T& VisTable<T>::operator()(int i, int j)
{
	return m_table[i][j];
}

template <typename T>
const T& VisTable<T>::operator()(int i, int j) const
{
	return m_table[i][j];
}

//#################### PUBLIC METHODS ####################
template <typename T>
int VisTable<T>::size() const
{
	return static_cast<int>(m_table.size());
}

}
