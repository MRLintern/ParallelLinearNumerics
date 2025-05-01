// -- parallel solver class with red-black ordering due to the sequential nature of the Gauss-Seidel and SOR methods; NOT for Jacobi method
// -- purpose: to solve systems of the form: Ax = b, and to measure convergence rates of the Jacobi, Gauss-Seidel and SOR methods

#pragma once

#include <vector>
#include <string>
#include <Eigen/Dense>

// Linear Parallel Solver
class LPSolver {

protected:

	// matrix A; cofficients of algebraic equations
	Eigen::MatrixXd A_; 

	// column vector b; values to known
	Eigen::VectorXd b_;

	// error tolerence level
	double tolerance_{0.0};

	// maximum number of iterations to perform
	int maxIterations_{0};

	// relaxation factor; for SOR method only
	double relaxFactor_{0.0};

	// log and plot the convergence rates via this function; Jacobi & Gauss-Seidel algorithms
	void logPlotConvergence(const std::vector<double>& residuals, const std::string& method) const;

	// log and plot the convergence rates via this function; for SOR algorithm
	void logPlotConvergence(const std::vector<double>& residuals, const std::string& method, double relaxFactor) const;

public:

	// constructor; initialise data members
	LPSolver(const Eigen::MatrixXd& A, const Eigen::VectorXd& b, double tolerance, int maxIterations, double relaxFactor = 1.0);

	// function to solve the system; implemented by inherited/child classes
	virtual Eigen::VectorXd solve() = 0;

};

// -- iterative solvers: Parallel Jacobi, Parallel Gauss-Seidel (red-black) & Parallel SOR (red-black)

// Jacobi method; doesn't require red-black ordering; not sequential by nature
class PJacobi : public LPSolver {

public:

	// inherit the constructor from parent class
	using LPSolver::LPSolver;

	// overide solve function; needs overiding for Jacobi method to be implemented
	Eigen::VectorXd solve() override;

};

// Gauss-Seidel method with red-black ordering for better parallelism
class PGaussSeidel : public LPSolver {

public:

	// inherit the constructor from parent class
	using LPSolver::LPSolver;

	// overide solve function; needs overiding for Gauss-Seidel method to be implemented
	Eigen::VectorXd solve() override;

};

// SOR method with red-black ordering for better parallelism
class PSOR : public LPSolver {

public:

	// inherit the constructor from parent class
	using LPSolver::LPSolver;

	// overide solve function; needs overiding for SOR method to be implemented
	Eigen::VectorXd solve() override;
};

// end of LPSolver class
