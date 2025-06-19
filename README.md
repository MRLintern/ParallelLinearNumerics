# Parallel_Linear_Solver
* `Modern C++ (ISO 20)` Software which uses __OpenMP__ to __parallelise__ the three classic __Algebraic Iterative Methods__: __Jacobi__, __Gauss-Seidel__ and __Successive Over Relaxation (SOR)__ for solving Systems of the form Ax = b.
* __Note__: this software looks at __measuring convergence of the solvers__ and __not on solving a large algebraic system of equations__; another time.

## Introduction
* There is a problem in trying to parallelise the __Gauss-Seidel__ & __SOR__ methods because they are inherently __sequential__ due to the dependency on the latest updated values within each iteration.
* However, we can parallelise it partially using a ___Red-Black Ordering Scheme___.
* However, this scheme is more appropriate for systems where the matrix isn't __dense__.
* If the matrix is dense, true parallel forms of the methods is tricky, and so the algorithm logic has to be altered.
* If the matrix is __dense__, we can use ___Graph Colouring___ (AKA ___Multi-Colouring___).
* Note: `range-based for loops` are used in places, so a compiler which supports `C++20` features is required. However, `OpenMP` still requires __traditional (index-based) for loops__.

## The Iterative Solvers
* Here is a brief overview of the methods.

### The Jacobi Method
* __Idea__: Updates each component of the solution vector independently using the values from the previous iteration.
* __Convergence__: Requires that the `coefficient matrix A` is `diagonally dominant` or `symmetric positive definite`.
* __Pros__: Simple and parallelizable.
* __Cons__: Slow convergence compared to the `Gauss-Seidel Method`.
  
### The Gauss-Seidel Method
* __Idea__: Like the `Jacobi Method`, but uses newly updated values as soon as they are available.
* __Convergence__: Often faster than the `Jacobi Method`. Also needs the `coefficient matrix A` to be `diagonally dominant` or `symmetric positive definite`.
* __Pros__: Improved convergence over the `Jacobi Method`.
* __Cons__: `Less parallelizable` due to `data dependencies`. To `parallelise`, you need to apply, for example, the `Red-Black Ordering Scheme`.
  
### The Successive Over Relaxation (SOR) Method
* __Idea__: An extension of the `Gauss-Seidel Method` that uses a ___Relaxation Factor ω___ to potentially `accelerate convergence`.
* __Parameter__: __ω ∈ (0,2)__.
    * __ω = 1__: `Gauss-Seidel Method`.
    * __ω > 1__: `Over-relaxation` (usually speeds up convergence).
* __Pros__: ___Can___ converge much faster with optimal ω.
* __Cons__: Requires tuning of ω; not always easy to choose. Values are chosen experimentally. `Less parallelizable` due to `data dependencies`. To `parallelise`, you need to apply, for example, the `Red-Black Ordering Scheme`.
  
### Red-Black Ordering Scheme: What is it?
* This is a technique to organize a __structured grid__ (like a 2D matrix) into two independent groups of points — typically labeled __"red"__ and __"black"__ like a __chessboard pattern__.
* Points of one colour can be updated in parallel because none of them directly depend on each other - only on the other colour's points.
* You alternate updating all the red points and then all the black points in each iteration.


## Requirements
* __Compiler__:`g++ 13.1.0`. 
* __OS__: `Ubuntu 20.04`.
* `OpenMP`. For __parallelising__ the iterative methods: __Jacobi__, __Gauss-Seidel__ and __SOR__.
* `CMake`. For building the software.
* `matplotlib-cpp`. For plotting __Convergence Rates__.

## Instructions for getting and Running the Software
* `$ git clone git@github.com:MRLintern/Parallel_Linear_Solver.git`
* `$ cd Parallel_Linear_Solver`
* `$ mkdir build -p && cd build`
* `$ cmake ..`
* `$ cmake --build .`
* `$ ./laPSolver`
* The `.csv` files are saved in a folder called `Results` (within `build`).

## Results
* A `Python` script called `plotter.py` is provided to plot the results. This can be found in the `Results` folder.
* A sample of the results is provided; go to the `Results` directory. You'll find a collated data set and a data set for each algorithm.
* At present this plots the collated data set.
