// driver; create the system Ax = b

#include <iostream>
#include <Eigen/Dense>
#include "laPSolver.hpp"

int main() {

	// size of matrix A
	const int n {200};

	// create a diagonally dominant (sparse-like) matrix A with random integers; can always create a 200 x 200 matrix; time consuming!
	Eigen::MatrixXd A = Eigen::MatrixXd::Random(n, n);

	// transpose matrix A: m x n matrix; A^T: n x m matrix; so A^T*A: m x m matrix
	A = A.transpose() * A;

	// add n to every diagonal element of matrix A
	A.diagonal().array() += n;

	// rhs of system; column vector b; needs to have 100 elements
	Eigen::VectorXd b = Eigen::VectorXd::Random(n);

	// -- parameters for "controlling" the system

	// level of error acceptance
	double tolerance {1e-6};

	// maximum number of iterations to perform
	int maxIterations {10000};

	// relaxation factor (w) for the SOR method; Note: values chosen experimentally: 0 < w < 2; if w = 1, the SOR method = Gauss-Seidel method
	double relaxFactor {1.7};

	// -- create solver objects and solve them to find vector x

	// parallel Jacobi method object/system
	PJacobi pjacobi(A, b, tolerance, maxIterations);

	// solve parallel jacobi system
	pjacobi.solve();

	// parallel Gauss-Seidel (red-black) method object/system
	PGaussSeidel pgs(A, b, tolerance, maxIterations);

	// solve parallel Gauss-Seidel object/system
	pgs.solve();

	// parallel SOR (red-black) method object/system
	PSOR psor(A, b, tolerance, maxIterations, relaxFactor);

	// solve parallel SOR object/system
	psor.solve();

}
