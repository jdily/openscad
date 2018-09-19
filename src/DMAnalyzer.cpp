#include "DMAnalyzer.h"

DMAnalyzer::DMAnalyzer() {

}

DMAnalyzer::DMAnalyzer(tree_hnode* tree) {
    shape_tree = tree;
}

DMAnalyzer::~DMAnalyzer() {

}

// go through the poly node and form the constraints.
void DMAnalyzer::build_constraints() {

}

void DMAnalyzer::add_constraint() {
    // int shape0 = 2;
    // int shape1 = 4;
    // int s0_r_id = shape_var_dict[shape0][0];
    // int s1_r_id = shape_var_dict[shape1][0];
    // EqualNumConsts encont(all_vars[s0_r_id], all_vars[s1_r_id]);
    // constraints.push_back(encont);
}