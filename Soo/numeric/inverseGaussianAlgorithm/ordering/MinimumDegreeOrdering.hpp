#pragma once

#include <deque>
#include <set>
#include <vector>

#include "Soo/range/Range.hpp"
#include "Soo/numeric/inverseGaussianAlgorithm/ordering/VectorOrdering.hpp"

#include "boost/heap/pairing_heap.hpp"

namespace Soo{

///The LinkCont should be SetLike
template <class DynamicAdjList>
class MinimumDegreeOrdering{
public:
	typedef typename DynamicAdjList::IndexType IntType;
	typedef boost::heap::pairing_heap< std::pair<IntType, IntType>,
		boost::heap::compare< std::greater<std::pair<IntType, IntType> > > > PriorityQueueType;
	typedef typename PriorityQueueType::handle_type Handle;

public:
	MinimumDegreeOrdering(DynamicAdjList dynAdjList)
		: adjList(std::move(dynAdjList))
	{
		for (IntType i = 0; i < adjList.nNode(); ++i) {
			handle.emplace_back( pq.emplace( std::make_pair(adjList.degree(i), i) ) );
		}
	}

	std::deque<IntType> killAll(){
		while(!pq.empty()){
			IntType node = pq.top().second;
			pq.pop();
			ordering.push_back(node);
			killNode(node);
		}
		return std::move(ordering);
	}

	void killNode(IntType node){
		forEachPair(adjList[node], [&](IntType nei1, IntType nei2){
			adjList.addLink(nei1, nei2);
		});

		for (const auto& nei : adjList[node]) {
			adjList[nei].erase(node);
		}
		for (const auto nei : adjList[node]) {
			*handle[nei] = std::make_pair( adjList.degree(nei), nei );
			pq.update(handle[nei]);
		}
		adjList[node].clear();
	}

public:
	DynamicAdjList adjList;
	PriorityQueueType pq;
	std::vector<Handle> handle;
	std::deque<IntType> ordering;
};

template <class DynamicAdjList>
class MinDegreeOrdering{
public:
    typedef typename DynamicAdjList::IndexType IntType;
    typedef boost::heap::pairing_heap< std::pair<IntType, IntType>,
        boost::heap::compare< std::greater<std::pair<IntType, IntType> > > > PriorityQueueType;
    typedef typename PriorityQueueType::handle_type Handle;

public:
    MinDegreeOrdering(const DynamicAdjList& dynAdjList)
        : adjList(dynAdjList)
    {
        for (IntType i = 0; i < adjList.nNode(); ++i) {
            handle.emplace_back( pq.emplace( std::make_pair(adjList.degree(i), i) ) );
        }
    }

    bool isLastElement() const {
        return pq.size() == 1;
    }

    IntType top() const {
        std::cout << pq.top().second << std::endl;
        return pq.top().second;
    }

    void pop(IntType killed){
        pq.pop();
        for (const auto& neighbor : adjList[killed]) {
            auto nei = neighbor.first;
            *handle[nei] = std::make_pair( adjList.degree(nei), nei );
            pq.update(handle[nei]);
        }
    }

private:
    const DynamicAdjList& adjList;
    PriorityQueueType pq;
    std::vector<Handle> handle;
//    std::deque<IntType> ordering;
};

//VectorOrdering<std::size_t> getMinimumDegreeOrdering(const std::string& fName){
//	MinimumDegreeOrdering<DynAdjList<std::set>> ordering(makeDynAdjList<std::set>(fName));
//	return VectorOrdering<std::size_t>(ordering.killAll());
//}

}//namespace Soo
