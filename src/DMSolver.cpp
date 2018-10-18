#include "DMSolver.h"
#include "primitives.h"
#include "ModuleInstantiation.h"

DMSolver::DMSolver() {}
DMSolver::DMSolver(tree_hnode* tree) : shape_tree(tree) {}
DMSolver::~DMSolver() {}

void DMSolver::set_tree(tree_hnode* tree) {
    shape_tree = tree;
    var_count = 0;
    all_vars.clear();
}

void DMSolver::prepare_vars() {
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
        // if (index == 4) {
        //     break;
        // }
        if (type == "poly") {
            std::string poly_type = (*iterator)->node->name();
            std::cout << "poly type : " << poly_type << std::endl;
            // check the Location things.
            // seems like work for non-module code, i.e. direct geometry declare.
            Location loc = (*iterator)->node->modinst->location();
            std::cout << "show the location of node " << index << std::endl;
            std::cout << loc.firstLine() << " " << loc.firstColumn() << " " << loc.lastLine() << " " << loc.lastColumn() << std::endl; 
            Eigen::Matrix4d m = (*iterator)->transform.matrix();
            std::cout << m << std::endl;
            Eigen::Vector4d o(0.0, 0.0, 0.0, 1.0);
            o = m*o;
            o[0] /= o[3];
            o[1] /= o[3];
            o[2] /= o[3];
            Eigen::Vector3d cur_o = o.head(3);
            std::cout << cur_o << std::endl;
            shape_origin_dict.insert(std::pair<int, Eigen::Vector3d>(index, cur_o));

            const PrimitiveNode *pn = dynamic_cast<const PrimitiveNode*>((*iterator)->node);
            // Check here.
            if (poly_type == "cube") {
                Var *vx = new Var(index, pn->x, false);
                Var *vy = new Var(index, pn->y, false);
                Var *vz = new Var(index, pn->z, false);
                (*iterator)->var_dict.insert(std::pair<std::string, Var*>("x", vx));
                (*iterator)->var_dict.insert(std::pair<std::string, Var*>("y", vy));
                (*iterator)->var_dict.insert(std::pair<std::string, Var*>("z", vz));
            } else if (poly_type == "sphere") {
                Var *vr = new Var(index, pn->r1, false);
                (*iterator)->var_dict.insert(std::pair<std::string, Var*>("r1", vr));
            } else if (poly_type == "cylinder") {
                Var *vh = new Var(index, pn->h, false);
                Var *vr1 = new Var(index, pn->r1, false);
                Var *vr2 = new Var(index, pn->r2, false);
                (*iterator)->var_dict.insert(std::pair<std::string, Var*>("h", vh));
                (*iterator)->var_dict.insert(std::pair<std::string, Var*>("r1", vr1));
                (*iterator)->var_dict.insert(std::pair<std::string, Var*>("r2", vr2));
            } else if (poly_type == "polyhedron") {

            }
            shape_node_dict.insert(std::pair<int, hnode*>(index, (*iterator)));
        }
        
        ++iterator;
    }
    for (std::map<int,hnode*>::iterator it=shape_node_dict.begin(); it!=shape_node_dict.end(); ++it) {
        std::cout << "index : " << it->first << std::endl;
        // if (it->first)
        std::cout << it->second->var_dict["x"]->_shape_id << std::endl;
        std::cout << it->second->var_dict["x"]->_solver_id << std::endl;
        std::cout << it->second->var_dict["x"]->_cur_val << std::endl;
    }


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
            // check the Location things.
            // seems like work for non-module code, i.e. direct geometry declare.
            Location loc = (*iterator)->node->modinst->location();
            std::cout << "show the location of node " << index << std::endl;
            std::cout << loc.firstLine() << " " << loc.firstColumn() << " " << loc.lastLine() << " " << loc.lastColumn() << std::endl; 

            Eigen::Matrix4d m = (*iterator)->transform.matrix();
            // std::cout << m << std::endl;
            Eigen::Vector4d o(0.0, 0.0, 0.0, 1.0);
            o = m*o;
            o[0] /= o[3];
            o[1] /= o[3];
            o[2] /= o[3];
            Eigen::Vector3d cur_o = o.head(3);
            std::cout << cur_o << std::endl;
            shape_origin_dict.insert(std::pair<int, Eigen::Vector3d>(index, cur_o));

            const PrimitiveNode *pn = dynamic_cast<const PrimitiveNode*>((*iterator)->node);
            if (poly_type == "cube") {
                Var vx(cur_id, index, pn->x);
                Var vy(cur_id+1, index, pn->y);
                Var vz(cur_id+2, index, pn->z);
                all_vars.push_back(&vx);
                all_vars.push_back(&vy);
                all_vars.push_back(&vz);
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
                all_vars.push_back(&vr);
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
                all_vars.push_back(&vh);
                all_vars.push_back(&vr1);
                all_vars.push_back(&vr2);
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
        init_vals.push_back(all_vars[i]->_cur_val);
    }
    sigma_0 = Eigen::Map<Eigen::VectorXd>(init_vals.data(), init_vals.size());  
}  

