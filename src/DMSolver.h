#pragma once

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/Dense>
#include <Eigen/Sparse>

#include "iTree.h"
// #include "Constraints.h"

class Constraints;
// TODO:
// 1. collect the parameters from the given shape tree as the init state
// 2. check how to represent the constraints
// 3. check how to "pick the constraints"
// 4. make the jacobian from the constraints

typedef stree<hnode*> tree_hnode;
typedef Eigen::SparseMatrix<double> SpMat; // declares a column-major sparse matrix type of double
typedef Eigen::Triplet<double> T;

// TODO :
// check later if we need bool type of Vars
struct Var {
public: 
    Var() {}
    Var(int solver_id, int shape_id, double cur_val) {
        _solver_id = solver_id;
        _shape_id = shape_id;
        _cur_val = cur_val;
    }
    // the variable id in the list of all variables
    // i.e. the column id
    int _solver_id;
    double _cur_val;
    int _shape_id;
};

// struct ShapeVars {

// }

// we need to also record the original point of each primitive
// how to get it??

class DMSolver {
public:
    DMSolver();
    DMSolver(tree_hnode* tree);
    ~DMSolver();
    void set_tree(tree_hnode* tree);
    void gather_vars(); 

    void add_constraints(std::vector<Constraints> conts);
    void add_constraint(Constraints cont);


// private:
    tree_hnode* shape_tree;
    // refer to the Lilicon symbols
    Eigen::VectorXd sigma_0;
    SpMat jac_mat;

    int var_count;
    std::vector<Var> all_vars;
    std::map<int, std::vector<int>> shape_var_dict;
};