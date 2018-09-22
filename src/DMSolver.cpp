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
                std::vector<int> var_ids;
                var_ids.push_back(cur_id);
                var_ids.push_back(cur_id+1);
                var_ids.push_back(cur_id+2);
                shape_var_dict.insert(std::pair<int, std::vector<int>>(index, var_ids));
                cur_id += 3;
                nids.push_back(index);
                nids.push_back(index);
                nids.push_back(index);
            } else if (poly_type == "sphere") {
                Var vr(cur_id, index, pn->r1);
                all_vars.push_back(vr);
                std::vector<int> var_ids;
                var_ids.push_back(cur_id);
                shape_var_dict.insert(std::pair<int, std::vector<int>>(index, var_ids));
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
                std::vector<int> var_ids;
                var_ids.push_back(cur_id);
                var_ids.push_back(cur_id+1);
                var_ids.push_back(cur_id+2);
                shape_var_dict.insert(std::pair<int, std::vector<int>>(index, var_ids));
                cur_id += 3;
                nids.push_back(index);
                nids.push_back(index);
                nids.push_back(index);
            } else if (poly_type == "polyhedron") {

            } 
        }
        ++iterator;
    }
    // record total params length
    var_count = (int)all_vars.size();
    std::vector<double> init_vals;
    for (int i = 0; i < var_count; i++) {
        init_vals.push_back(all_vars[i]._cur_val);
    }
    sigma_0 = Eigen::Map<Eigen::VectorXd>(init_vals.data(), init_vals.size());  
}  

int DMSolver::num_vars() { return var_count; }
int DMSolver::num_constraints() { return (int)all_constraints.size(); }

// tmp fixed function for constraints.
void DMSolver::analyze_constraints() {
    int shape0 = 3; // 4;
    int shape1 = 6; //10;
    // x
    int s0_id = shape_var_dict[shape0][0];
    int s1_id = shape_var_dict[shape1][0];
    std::cout << "s0_x_id : " << s0_id << std::endl;
    std::cout << "s1_x_id : " << s1_id << std::endl;
    EqualNumConsts *encont = new EqualNumConsts(all_vars[s0_id], all_vars[s1_id]);
    all_constraints.push_back(encont);
    // y
    s0_id = shape_var_dict[shape0][1];
    s1_id = shape_var_dict[shape1][1];
    std::cout << "s0_x_id : " << s0_id << std::endl;
    std::cout << "s1_x_id : " << s1_id << std::endl;  
    EqualNumConsts *encout2 = new EqualNumConsts(all_vars[s0_id], all_vars[s1_id]);
    all_constraints.push_back(encout2);
}

void DMSolver::load_constraint_jacobian() {
    if (jac_mat == nullptr) {
        jac_mat = new SpMat(all_constraints.size(), var_count);
        // should be 0
        std::cout << "init, nonzero count : " << jac_mat->nonZeros() << std::endl; 
        int row_i = 0;
        for (int i = 0; i < num_constraints(); i++) {
            std::cout << i << " add constraint jacobian " << std::endl;
            all_constraints[i]->write_jacobian(jac_mat, row_i);  
            row_i += all_constraints[i]->num_eqs();
        }
        std::cout << "after, nonzero count : " << jac_mat->nonZeros() << std::endl; 
    }
}
// int DMSolver::var_c

// desired_sigma -> user-specified
void DMSolver::solve_ff(Eigen::VectorXd desired_sigma) {
    Eigen::VectorXd ideal_sigma = Eigen::VectorXd::Zero(var_count);
    Eigen::VectorXd delta = desired_sigma - sigma_0;
    Eigen::VectorXd rhs = Eigen::VectorXd::Zero(all_constraints.size());
    rhs = (*jac_mat)*sigma_0;
    // negate.
    rhs *= -1.0;
    // lagrange_multiplier
    Eigen::VectorXd lag_multi = Eigen::VectorXd::Zero(all_constraints.size());
    // solve for X: (A * A^t) * X = B
    SpMat Jt = (jac_mat->transpose());
    SpMat JJt = (*jac_mat)*(Jt);
    Eigen::ConjugateGradient<SpMat, Eigen::Upper> cgsolver;
    lag_multi = cgsolver.compute(JJt).solve(rhs);
    ideal_sigma = delta - Jt*lag_multi;
    // closest output 
    Eigen::VectorXd out = sigma_0 + ideal_sigma;
}