#pragma once

#include <cstddef>
#include <type_traits>

namespace Soo{

template <class IsReverse = std::true_type, class Index = std::size_t>
class NodeIndexOrdering {
public:

    template <class Graph>
    NodeIndexOrdering(const Graph& graph)
        : nSize(graph.nNode())
        , index(initializeIndex())
	{}

	Index top() const {
		return index;
	}

	void pop(){
		if(IsReverse::value){
			--index;
		}
		else {
			++index;
		}
	}

    bool isLastElement() const {
		if(IsReverse::value){
			return index == Index{};
		} else {
			return index == nSize -1;
		}
	}

private:
	Index initializeIndex() const {
		if(IsReverse::value){
			return nSize - 1;
		} else {
			return Index{};
		}
	}

private:
	const Index nSize;
	Index index;
};

}//namespace Soo