// [TODO]
// func to packing the current vals of variables 
// used for getting editor updated values -> compute force vector
// Return : force vector
Eigen::VectorXd DMSolver::pack_vars(tree_hnode* temp_tree) {
    Eigen::VectorXd edited_vars;
    if (temp_tree == nullptr) {
        return edited_vars;
    }
    // navigate the shape tree
    tree_hnode::sibling_iterator children;
	tree_hnode::iterator iterator;
    iterator = temp_tree->begin();
    std::vector<double> params;
    std::vector<int> nids;
    std::vector<Var> vars;
    int cur_id = 0;
    std::vector<double> edited_vals;
    while (iterator != temp_tree->end()) {
        std::string type = (*iterator)->type;
        int index = (*iterator)->idx;
        if (type == "poly") {
            std::string poly_type = (*iterator)->node->name();
            // std::cout << "poly type : " << poly_type << std::endl;
            // check the Location things.
            // seems like work for non-module code, i.e. direct geometry declare.
            Location loc = (*iterator)->node->modinst->location();
            std::cout << "show the location of node " << index << std::endl;
            std::cout << loc.firstLine() << " " << loc.firstColumn() << " " << loc.lastLine() << " " << loc.lastColumn() << std::endl; 

            const PrimitiveNode *pn = dynamic_cast<const PrimitiveNode*>((*iterator)->node);
            if (poly_type == "cube") {
                edited_vals.push_back(pn->x);
                edited_vals.push_back(pn->y);
                edited_vals.push_back(pn->z);
                cur_id += 3;
            } else if (poly_type == "sphere") {
                edited_vals.push_back(pn->r1);
                cur_id += 1;
            } else if (poly_type == "cylinder") {
                edited_vals.push_back(pn->h);
                edited_vals.push_back(pn->r1);
                edited_vals.push_back(pn->r2);
                cur_id += 1;
            } else if (poly_type == "polyhedron") {

            } 
        }
        ++iterator;
    }
    edited_vars = Eigen::Map<Eigen::VectorXd>(edited_vals.data(), edited_vals.size());  
    return edited_vars;
}
void DMSolver::clear() {
    all_constraints.clear();
    all_vars.clear();
}

void DMSolver::compile() {
    var_count = (int)all_vars.size();
    int n_constraints = all_constraints.size();
    for (int c = 0; c < n_constraints; c++) {
        all_constraints[c]->save_indices();
    }
}

int DMSolver::num_vars() { return var_count; }
int DMSolver::num_constraints() { return (int)all_constraints.size(); }

// tmp fixed function for constraints.
void DMSolver::analyze_constraints() {
    int shape0 = 2; // 4;
    int shape1 = 4; //10;

    // 
    std::cout << "2 : " << shape_node_dict[shape0]->idx << std::endl;
    std::cout << "4 : " << shape_node_dict[shape1]->idx << std::endl;

    // test
    std::cout << "2_x : " << shape_node_dict[shape0]->var_dict["x"]->_shape_id << std::endl;
    std::cout << "2_x solver : " << shape_node_dict[shape0]->var_dict["x"]->_solver_id << std::endl;

    std::cout << "4_x : " << shape_node_dict[shape1]->var_dict["x"]->_shape_id << std::endl;
    std::cout << "4_x solver : " << shape_node_dict[shape1]->var_dict["x"]->_solver_id << std::endl;


    EqualNumConsts *cont1 = new EqualNumConsts(shape_node_dict[shape0]->var_dict["x"], shape_node_dict[shape1]->var_dict["x"]);
    this->add_constraint(cont1);

    std::cout << "num of variable : " << this->all_vars.size() << std::endl;
    // for (int i = 0; i < this->all_vars.size(); i++) {
        // std::cout << all_vars[i]->_solver_id << std::endl;
    // }
    std::cout << all_vars[0]->_shape_id << std::endl;
    std::cout << all_vars[1]->_shape_id << std::endl;


    // // x
    // int s0_id = shape_var_dict[shape0][0];
    // int s1_id = shape_var_dict[shape1][0];
    // std::cout << "s0_x_id : " << s0_id << std::endl;
    // std::cout << "s1_x_id : " << s1_id << std::endl;
    // EqualNumConsts *encont = new EqualNumConsts(all_vars[s0_id], all_vars[s1_id]);
    // all_constraints.push_back(encont);
    // // y
    // s0_id = shape_var_dict[shape0][1];
    // s1_id = shape_var_dict[shape1][1];
    // std::cout << "s0_x_id : " << s0_id << std::endl;
    // std::cout << "s1_x_id : " << s1_id << std::endl;  
    // EqualNumConsts *encout2 = new EqualNumConsts(all_vars[s0_id], all_vars[s1_id]);
    // all_constraints.push_back(encout2);
}

