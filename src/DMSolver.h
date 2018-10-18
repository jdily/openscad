#pragma once

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include<Eigen/IterativeLinearSolvers>
#include "iTree.h"
// #include "DMAnalyzer.h"
#include "Var.h"
#include "Constraint.h"

// class Constraints;
// class EqualNumConsts;
// TODO:
// 1. collect the parameters from the given shape tree as the init state
// 2. check how to represent the constraints
// 3. check how to "pick the constraints"
// 4. make the jacobian from the constraints

typedef stree<hnode*> tree_hnode;
typedef Eigen::SparseMatrix<double> SpMat; // declares a column-major sparse matrix type of double
typedef Eigen::Triplet<double> T;



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
    void prepare_vars();

    int num_vars();
    // void add_constraints(std::vector<Constraints> conts);
    void add_constraint(Constraint *cont);
    void add_constraints(std::vector<Constraint*> conts);
    
    void add_variable(Var *_v);
    // temp function
    void analyze_constraints();
    // DMAnalyzer *analyzer;
    int num_constraints();

    void load_constraint_jacobian();
    
    void compile();
    void clear();


    Eigen::VectorXd solve_ff(Eigen::VectorXd desired_sigma);
    // Use the gradient of constraints to snap to the constraint manifold
    Eigen::VectorXd snap_constraints(Eigen::VectorXd cur_vals);
    static Eigen::VectorXd pack_vars(tree_hnode* temp_tree);

// private:
    tree_hnode* shape_tree;
    // refer to the Lilicon symbols
    Eigen::VectorXd sigma_0;
    SpMat* jac_mat;

    int var_count;

    // Given node id -> fetch the var ids in the var array
    std::map<int, std::vector<int>> shape_var_dict;
    std::map<int, Eigen::Vector3d> shape_origin_dict;
    std::map<int, hnode*> shape_node_dict;
    std::vector<Constraint*> all_constraints;
    std::vector<Var*> all_vars;
};