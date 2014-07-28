#include <iostream>

#include "Soo/numeric/inverseGaussianAlgorithm/SeriesValue.hpp"
#include "Soo/numeric/inverseGaussianAlgorithm/InverseGaussianAlgorithm.hpp"
#include "Soo/numeric/inverseGaussianAlgorithm/ordering/MinimumDegreeOrdering.hpp"

using namespace std;
using namespace Soo;

/////Test case: matrix = {{-3 + \[Mu], 1, 1, 1}, {1, -2 + \[Mu], 1, 0}, {1, 1, -2 + \[Mu], 0}, {1, 0, 0, -1 + \[Mu]}}
/////PseudoInverse of matrix = {{-(3/16), 1/16, 1/16, 1/16}, {1/16, -(17/48), -(1/48), 5/16}, {1/16, -(1/48), -(17/48), 5/16}, {1/16, 5/16, 5/16, -(11/16)}}
///// which is = {{-0.1875, 0.0625, 0.0625, 0.0625}, {0.0625, -0.354167, -0.0208333, 0.3125}, {0.0625, -0.0208333, -0.354167, 0.3125}, {0.0625, 0.3125, 0.3125, -0.6875}}

int main()
{
    typedef Matrix<SeriesValue> MatType;

    MatType::Graph g(4);
    g.addLink(0, 1, SeriesValue{1.0});
    g.addLink(0, 2, SeriesValue{1.0});
    g.addLink(0, 3, SeriesValue{1.0});
    g.addLink(1, 2, SeriesValue{1.0});

    MatType::DiagonalWeight diag = {{-3.0, 1.0}, {-2.0,1.0}, {-2.0,1.0}, {-1.0,1.0}};
    MatType mat(std::move(diag), std::move(g));

    mat.killAll();
    auto res = mat.getResult();

    for (auto val : res) {
        cout << val << endl;
    }
}
