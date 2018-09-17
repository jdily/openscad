#include "DMSolver.h"
#include "primitives.h"

DMSolver::DMSolver() {}
DMSolver::DMSolver(tree_hnode* tree) : shape_tree(tree) {}
DMSolver::~DMSolver() {}

void DMSolver::set_tree(tree_hnode* tree) {
    shape_tree = tree;
    all_vars.clear();
}

void DMSolver::gather_vars() {
    if (shape_tree == nullptr) {
        return ;
    }
    // navigate the shape tree
    tree_hnode::sibling_iterator children;
	tree_hnode::iterator iterator;
    iterator = shape_tree->begin();
    std::vector<double> params;
    std::vector<int> nids;
    std::vector<Var> vars;
    while (iterator != shape_tree->end()) {
        std::string type = (*iterator)->type;
        int index = (*iterator)->idx;
        if (type == "poly") {
            std::string poly_type = (*iterator)->node->name();
            std::cout << "poly type : " << poly_type << std::endl;
            const PrimitiveNode *pn = dynamic_cast<const PrimitiveNode*>((*iterator)->node);
            if (poly_type == "cube") {
                params.push_back(pn->x);
                params.push_back(pn->y);
                params.push_back(pn->z);
                nids.push_back(index);
                nids.push_back(index);
                nids.push_back(index);
            } else if (poly_type == "sphere") {
                params.push_back(pn->r1);
                nids.push_back(index);
            } else if (poly_type == "cylinder") {
                params.push_back(pn->h);
                params.push_back(pn->r1);
                params.push_back(pn->r2);
                nids.push_back(index);
                nids.push_back(index);
                nids.push_back(index);
            } else if (poly_type == "polyhedron") {

            } 
        }
    }
    // record total params length
    param_count = (int)params.size();
    sigma_0 = Eigen::Map<Eigen::VectorXd>(params.data(), params.size()); 
    
}  