module;

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <OsqpEigen/OsqpEigen.h>

export module qpmodule;

import lglmodule;

export namespace eig {
	using matf = Eigen::MatrixXf;
	using vecf = Eigen::VectorXf;

	using matd = Eigen::MatrixXd;
	using vecd = Eigen::VectorXd;

    bool isPSD(const Eigen::MatrixXd& matrix, float tol = 1e-6f) {
        const double dtol = static_cast<double>(tol);
        if (!matrix.isApprox(matrix.transpose(), dtol))
            return false;

        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> es(matrix);
        if (es.info() != Eigen::Success)
            return false;

        return (es.eigenvalues().array() >= -dtol).all();
    }

    void logMatrix(const Eigen::MatrixXd& matrix, bool logSwtich) {
        for (int i = 0; i < matrix.rows(); ++i) {
            for (int j = 0; j < matrix.cols(); ++j) {
                auto element = matrix(i, j);
                lgl::Logger::logIf(logSwtich, lgl::Logger::LGL_EMPTY, "{:.4f}  ", element);
            }
            lgl::Logger::logIf(logSwtich, lgl::Logger::LGL_EMPTY, "\n");
        }
    }

    void logVector(const Eigen::VectorXd& vector, bool logSwtich) {
        for (int i = 0; i < vector.size(); ++i) {
            auto element = vector(i);
            lgl::Logger::logIf(logSwtich, lgl::Logger::LGL_EMPTY, "{:.4f}\n", element);
        }
    }

    void setFromFloatAt(Eigen::MatrixXd& matrix, int row, int col, float value) {
        matrix(row, col) = static_cast<double>(value);
	}

    float getAsFloatAt(const Eigen::MatrixXd& matrix, int row, int col) {
        return static_cast<float>(matrix(row, col));
	}

    void setFromFloatAt(Eigen::VectorXd& vector, int index, float value) {
        vector(index) = static_cast<double>(value);
    }

    float getAsFloatAt(const Eigen::VectorXd& vector, int index) {
        return static_cast<float>(vector(index));
    }
}

export namespace qp {
    Eigen::SparseMatrix<double> denseToSparseUpperSymmetric(const Eigen::MatrixXd& dense) {
        const int n = static_cast<int>(dense.rows());

        std::vector<Eigen::Triplet<double>> triplets;
        triplets.reserve(n * n);

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                const double value = dense(i, j);
                if (std::abs(value) > 1e-12) {
                    triplets.emplace_back(i, j, value);
                }
            }
        }

        Eigen::SparseMatrix<double> sparse(n, n);
        sparse.setFromTriplets(triplets.begin(), triplets.end());
        sparse.makeCompressed();
        return sparse;
    }

    bool solveBaraffContactForces(const Eigen::MatrixXd& contactA, const Eigen::VectorXd& contactB, Eigen::VectorXd& outForces) {
        const int n = static_cast<int>(contactB.size());

        if (contactA.rows() != n || contactA.cols() != n) {
			lgl::Logger::log(lgl::Logger::LGL_WARN, "Invalid contact matrix size: expected {}x{}, got {}x{}.", n, n, contactA.rows(), contactA.cols());
            return false;
        }

        Eigen::MatrixXd P_dense = 0.5 * (contactA + contactA.transpose());

        const double regularization = 1e-9;
        P_dense.diagonal().array() += regularization;

        Eigen::VectorXd q = contactB;

        Eigen::SparseMatrix<double> P = denseToSparseUpperSymmetric(P_dense);

        Eigen::SparseMatrix<double> C(n, n);
        C.setIdentity();
        C.makeCompressed();

        Eigen::VectorXd lowerBound = Eigen::VectorXd::Zero(n);
        Eigen::VectorXd upperBound = Eigen::VectorXd::Constant(n, OsqpEigen::INFTY);

        OsqpEigen::Solver solver;

        solver.settings()->setVerbosity(false);
        solver.settings()->setWarmStart(true);

        solver.settings()->setAbsoluteTolerance(1e-5);
        solver.settings()->setRelativeTolerance(1e-5);
        solver.settings()->setMaxIteration(100);

        solver.data()->setNumberOfVariables(n);
        solver.data()->setNumberOfConstraints(n);

        if (!solver.data()->setHessianMatrix(P)) {
            lgl::Logger::log(lgl::Logger::LGL_WARN, "Failed to set Hessian matrix.");
            return false;
        }

        if (!solver.data()->setGradient(q)) {
            lgl::Logger::log(lgl::Logger::LGL_WARN, "Failed to set gradient vector.");
            return false;
        }

        if (!solver.data()->setLinearConstraintsMatrix(C)) {
            lgl::Logger::log(lgl::Logger::LGL_WARN, "Failed to set constraint matrix.");
            return false;
        }

        if (!solver.data()->setLowerBound(lowerBound)) {
            lgl::Logger::log(lgl::Logger::LGL_WARN, "Failed to set lower bound.");
            return false;
        }

        if (!solver.data()->setUpperBound(upperBound)) {
            lgl::Logger::log(lgl::Logger::LGL_WARN, "Failed to set upper bound.");
            return false;
        }

        if (!solver.initSolver()) {
            lgl::Logger::log(lgl::Logger::LGL_WARN, "Failed to initialize OSQP solver.");
            return false;
        }

        const OsqpEigen::ErrorExitFlag solveResult = solver.solveProblem();
        if (solveResult != OsqpEigen::ErrorExitFlag::NoError) {
            lgl::Logger::log(lgl::Logger::LGL_WARN, "OSQP failed to solve problem (error flag: {}).", static_cast<int>(solveResult));
            return false;
        }

        outForces = solver.getSolution();
        return true;
    }
}