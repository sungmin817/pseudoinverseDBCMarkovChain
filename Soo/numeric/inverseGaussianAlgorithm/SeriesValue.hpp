#pragma once

#include <iostream>

namespace Soo{

struct SeriesValue {
	/// a0 + a1 mu + a2 mu^2
	SeriesValue(double a0 = 0.0, double a1 = 0.0, double a2 = 0.0)
		: a0(a0), a1(a1), a2(a2) {}

	SeriesValue(const SeriesValue& ) = default;

	SeriesValue& operator+= (SeriesValue n){
		a0 += n.a0;
		a1 += n.a1;
		a2 += n.a2;
		return *this;
	}

	friend std::ostream& operator<<(std::ostream& out, SeriesValue n){
		out << n.a0 << " " << n.a1 << " " << n.a2;
		return out;
	}

	double a0{};
	double a1{};
	double a2{};
};

inline double& operator+=(double& res, SeriesValue added){
	return (res += added.a0);
}

inline double& operator-=(double& res, SeriesValue added){
    return (res -= added.a0);
}


///This basically calculates (- n1 * n2 / n3). Beware of the sign!!.
inline SeriesValue update(SeriesValue n1,SeriesValue n2, SeriesValue n3, std::false_type = {}){
	return { -n1.a0 * n2.a0 / n3.a0,
				-(n1.a1 * n2.a0 * n3.a0 + n1.a0 * n2.a1 * n3.a0 - n1.a0 * n2.a0 * n3.a1)/(n3.a0 * n3.a0),
				-(n3.a0 * n3.a0 * ( n1.a2 * n2.a0 + n1.a1 * n2.a1 + n1.a0 * n2.a2)
				  - n3.a0 * n3.a1 * ( n1.a1 * n2.a0 + n1.a0 * n2.a1) +
				  n3.a1 * n3.a1 * n1.a0 * n2.a0 - n3.a0 * n3.a2 * n1.a0 * n2.a0) / (n3.a0 * n3.a0 * n3.a0)
	};
}

///This calculates zeroth order of (- n1 * n2 / n3). Note that n3 is singular which means that n3.a0 == 0.0
inline double update(SeriesValue n1, SeriesValue n2, SeriesValue n3, std::true_type){
    return -( n1.a0 * n2.a1 * n3.a1 + n1.a1 * n2.a0 * n3.a1 - n1.a0 * n2.a0 * n3.a2) / (n3.a1 * n3.a1);
}

}//namespace Soo
