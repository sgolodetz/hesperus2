/***
 * hesperus: PriorityQueue.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#define PQ_HEADER	template <typename ID, typename Key, typename Data, typename Comp>
#define PQ_THIS		PriorityQueue<ID,Key,Data,Comp>

namespace hesp {

//#################### PUBLIC METHODS ####################
PQ_HEADER
void PQ_THIS::clear()
{
	m_dictionary.clear();
	m_heap.swap(Heap());

	ensure_invariant();
}

PQ_HEADER
bool PQ_THIS::contains(ID id) const
{
	return m_dictionary.find(id) != m_dictionary.end();
}

PQ_HEADER
typename PQ_THIS::Element& PQ_THIS::element(ID id)
{
	return m_heap[m_dictionary[id]];
}

PQ_HEADER
bool PQ_THIS::empty() const
{
	return m_dictionary.empty();
}

PQ_HEADER
void PQ_THIS::erase(ID id)
{
	size_t i = m_dictionary[id];
	m_dictionary.erase(id);
	m_heap[i] = m_heap[m_heap.size()-1];
	if(m_heap[i].id() != id)	// assuming the element we were erasing wasn't the last one in the heap, update the dictionary
	{
		m_dictionary[m_heap[i].id()] = i;
	}
	m_heap.pop_back();
	heapify(i);

	ensure_invariant();
}

PQ_HEADER
void PQ_THIS::insert(ID id, const Key& key, const Data& data)
{
	if(contains(id))
	{
		throw Exception("An element with the specified ID is already in the priority queue");
	}

	size_t i = m_heap.size();
	m_heap.resize(i+1);
	while(i > 0 && Comp()(key, m_heap[parent(i)].key()))
	{
		size_t p = parent(i);
		m_heap[i] = m_heap[p];
		m_dictionary[m_heap[i].id()] = i;
		i = p;
	}
	m_heap[i] = Element(id, key, data);
	m_dictionary[id] = i;

	ensure_invariant();
}

PQ_HEADER
void PQ_THIS::pop()
{
	erase(m_heap[0].id());
	ensure_invariant();
}

PQ_HEADER
typename PQ_THIS::Element& PQ_THIS::top()
{
	return m_heap[0];
}

PQ_HEADER
void PQ_THIS::update_key(ID id, const Key& key)
{
	size_t i = m_dictionary[id];
	update_key_at(i, key);

	ensure_invariant();
}

//#################### PRIVATE METHODS ####################
PQ_HEADER
void PQ_THIS::ensure_invariant()
{
	if(m_dictionary.size() != m_heap.size())
	{
		throw Exception("The operation which just executed invalidated the priority queue");
	}
}

PQ_HEADER
void PQ_THIS::heapify(size_t i)
{
	bool done = false;
	while(!done)
	{
		size_t L = left(i), R = right(i);
		size_t largest = i;
		if(L < m_heap.size() && Comp()(m_heap[L].key(), m_heap[largest].key()))
			largest = L;
		if(R < m_heap.size() && Comp()(m_heap[R].key(), m_heap[largest].key()))
			largest = R;
		if(largest != i)
		{
			std::swap(m_heap[i], m_heap[largest]);
			m_dictionary[m_heap[i].id()] = i;
			m_dictionary[m_heap[largest].id()] = largest;
			i = largest;
		}
		else done = true;
	}
}

PQ_HEADER
inline size_t PQ_THIS::left(size_t i)
{
	return 2*i + 1;
}

PQ_HEADER
inline size_t PQ_THIS::parent(size_t i)
{
	return (i+1)/2 - 1;
}

PQ_HEADER
void PQ_THIS::percolate(size_t i)
{
	while(i > 0 && Comp()(m_heap[i].key(), m_heap[parent(i)].key()))
	{
		size_t p = parent(i);
		std::swap(m_heap[i], m_heap[p]);
		m_dictionary[m_heap[i].id()] = i;
		m_dictionary[m_heap[p].id()] = p;
		i = p;
	}
}

PQ_HEADER
inline size_t PQ_THIS::right(size_t i)
{
	return 2*i + 2;
}

PQ_HEADER
void PQ_THIS::update_key_at(size_t i, const Key& key)
{
	if(Comp()(key, m_heap[i].key()))
	{
		// The key has increased.
		m_heap[i].m_key = key;
		percolate(i);
	}
	else if(Comp()(m_heap[i].key(), key))
	{
		// The key has decreased.
		m_heap[i].m_key = key;
		heapify(i);
	}
}

}

#undef PQ_HEADER
#undef PQ_THIS
