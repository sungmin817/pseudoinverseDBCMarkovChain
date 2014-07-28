pseudoinverse
=============

An algorithm for calculating O(N) elements of Markov chain generator matrix satisfying detailed balance condition.

The code is written by C++11 and requires boost library 1.49 or higher.
The code is tested using gcc 4.8.2 with the additional compiler flags -O3 -std=c++11 -ffast-math.

The source only contains one cpp file(main.cpp) and it computes all of the diagonal elements of the matrix 
{{-3, 1, 1, 1}, {1, -2, 1, 0}, {1, 1, -2, 0}, {1, 0, 0, -1}} using the minimum degree ordering scheme.

We utilizes the power of the template to make the code generic so one can easily replace the ordering scheme and the set of elements you want to get.

