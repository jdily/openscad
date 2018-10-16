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
    virtual void write_jacobian(SpMat *jac_mat, int _row, Eigen::VectorXd pos) = 0;
    virtual void accumulate_enforcement_grad(float step_size, Eigen::VectorXd &grad, Eigen::VectorXd pos) = 0;
    // {
        // std::cout << "original contraints class" << std::endl;
    // }
protected:
    int _num_eqs;
    int _num_vars;
};

// Equal Number Constraints -> g(\sigma) = a-b
// thinking about if we can add additional constraints for user-specified values
class EqualNumConsts : public Constraints {
public:
    // arguments should be the variable position (idx)
    EqualNumConsts();
    EqualNumConsts(Var a, Var b);
    EqualNumConsts(Var a, Var b, double tar_val);
    ~EqualNumConsts();

    void write_jacobian(SpMat *jac_mat, int _row, Eigen::VectorXd pos);
    void accumulate_enforcement_grad(float step_size, Eigen::VectorXd &grad, Eigen::VectorXd pos);
    double violate_distance(Eigen::VectorXd pos);
    // int num_eqs() { return _num_eqs; }
    // int num_vars() { return _num_vars; }
    Var _a;
    Var _b;
    double _tar_val;
};

class EqualPtsConsts : public Constraints {
public:
    EqualPtsConsts();
    // ws is used to specify which point to align
    EqualPtsConsts(std::vector<Var> as, std::vector<Var> bs, std::vector<float> ws_a, std::vector<float> ws_b);
    ~EqualPtsConsts();

    void write_jacobian(SpMat *jac_mat, int _row, Eigen::VectorXd pos);
    void accumulate_enforcement_grad(float step_size, Eigen::VectorXd &grad, Eigen::VectorXd pos);
    std::vector<Var> _as;
    std::vector<Var> _bs;
    std::vector<float> ws_a;
    std::vector<float> ws_b;
};

class ParallelLineConsts : public Constraints {
public:
    ParallelLineConsts();
    ~ParallelLineConsts();
    void write_jacobian(SpMat *jac_mat, int _row, Eigen::VectorXd pos);
    void accumulate_enforcement_grad(float step_size, Eigen::VectorXd &grad, Eigen::VectorXd pos);

    
};