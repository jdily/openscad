#include "DMSolver.h"
#include "primitives.h"

DMSolver::DMSolver() {}
DMSolver::DMSolver(tree_hnode* tree) : shape_tree(tree) {}
DMSolver::~DMSolver() {}

void DMSolver::set_tree(tree_hnode* tree) {
    shape_tree = tree;
    var_count = 0;
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
    int cur_id = 0;
    while (iterator != shape_tree->end()) {
        std::string type = (*iterator)->type;
        int index = (*iterator)->idx;
        if (type == "poly") {
            std::string poly_type = (*iterator)->node->name();
            std::cout << "poly type : " << poly_type << std::endl;
            const PrimitiveNode *pn = dynamic_cast<const PrimitiveNode*>((*iterator)->node);
            if (poly_type == "cube") {
                Var vx(cur_id, index, pn->x);
                Var vy(cur_id+1, index, pn->y);
                Var vz(cur_id+2, index, pn->z);
                all_vars.push_back(vx);
                all_vars.push_back(vy);
                all_vars.push_back(vz);
                cur_id += 3;
                // params.push_back(pn->x);
                // params.push_back(pn->y);
                // params.push_back(pn->z);
                nids.push_back(index);
                nids.push_back(index);
                nids.push_back(index);
            } else if (poly_type == "sphere") {
                Var vr(cur_id, index, pn->r1);
                all_vars.push_back(vr);
                cur_id += 1;
                // params.push_back(pn->r1);
                nids.push_back(index);
            } else if (poly_type == "cylinder") {
                Var vh(cur_id, index, pn->h);
                Var vr1(cur_id+1, index, pn->r1);
                Var vr2(cur_id+2, index, pn->r2);
                all_vars.push_back(vh);
                all_vars.push_back(vr1);
                all_vars.push_back(vr2);
                cur_id += 3;
                // params.push_back(pn->h);
                // params.push_back(pn->r1);
                // params.push_back(pn->r2);
                nids.push_back(index);
                nids.push_back(index);
                nids.push_back(index);
            } else if (poly_type == "polyhedron") {

            } 
        }
    }
    // record total params length
    var_count = (int)all_vars.size();
    std::vector<double> init_vals;
    for (int i = 0; i < var_count; i++) {
        init_vals.push_back(all_vars[i]._cur_val);
    }
    sigma_0 = Eigen::Map<Eigen::VectorXd>(init_vals.data(), init_vals.size());  
    
}  