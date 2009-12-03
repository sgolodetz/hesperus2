/***
 * hesperus: PriorityQueue.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_PRIORITYQUEUE
#define H_HESP_PRIORITYQUEUE

#include <map>
#include <vector>

#include <hesp/exceptions/Exception.h>

namespace hesp {

template <typename ID, typename Key, typename Data, typename Comp = std::less<Key> >
class PriorityQueue
{
	//#################### NESTED CLASSES ####################
public:
	class Element
	{
	private:
		Data m_data;
		ID m_id;
		Key m_key;

	public:
		Element() {}
		Element(const ID& id, const Key& key, const Data& data) : m_id(id), m_key(key), m_data(data) {}

		Data& data()				{ return m_data; }
		const ID& id() const		{ return m_id; }
		const Key& key() const		{ return m_key; }

		friend class PriorityQueue;
	};

	//#################### TYPEDEFS ####################
private:
	typedef std::map<ID,size_t> Dictionary;		// maps IDs to their current position in the heap
	typedef std::vector<Element> Heap;

	//#################### PRIVATE VARIABLES ####################
private:
	// Datatype Invariant: m_dictionary.size() == m_heap.size()
	Dictionary m_dictionary;
	Heap m_heap;

	//#################### PUBLIC METHODS ####################
public:
	void clear();
	bool contains(ID id) const;
	Element& element(ID id);
	bool empty() const;
	void erase(ID id);
	void insert(ID id, const Key& key, const Data& data);
	void pop();
	Element& top();
	void update_key(ID id, const Key& key);

	//#################### PRIVATE METHODS ####################
private:
	void ensure_invariant();
	void heapify(size_t i);
	static size_t left(size_t i);
	static size_t parent(size_t i);
	void percolate(size_t i);
	static size_t right(size_t i);
	void update_key_at(size_t i, const Key& key);
};

}

#include "PriorityQueue.tpp"

#endif
