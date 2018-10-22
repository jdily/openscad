#pragma once 
// #include "DMSolver.h"
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <iostream>
#include <cmath>
#include "Var.h"
typedef Eigen::SparseMatrix<double> SpMat; // declares a column-major sparse matrix type of double
// typedef Eigen::Triplet<double> T;
class Constraint {
public:
    Constraint();
    ~Constraint();
    int num_eqs() { return _num_eqs; }
    int num_vars() { return _num_vars; }
    virtual void write_jacobian(SpMat *jac_mat, int _row, Eigen::VectorXd pos) = 0;
    virtual void accumulate_enforcement_grad(float step_size, Eigen::VectorXd &grad, Eigen::VectorXd pos) = 0;
    virtual double violate_distance(Eigen::VectorXd pos) = 0;
    // {
        // std::cout << "original contraints class" << std::endl;
    // }
    virtual std::vector<Var*> variables() = 0;
    virtual void save_indices() = 0;
// protected:
    int _num_eqs;
    int _num_vars;
    std::vector<int> _indices;
    // std::vector<Var> _variables;
};

// Equal Number Constraints -> g(\sigma) = a-b
// thinking about if we can add additional constraints for user-specified values
class EqualNumConsts : public Constraint {
public:
    // arguments should be the variable position (idx)
    EqualNumConsts();
    EqualNumConsts(Var* a, Var* b);
    // EqualNumConsts(Var* a, Var* b, double tar_val);
    ~EqualNumConsts();

    void write_jacobian(SpMat *jac_mat, int _row, Eigen::VectorXd pos);
    void accumulate_enforcement_grad(float step_size, Eigen::VectorXd &grad, Eigen::VectorXd pos);
    double violate_distance(Eigen::VectorXd pos);
    void save_indices();
    std::vector<Var*> variables();
    // int num_eqs() { return _num_eqs; }
    // int num_vars() { return _num_vars; }
    Var* _a;
    Var* _b;
    // double _tar_val;
};

class NumDiffConsts : public Constraint {
public:
    NumDiffConsts();
    NumDiffConsts(Var* a, Var* b, Var* _val);
    ~NumDiffConsts();
    void write_jacobian(SpMat *jac_mat, int _row, Eigen::VectorXd pos);
    void accumulate_enforcement_grad(float step_size, Eigen::VectorXd &grad, Eigen::VectorXd pos);
    double violate_distance(Eigen::VectorXd pos);
    void save_indices();
    std::vector<Var*> variables();

    Var* _a;
    Var* _b;
    // double _tar_val;
    Var* _val;
};

class EqualPtsConsts : public Constraint {
public:
    EqualPtsConsts();
    // ws is used to specify which point to align
    EqualPtsConsts(std::vector<Var*> as, std::vector<Var*> bs, std::vector<float> ws_a, std::vector<float> ws_b);
    ~EqualPtsConsts();

    void write_jacobian(SpMat *jac_mat, int _row, Eigen::VectorXd pos);
    void accumulate_enforcement_grad(float step_size, Eigen::VectorXd &grad, Eigen::VectorXd pos);
    double violate_distance(Eigen::VectorXd pos);
    void save_indices();
    std::vector<Var*> variables(); 
    std::vector<Var*> _as;
    std::vector<Var*> _bs;
    std::vector<float> ws_a;
    std::vector<float> ws_b;
};

class ParallelLineConsts : public Constraint {
public:
    ParallelLineConsts();
    ~ParallelLineConsts();
    void write_jacobian(SpMat *jac_mat, int _row, Eigen::VectorXd pos);
    void accumulate_enforcement_grad(float step_size, Eigen::VectorXd &grad, Eigen::VectorXd pos);
    double violate_distance(Eigen::VectorXd pos);
    void save_indices();
    std::vector<Var*> variables();
    
};

class AlignPoint2DConsts : public Constraint {
public:
    AlignPoint2DConsts();
    AlignPoint2DConsts(std::vector<Var*> vas, std::vector<Var*> vbs, std::vector<float> ws_a, std::vector<float> ws_b, std::vector<Var*> measurement);
    AlignPoint2DConsts(std::vector<Var*> v0, std::vector<Var*> v1, std::vector<Var*> measurements);
    ~AlignPoint2DConsts();
    void write_jacobian(SpMat *jac_mat, int _row, Eigen::VectorXd pos);
    void accumulate_enforcement_grad(float step_size, Eigen::VectorXd &grad, Eigen::VectorXd pos);
    void save_indices();
    double violate_distance(Eigen::VectorXd pos);
    std::vector<Var*> variables();
    // std::vector<Var*> _as;
    // std::vector<Var*> _bs;
    std::vector<Var*> ax0_vars;
    std::vector<Var*> ax1_vars;
    std::vector<float> ws_a;
    std::vector<float> ws_b;
    std::vector<Var*> _meas;
    std::vector<int> ax0_indices;
    std::vector<int> ax1_indices;
    std::vector<int> meas_indices;
};