#pragma once 
// #include "DMSolver.h"
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <iostream>

#include "Var.h"
typedef Eigen::SparseMatrix<double> SpMat; // declares a column-major sparse matrix type of double
// typedef Eigen::Triplet<double> T;
class Constraints {
public:
    Constraints();
    ~Constraints();
    int num_eqs() { return _num_eqs; }
    int num_vars() { return _num_vars; }
    virtual void write_jacobian(SpMat *jac_mat, int _row) = 0;
    // {
        // std::cout << "original contraints class" << std::endl;
    // }
protected:
    int _num_eqs;
    int _num_vars;
};

// Equal Number Constraints -> g(\sigma) = a-b
class EqualNumConsts : public Constraints {
public:
    // arguments should be the variable position (idx)
    EqualNumConsts();
    EqualNumConsts(Var a, Var b);
    ~EqualNumConsts();

    void write_jacobian(SpMat *jac_mat, int _row);
    double violate_distance(Eigen::VectorXd pos);
    // int num_eqs() { return _num_eqs; }
    // int num_vars() { return _num_vars; }
    Var _a;
    Var _b;
};

class EqualPtsConsts : public Constraints {
public:
    EqualPtsConsts();
    EqualPtsConsts(std::vector<Var> as, std::vector<Var> bs);
    ~EqualPtsConsts();

    void write_jacobian(SpMat *jac_mat, int _row);

    std::vector<Var> _as;
    std::vector<Var> _bs;
};