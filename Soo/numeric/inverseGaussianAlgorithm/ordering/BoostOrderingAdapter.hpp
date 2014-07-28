#pragma once

#include <fstream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cuthill_mckee_ordering.hpp>
#include "boost/graph/graph_utility.hpp"
#include "boost/graph/minimum_degree_ordering.hpp"
#include <boost/graph/properties.hpp>

#include "Soo/numeric/inverseGaussianAlgorithm/ordering/VectorOrdering.hpp"
#include "Soo/network/boost/LoadBoostGraph.hpp"

namespace Soo{
//Seems buggy in boost library.
VectorOrdering<int> getMinimumDegreeOrderingBoost(std::istream& stream, int delta = 0){
	auto graph = loadBoostGraph<boost::directedS, boost::setS>(stream);
	auto n = boost::num_vertices(graph);
	typedef std::deque<int> Vector;
	Vector inverse_perm(n, 0);
	Vector perm(n, 0);
	Vector supernode_sizes(n, 1); // init has to be 1

	typename boost::property_map<decltype(graph), boost::vertex_index_t>::type
			id = get(boost::vertex_index, graph);

	Vector degree(n, 0);
	boost::minimum_degree_ordering
			(graph,
			 boost::make_iterator_property_map(&degree[0], id, degree[0]),
			&inverse_perm[0],
			&perm[0],
			boost::make_iterator_property_map(&supernode_sizes[0], id, supernode_sizes[0]),
			delta, id);
	return VectorOrdering<int>(std::move(perm));
}

VectorOrdering<int> getReverseCuthillMckeeOrdering(std::istream& stream){
	auto graph = loadBoostGraph<boost::undirectedS, boost::vecS>(stream);
	typedef typename boost::graph_traits<decltype(graph)>::vertex_descriptor Vertex;

	typename boost::property_map<decltype(graph), boost::vertex_index_t>::type
			indexMap = get(boost::vertex_index, graph);

	std::vector<Vertex> inv_perm(boost::num_vertices(graph));

	//reverse cuthill_mckee_ordering
	boost::cuthill_mckee_ordering(graph, inv_perm.rbegin());

	typedef std::deque<int> Vector;
	Vector perm(boost::num_vertices(graph));
	std::transform(inv_perm.begin(), inv_perm.end(), perm.begin(), [&](Vertex v) {
		return indexMap[v];
	});
	return {std::move(perm)};
}

}//namespace Soo
