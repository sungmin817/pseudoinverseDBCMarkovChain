#pragma once

#include <iterator>

namespace Soo{

template <class Range, class Func>
void forEachPair(Range& rng, Func func) {
	for (auto i = rng.begin(); i != rng.end(); ++i) {
		for (auto j = std::next(i); j != rng.end(); ++j) {
			func(*i, *j);
		}
	}
}

}//namespace Soo
