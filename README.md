# Parallel_Linear_Solver
Software which uses OpenMP to parallelise the three classic Algebraic Iterative Methods: Jacobi, Gauss-Seidel &amp; Successive Over Relaxationolving, for solving Systems of the form Ax = b.

## Introduction
* There is a problem in trying to parallelise the __SOR__ method because __SOR__ is inherently __sequential__ due to the dependency on the latest updated values within each iteration.
* However, we can parallelise it partially using a ___Red-Black Ordering Scheme___.
* However, this scheme is more appropriate for systems where the matrix isn't __dense__.
* If the matrix is dense, true parallel __SOR__ isn’t straightforward without altering the algorithms logic.

## Requirements
* __Compiler__:`g++ 13.1.0`. Where possible, __C++20__ features have been used.
* __OS__: `Ubuntu 20.04`.
* `OpenMP`. For __parallelising__ the iterative methods: __Jacobi__, __Gauss-Seidel__ and __SOR__.
* `CMake`. For building the software.
* `matplotlib-cpp`. For plotting __Convergence Rates__.
