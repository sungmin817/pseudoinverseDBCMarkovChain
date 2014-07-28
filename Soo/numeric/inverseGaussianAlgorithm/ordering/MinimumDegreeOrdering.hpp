#pragma once

#include <deque>
#include <set>
#include <vector>

#include "Soo/range/Range.hpp"
#include "boost/heap/pairing_heap.hpp"

namespace Soo{

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
};

}//namespace Soo
