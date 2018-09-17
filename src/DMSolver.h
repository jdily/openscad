#pragma once

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/Dense>
#include "iTree.h"


// TODO:
// 1. collect the parameters from the given shape tree as the init state
// 2. check how to represent the constraints
// 3. check how to "pick the constraints"
// 4. make the jacobian from the constraints

typedef stree<hnode*> tree_hnode;

// TODO :
// check later if we need bool type of Vars
struct Var {
public: 
    // the variable id in the list of all variables
    // i.e. the column id
    int _solver_id;
    float _cur_val;
};

// we need to also record the original point of each primitive
// how to get it??

class DMSolver {
public:
    DMSolver();
    DMSolver(tree_hnode* tree);
    ~DMSolver();
    void set_tree(tree_hnode* tree);
    void gather_vars(); 
private:
    tree_hnode* shape_tree;
    Eigen::VectorXd sigma_0;
    int param_count;
    std::vector<Var> all_vars;
};