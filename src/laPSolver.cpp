// implement the LPSolver class

#include <iostream>
#include <fstream>
#include <filesystem> // facilities for performing operations on file systems and their components e.g. file paths etc
#include <chrono> // for timing convergence of methods
#include <ranges> // for range based for loops and views; C++20 features
#include <vector>
#include <omp.h> // for OpenMP
#include "laPSolver.hpp"

// for saving CSV output data to different folder/directory
namespace fs = std::filesystem;

// implement constructor to initialise LPSolver data members
LPSolver::LPSolver(const Eigen::MatrixXd& A, const Eigen::VectorXd& b, double tolerance, int maxIterations, double relaxFactor)

	: A_(A), b_(b), tolerance_(tolerance), maxIterations_(maxIterations), relaxFactor_(relaxFactor) {}


void LPSolver::logPlotConvergence(const std::vector<double>& residuals, const std::string& method) const {

    if (residuals.empty()) {
        std::cerr<<"WARNING: No residuals found for "<<method<<"\n";
        return;
    }

    const std::string resultsDir { "Results" };

    if (!fs::exists(resultsDir)) {
        if (!fs::create_directory(resultsDir)) {
            std::cerr<<"ERROR: Failed to Create Directory "<<resultsDir<<"\n";
            return;
        }
    }

    std::string filename { resultsDir + "/" + method + "_convergenceRates.csv" };
    std::ofstream outFile(filename);

    if (!outFile.is_open()) {
        std::cerr<<"ERROR: Could Not Open File "<<filename<<" for Writing\n";
        return;
    }

    outFile <<"Iteration Residual"<<"\n";

    for (size_t i{0}; i < residuals.size(); ++i) {
        
        outFile <<i<<" "<<residuals[i]<<"\n";
    }

    outFile.close();
    std::cout<<"Convergence Residuals Saved to "<<filename<<"\n";
}
// -- iterative solvers

// -- parallel Jacobi method
Eigen::VectorXd PJacobi::solve() {

	// rows of coefficient matrix A
	int n = A_.rows();

	// column vector for solution vector x; initialised to 0 initially
	Eigen::VectorXd x = Eigen::VectorXd::Zero(n);

	// column vector for updated solution vector x; initialised to 0 initially
	Eigen::VectorXd x_updated = Eigen::VectorXd::Zero(n);

	// vector to hold the residuals for plotting convergence
	std::vector<double> residuals;

	// start timing for convergence
	auto start { std::chrono::steady_clock::now() };

	// -- main iteration loop

	for (int k{0}; k < maxIterations_; ++k) {

		// parallel for loop for implementing OpenMP
		#pragma omp parallel for
		for (int i{0}; i < n; ++i) {

			// summation; for summing up values of x
			double summation {0.0};

			// -- now perform Ax; that is, sum the elements of matrix A * by elements of vector x
			for (auto j : std::views::iota(0, n)) {

				// perform the summation providing [rows X cols] * cols
				if (j != i) { summation += A_(i, j) * x(j); }
			}

			// Jacobi formula; updated values for solution vector
			x_updated(i) = (b_(i) - summation) / A_(i, i);
		}

		// store new updated values in the solution vector x
		x = x_updated;

		// calculate residuals via Euclidean norm
		double res { (A_ * x - b_).norm() };

		// store calculated residuals for plotting
		residuals.push_back(res);

		// check for convergence
		if (res < tolerance_) { break; } // once convergence is reached, stop
	}

	// stop timing for convergence
	auto end { std::chrono::steady_clock::now() };

	// output time taken by parallel Jacobi method
	std::cout<<"The Parallel Jacobi Method took "<<std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()<<"ms\n";

	// plot convergence rate for parallel Jacobi method
	logPlotConvergence(residuals, "Parallel-Jacobi");

	// return final computed value of vector x
	return x;
}

