# Parallel_Linear_Solver
Software which uses OpenMP to parallelise the three classic Algebraic Iterative Methods: Jacobi, Gauss-Seidel &amp; Successive Over Relaxationolving, for solving Systems of the form Ax = b.

## Introduction
* There is a problem in trying to parallelise the __Gauss-Seidel__ & __SOR__ methods because they are inherently __sequential__ due to the dependency on the latest updated values within each iteration.
* However, we can parallelise it partially using a ___Red-Black Ordering Scheme___.
* However, this scheme is more appropriate for systems where the matrix isn't __dense__.
* If the matrix is dense, true parallel forms of the methods is tricky, and so the algorithm logic has to be altered.
* If the matrix is __dense__, we can use ___Graph Colouring___ (AKA ___Multi-Colouring___).

## The Iterative Solvers
* Here is a brief overview of the methods.

### The Jacobi Method

### The Gauss-Seidel Method

### The Successive Over Relaxation (SOR) Method

### Red-Black Ordering Scheme: What is it?
* This is a technique to organize a __structured grid__ (like a 2D matrix) into two independent groups of points — typically labeled __"red"__ and __"black"__ like a __chessboard pattern__.
* Points of one colour can be updated in parallel because none of them directly depend on each other - only on the other color's points.
* You alternate updating all the red points and then all the black points in each iteration.


## Requirements
* __Compiler__:`g++ 13.1.0`. Where possible, __C++20__ features have been used.
* __OS__: `Ubuntu 20.04`.
* `OpenMP`. For __parallelising__ the iterative methods: __Jacobi__, __Gauss-Seidel__ and __SOR__.
* `CMake`. For building the software.
* `matplotlib-cpp`. For plotting __Convergence Rates__.
