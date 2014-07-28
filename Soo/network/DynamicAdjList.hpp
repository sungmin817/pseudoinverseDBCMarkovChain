#pragma once

#include <vector>
#include <type_traits>
#include <algorithm>

#include "Soo/TypeTraits.hpp"

namespace Soo{

template <template <class... > class NodeCont
		  , template <class... > class LinkCont
		  , class Index
		  , class Dummy
		  , class... Node
		  >
class DynamicAdjList {
public:
	DynamicAdjList() = delete;
};

template <template <class... > class NodeCont
		  , template <class... > class LinkCont
		  , class Index
		  , class... Node
		  >
class DynamicAdjList<NodeCont, LinkCont, Index
		, typename std::enable_if< Soo::IsVectorLike<NodeCont<LinkCont<Node...>>>::value , Index>::type
		, Node...
		>
{
public:
	typedef Index IndexType;
public:
	DynamicAdjList() = default;

	DynamicAdjList(Index nNode)
		: data(nNode) {}

	DynamicAdjList(NodeCont<LinkCont<Node...>> data)
		: data(std::move(data))
	{}

	void resizeNode(Index sizeNew){
		data.resize(sizeNew);
	}

	std::size_t nNode() const {
		return data.size();
	}

	std::size_t degree(const Index index) const  {
		return data[index].size();
	}

	const LinkCont<Node...>& operator[](const Index index) const
	{
		return data[index];
	}

	LinkCont<Node...>& operator[](const Index index)
	{
		return data[index];
	}

	template <class Rng>
	Index randomNeighbor(Index node, Rng& rng) const {
		Index pos = static_cast<Index>(degree(node) * rng());
		return operator[](node)[pos];
	}

    template <class... Args>
    void addLink(Index node1, Index node2, Args&&... args){
        addNeighborAt(node1, node2, std::forward<Args>(args)...);
        addNeighborAt(node2, node1, std::forward<Args>(args)...);
	}

	Index nLink() const {
		Index nLink{};
		for (const auto& linkList : data) {
			nLink += linkList.size();
		}
		return nLink / 2;
	}

private:
	template <typename U = LinkCont<Node...>,
			  typename std::enable_if<Soo::IsVectorLike<U>::value, int>::type = 0,
			  class... Args
			  >
	void addNeighborAt(Index at, Index to, Args&&... args){
		data[at].emplace_back(to, std::forward<Args>(args)...);
	}

	template <typename U = LinkCont<Node...>,
			  typename std::enable_if<Soo::IsSetLike<U>::value
									  or Soo::IsMapLike<U>::value, int>::type = 0,
			  class... Args>
	void addNeighborAt(Index at, Index to, Args&&... args){
		data[at].emplace(to, std::forward<Args>(args)...);
	}

private:
	NodeCont<LinkCont<Node...>> data;
};

template <template <class... > class LinkCont>
using DynAdjList = DynamicAdjList<std::vector, LinkCont, std::size_t, std::size_t, std::size_t>;

}//namespace Soo