// -- parallel Gauss-Seidel method with red-black ordering scheme
Eigen::VectorXd PGaussSeidel::solve() {

	// number of rows of coefficient matrix A
	int n = A_.rows();

	// solution vector; initially initialised with 0's
	Eigen::VectorXd x = Eigen::VectorXd::Zero(n);

	// vector to hold the residuals for plotting convergence
	std::vector<double> residuals;

	// start timing for convergence
	auto start { std::chrono::steady_clock::now() };

	// -- main iteration loop

	for (int k{0}; k < maxIterations_; ++k) {

		// red points; even indices and parallelise red point updates
		#pragma omp	parallel for
		
		for (int i{0}; i < n; ++i) {

			// check index is even; if so, proceed with summation etc...
			if (i % 2 == 0) {

				// summation; for summing up values of x
				double summation {0.0};

				// -- now perform Ax; that is, sum the elements of matrix A * by elements of vector x
				for (auto j : std::views::iota(0, n)) {

					// perform the summation providing [rows X cols] * cols
					if (j != i) { summation += A_(i, j) * x(j); }
				}

				// update x(i) using the Gauss-Seidel formula
				x(i) = (b_(i) - summation) / A_(i, i);
			}
		}

		// update black points; odd indices and parallelise black point updates
		#pragma omp parallel for
		for (int i{0}; i < n; ++i) {

			// check index is odd
			if (i % 2 == 1) {

				// summation; for summing up values of x
				double summation {0.0};

				// -- now perform Ax; that is, sum the elements of matrix A * by elements of vector x
				for (auto j : std::views::iota(0, n)) {

					// perform the summation providing [rows X cols] * cols
					if (j != i) summation += A_(i, j) * x(j);
				}

				// update x(i) using Gauss-Seidel formula
				x(i) = (b_(i) - summation) / A_(i, i);
			}
		}

		// calculate residuals via Euclidean norm
		double res { (A_ * x - b_).norm() };

		// store calculated residuals for plotting
		residuals.push_back(res);

		// check for convergence
		if (res < tolerance_) { break; }

	}

	// stop timing for convergence
	auto end { std::chrono::steady_clock::now() };

	// output time taken by parallel Gauss-Seidel method
	std::cout<<"The Parallel Gauss-Seidel Method took "<<std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()<<"ms\n";

	// plot convergence rate for parallel Gauss-Seidel method
	logPlotConvergence(residuals, "Parallel-Gauss-Seidel");

	// return final computed value of vector x
	return x;
}

// -- parallel SOR method with red-black ordering

Eigen::VectorXd PSOR::solve() {

	// get the number of rows of coefficient matrix A
	int n = A_.rows();

	// solution vector; initially initialised with 0's
	Eigen::VectorXd x = Eigen::VectorXd::Zero(n);

	// vector to hold the residuals for plotting convergence
	std::vector<double> residuals;

	// start timing for convergence
	auto start { std::chrono::steady_clock::now() };

	// -- main iteration loop

	for (int k{0}; k < maxIterations_; ++k) {

		// update red points; even indices; parallelise red points updated
		#pragma omp parallel for
		for (int i{0}; i < n; ++i) {

			// make sure index is even; if so, proceed with summation etc...
			if (i % 2 == 0) {

				// summation; for summing up values of x
				double summation {0.0};

				// -- now perform Ax; that is, sum the elements of matrix A * by elements of vector x
				for (int j{0}; j < n; ++j) {

					// perform the summation providing [rows X cols] * cols
					if (j != i) { summation += A_(i, j) * x(j); }

				}

				// update x(i) using SOR formula
				x(i) = (1 - relaxFactor_) * x(i) + relaxFactor_ * (b_(i) - summation) / A_(i, i);
			}
		}

		// update black points; odd indices; parallelise updated black points
		#pragma omp parallel for 
		for (int i{0}; i < n; ++i) {

			// make sure index is odd; if so, proceed with summation etc...
			if (i % 2 == 1) {

				// summation; for summing up values of x
				double summation {0.0};

				// -- now perform Ax; that is, sum the elements of matrix A * by elements of vector x
				for (int j{0}; j < n; ++j) {

					// perform the summation providing [rows X cols] * cols
					if (j != i) { summation += A_(i, j) * x(j); }

				}

				// update x(i) using SOR formula
				x(i) = (1 - relaxFactor_) * x(i) + relaxFactor_ * (b_(i) - summation) / A_(i, i);
			}
		}

		// stop timing for convergence
		double res { (A_ * x - b_).norm() };

		// store calculated residuals for plotting
		residuals.push_back(res);

		// check for convergence
		if (res < tolerance_) { break; }
	}

	// stop timing for convergence
	auto end { std::chrono::steady_clock::now() };

	// output time taken by parallel Gauss-Seidel method
	std::cout<<"The Parallel SOR Method took "<<std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()<<"ms\n";

	// plot convergence rate for parallel Gauss-Seidel method
	logPlotConvergence(residuals, "Parallel-SOR");

	// return final computed value of vector x
	return x;
}


