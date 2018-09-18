#pragma once

#include <Eigen/Core>
#include "Constraints.h"


typedef stree<hnode*> tree_hnode;

class DMAnalyzer {
public:
    DMAnalyzer();
    DMAnalyzer(tree_hnode* tree);
    ~DMAnalyzer();

    void build_constraints();
    void add_constraint();
// private:
    std::vector<Constraints> constraints;
    tree_hnode* shape_tree;
};