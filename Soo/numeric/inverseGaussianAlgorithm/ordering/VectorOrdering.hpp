#pragma once

#include <cstddef>
#include <fstream>
#include <type_traits>
#include <deque>

namespace Soo{

template <class Index = std::size_t>
class VectorOrdering {
public:
	VectorOrdering(std::istream& in)
		: ordering{}, currentPos{}
	{
		Index i;
		while(in >> i){
			ordering.push_back(i);
		}
	}

	VectorOrdering(std::deque<Index> v)
		: ordering(std::move(v))
		, currentPos{}
	{}

	Index top() const {
		return ordering.front();
	}

	void pop(){
		ordering.pop_front();
	}

	bool isLastElement(){
		return currentPos == ordering.size() - 1;
	}

private:
	std::deque<Index> ordering;
	Index currentPos;
};

}//namespace Soo
