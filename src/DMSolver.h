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
class DMSolver {
public:
    DMSolver();
    DMSolver(tree_hnode* tree);
    ~DMSolver();
    void set_tree(tree_hnode* tree);
private:
    tree_hnode* shape_tree;
    Eigen::VectorXd sigma_0;
    
};