void DMSolver::add_variable(Var *_v) {
    int cur_idx = (int)this->all_vars.size();
    std::cout << "cur_idx : " << cur_idx << std::endl;
    _v->_solver_id = cur_idx;
    std::cout << "v solver id : " << _v->_solver_id << std::endl;
    std::cout << "v shape id : " << _v->_shape_id << std::endl;
    this->all_vars.push_back(_v);
}

// add variable at the same time...
void DMSolver::add_constraint(Constraint *_const) {
    all_constraints.push_back(_const);
    for (auto V : _const->variables()) {
    // for (int i = 0; i < _const->num_vars(); i++) {
        this->add_variable(V);
    }
}  

void DMSolver::add_constraints(std::vector<Constraint*> consts) {
    int cont_size = (int)consts.size();
    for (int i = 0; i < cont_size; i++) {
        add_constraint(consts[i]);
    }

}

void DMSolver::load_constraint_jacobian() {
    if (jac_mat == nullptr) {
        jac_mat = new SpMat(all_constraints.size(), var_count);
        // should be 0
        std::cout << "init, nonzero count : " << jac_mat->nonZeros() << std::endl; 
        int row_i = 0;
        for (int i = 0; i < num_constraints(); i++) {
            std::cout << i << " add constraint jacobian " << std::endl;
            all_constraints[i]->write_jacobian(jac_mat, row_i, sigma_0);  
            row_i += all_constraints[i]->num_eqs();
        }
        std::cout << "after, nonzero count : " << jac_mat->nonZeros() << std::endl; 
        std::cout << "jacobian shape : " << jac_mat->rows() << " " << jac_mat->cols() << std::endl;
    }
}
// int DMSolver::var_c

// desired_sigma -> user-specified

Eigen::VectorXd DMSolver::solve_ff(Eigen::VectorXd desired_sigma) {
    std::cout << "solve ff " << std::endl;
    Eigen::VectorXd ideal_sigma = Eigen::VectorXd::Zero(var_count);
    // compute the force here, not outside..
    Eigen::VectorXd delta = desired_sigma - sigma_0;
    std::cout << delta << std::endl;
    Eigen::VectorXd rhs = Eigen::VectorXd::Zero(all_constraints.size());
    Eigen::MatrixXd dense_jac((*jac_mat));
    std::cout << "jacobian : " << std::endl;
    std::cout << dense_jac << std::endl;
    // rhs = (*jac_mat)*sigma_0;
    std::cout << "sigma_0 : " << std::endl;
    std::cout << sigma_0 << std::endl;
    // TODO : debug if this is jac*sigma_0 or jac*ideal_force
    // rhs = dense_jac*sigma_0;
    rhs = dense_jac*delta;
    std::cout << "rhs : " << std::endl;
    std::cout << rhs << std::endl;
    // negate or not
    // rhs *= -1.0;
    // lagrange_multiplier
    Eigen::VectorXd lag_multi = Eigen::VectorXd::Zero(all_constraints.size());
    // solve for X: (A * A^t) * X = B
    SpMat Jt = (jac_mat->transpose());
    SpMat JJt = (*jac_mat)*(Jt);
    Eigen::MatrixXd dJt = dense_jac.transpose();
    Eigen::MatrixXd dJJt = dense_jac*Jt;
    std::cout << "dJt : " << std::endl;
    std::cout << dJt << std::endl;
    std::cout << "dJJt : " << std::endl;
    std::cout << dJJt << std::endl;

    Eigen::ConjugateGradient<SpMat, Eigen::Upper> cgsolver;
    lag_multi = cgsolver.compute(JJt).solve(rhs);
    ideal_sigma = delta - Jt*lag_multi;
    std::cout << "ideal sigma : " << std::endl;
    std::cout << ideal_sigma << std::endl;
    // closest output 
    Eigen::VectorXd out = sigma_0 + ideal_sigma;

    std::cout << "out : " << std::endl;
    std::cout << out << std::endl;
    return out;
}

Eigen::VectorXd DMSolver::snap_constraints(Eigen::VectorXd cur_vals) {
    std::cout << "snap to constraints using gradient descent" << std::endl;
    Eigen::VectorXd grad = Eigen::VectorXd::Zero(var_count);
    float step_size = 0.49; // This is a crude tool here.  Can do better...
    int num_steps = 5;
    Eigen::VectorXd vals = cur_vals;
    for (int step = 0; step < num_steps; step++) {
        std::cout << "Step " << step << " : " << std::endl;
        grad.setZero();
        for (int i = 0; i < num_constraints(); i++) {
            // std::cout << i << " add constraint jacobian " << std::endl;
            all_constraints[i]->accumulate_enforcement_grad(step_size, grad, cur_vals);  
            // row_i += all_constraints[i]->num_eqs();
        }
        std::cout << "grad : " << std::endl;
        std::cout << grad << std::endl;
        for (int k = 0; k < var_count; k++) {
            vals[k] += grad[k];
        }
    }
    return vals;
}