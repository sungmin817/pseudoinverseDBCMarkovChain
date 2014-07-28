#pragma once

#include <vector>

#include "Soo/network/DynamicAdjList.hpp"
#include "Soo/numeric/inverseGaussianAlgorithm/ordering/NodeIndexOrdering.hpp"
#include "Soo/numeric/inverseGaussianAlgorithm/ordering/MinimumDegreeOrdering.hpp"
#include "Soo/range/Range.hpp"

namespace Soo{

template <class... IsSingular>
inline double update(double n1, double n2, double n3, IsSingular...){
	return { -n1 * n2 / n3 };
}

template <class Index = std::size_t>
class DiagonalGetter{
public:
	DiagonalGetter(Index nNode)
		: result(nNode) {}

	template<class CouplingGraph, class Number, class IsSingular = std::false_type>
	void push(const CouplingGraph& coupling
			  , Index killedNode, Number denominator, IsSingular isSingular = {})
	{
		for (const auto& coup : coupling[killedNode]) {
			result[coup.first] -= update(coup.second, coup.second, denominator, isSingular);
		}
	}

	const std::vector<double>& get(){
		return result;
	}

private:
	std::vector<double> result;
};

template <class Node = std::size_t>
class AverageGetter{
public:

	template<class... Args>
	AverageGetter(Args...){}

	template<class CouplingGraph, class Number>
	void push(const CouplingGraph& , Node , Number denominator, std::false_type){
		result += denominator.a1 / denominator.a0;
	}

	template<class CouplingGraph, class Number>
	void push(const CouplingGraph& , Node , Number denominator, std::true_type){
		result += denominator.a2 / denominator.a1;
	}

	double get(){
		return -result;
	}

private:
	double result{};
};

template <class Element = double
//		  , class Ordering = NodeIndexOrdering<>
		  , class Index = std::size_t
		  , class ResultGetter = DiagonalGetter<Index>
		  , class IgnoreCoupling = std::false_type>
class Matrix{
public:
	typedef std::vector<Element> DiagonalWeight;
	typedef	DynamicAdjList<std::vector
				, std::map, Index, Index, Index, Element> Graph;
//    using Ordering = NodeIndexOrdering<>;
    using Ordering = MinDegreeOrdering<Graph>;

public:
    Matrix(DiagonalWeight diag, Graph weight, Element coupStr = {1.0})
        : weight(std::move(weight))
        , diag(std::move(diag))
        , ordering(this->weight)
        , coupling(makeDefaultCoupling(this->weight.nNode(), coupStr))
        , getter(this->weight.nNode())
        {}


	void killAll(){
		while(not ordering.isLastElement()) {
			auto node = ordering.top();
            kill(node);
        }
		updateResult(ordering.top(), std::true_type{});
	}

	void kill(Index killedNode){
		updateWeight(killedNode);
		updateCoupling(killedNode);
        ordering.pop(killedNode);
		updateResult(killedNode, std::false_type{});
		clearGraphs(killedNode);
	}

	auto getResult() -> decltype(std::declval<ResultGetter>().get()){
		return getter.get();
	}

private:
	Graph makeDefaultCoupling(Index size, Element coupStrength){
		if(IgnoreCoupling::value){
			return Graph(0);
		} else
		{
			Graph g(size);
			for (Index i = 0; i < size; ++i) {
				g[i][i] = coupStrength;
			}
			return g;
		}
	}

	void updateWeight(Index killedNode)
	{
		const auto& neighbors = weight[killedNode];

		for (const auto& nei : neighbors) {
			diag[nei.first] += update(nei.second, nei.second, diag[killedNode]);
			weight[nei.first].erase(killedNode);
		}

		forEachPair(neighbors, [&](std::pair<Index, Element> i, std::pair<Index, Element> j){
			weight[i.first][j.first] += update(i.second, j.second, diag[killedNode]);
			weight[j.first][i.first] += update(j.second, i.second, diag[killedNode]);
		});
	}

	void updateCoupling(Index killedNode)
	{
		if(not IgnoreCoupling::value){
			const auto& neighbors = weight[killedNode];

			for (const auto& nei : neighbors) {
				for (const auto& coup : coupling[killedNode]) {
					coupling[nei.first][coup.first]
							+= update(nei.second, coup.second, diag[killedNode]);
				}
			}
		}
	}

	template <class IsLast = std::false_type>
	void updateResult(Index killedNode, IsLast isLast = {})
	{
		getter.push(coupling, killedNode, diag[killedNode], isLast);
	}

	void clearGraphs(Index killedNode){

		weight[killedNode].clear();
		if(not IgnoreCoupling::value){
			coupling[killedNode].clear();
		}
	}

private:
	Graph weight; // link weight : self link should be stored in DiagonalWeight (Not here).
	DiagonalWeight diag; // weights of self link are stored.
	Ordering ordering;
	Graph coupling; // couplings : self link is **included**.
	ResultGetter getter;
};

}//namespace Soo
