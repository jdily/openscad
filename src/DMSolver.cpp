#include "DMSolver.h"
#include "primitives.h"
#include "ModuleInstantiation.h"

DMSolver::DMSolver() {
    is_compiled = false;
    n_vars = 0;
    n_constraints = 0;
}
DMSolver::DMSolver(tree_hnode* tree) : shape_tree(tree) {
    is_compiled = false;
    n_vars = 0;
    n_constraints = 0;
}
DMSolver::~DMSolver() {}

void DMSolver::set_tree(tree_hnode* tree) {
    shape_tree = tree;
    n_vars = 0;
    all_vars.clear();
    // n_constraints = 0;
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
    n_vars = (int)all_vars.size();
    std::vector<double> init_vals;
    for (int i = 0; i < n_vars; i++) {
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
    n_vars = 0;
    n_constraints = 0;
    is_compiled = false;
}

void DMSolver::compile() {
    if (is_compiled) {
        std::cout << "solver already compiled" << std::endl;
    } else {
        n_vars = (int)all_vars.size();
        n_constraints = (int)all_constraints.size();

        std::cout << "n_vars : " << n_vars << std::endl;
        std::cout << "n_constraints : " << n_constraints << std::endl;

        int n_constraints = all_constraints.size();
        for (int c = 0; c < n_constraints; c++) {
            all_constraints[c]->save_indices();
        }
        is_compiled = true; 
    }
}

int DMSolver::num_vars() { return n_vars; }
int DMSolver::num_constraints() { return (int)all_constraints.size(); }

// tmp fixed function for constraints.
void DMSolver::analyze_constraints() {
    int shape0 = 2; // 4;
    int shape1 = 4; //10;
    EqualNumConsts *cont1 = new EqualNumConsts(shape_node_dict[shape0]->var_dict["x"], shape_node_dict[shape1]->var_dict["x"]);
    // this->add_constraint(cont1);

    Var *diff = new Var(-1, 1.5, true);
    NumDiffConsts *cont2 = new NumDiffConsts(shape_node_dict[shape0]->var_dict["z"], shape_node_dict[shape1]->var_dict["z"], diff);
    this->add_constraint(cont2);

    Var *diff_x = new Var(-1, 0.0, true);
    Var *diff_y = new Var(-1, 0.0, true);


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

// what if the variable solver id is set twice???
// [TODO] -> add check if _solver_id == -1??
void DMSolver::add_variable(Var *_v) {
    int cur_idx = (int)this->all_vars.size();
    // std::cout << "cur_idx : " << cur_idx << std::endl;
    if (_v->_solver_id == -1) {
        _v->_solver_id = cur_idx;
    }
    // std::cout << "v solver id : " << _v->_solver_id << std::endl;
    // std::cout << "v shape id : " << _v->_shape_id << std::endl;
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

SpMat* DMSolver::load_constraint_jacobian(Eigen::VectorXd pos) {
    // if (jac_mat == nullptr) {
    SpMat *out = new SpMat(n_constraints, n_vars);
    std::cout << "init, nonzero count : " << out->nonZeros() << std::endl; 

    //     jac_mat = new SpMat(all_constraints.size(), var_count);
    //     // should be 0
    //     std::cout << "init, nonzero count : " << jac_mat->nonZeros() << std::endl; 
    int row_i = 0;
    for (int i = 0; i < n_constraints; i++) {
        std::cout << i << " add constraint jacobian " << std::endl;
        all_constraints[i]->write_jacobian(out, row_i, pos);
        row_i += all_constraints[i]->num_eqs();
    }
    std::cout << "after, nonzero count : " << out->nonZeros() << std::endl; 
    std::cout << "jacobian shape : " << out->rows() << " " << out->cols() << std::endl;
    return out;
}

// extract the values from all poly node, after store_position
Eigen::VectorXd DMSolver::extract_values() {
    Eigen::VectorXd out;
    std::vector<double> out_vals;
    for (auto it : shape_node_dict) {
    // for (auto it = shape_node_dict.begin(); it != shape_node_dict.end(); ++it) {
        hnode* cur_node = it.second;
        for (auto vit : cur_node->var_dict) {
        // for (auto vit = (*it).second.var_dict.begin(); vit != (*it).second.var_dict.end(); vit++) {
            out_vals.push_back(vit.second->_cur_val);
        }
    }
    out = Eigen::Map<Eigen::VectorXd>(out_vals.data(), out_vals.size());  
    return out;
}

bool DMSolver::is_compile() {
    return this->is_compiled;
}

// store the value back to all_vars
void DMSolver::store_position(Eigen::VectorXd pos) {
    for (int i = 0; i < n_vars; i++) {
        all_vars[i]->_cur_val = pos[i];
    }
}

Eigen::VectorXd DMSolver::load_position() {
    Eigen::VectorXd pos(n_vars);
    for (int i = 0; i < all_vars.size(); i++) {
        pos[i] = all_vars[i]->_init_val;
    }
    return pos;
}
// int DMSolver::var_c

// desired_sigma -> user-specified

Eigen::VectorXd DMSolver::solve_ff(Eigen::VectorXd desired_sigma) {
    std::cout << "solve ff " << std::endl;
    if (!is_compiled) {
        this->compile();
    }
    // // [tmp] should be 2
    Eigen::VectorXd out;
    Eigen::VectorXd all_vals;
    Eigen::VectorXd init_pos = this->load_position();
    // load jacobian..
    // [tmp] should be 1x2
    SpMat *jac_mat = this->load_constraint_jacobian(init_pos);

    Eigen::VectorXd ideal_sigma = Eigen::VectorXd::Zero(n_vars);

    // first manuaully set the vars from the outside 
    // deal with the editor things later.
    Eigen::VectorXd _d_sigma = Eigen::VectorXd::Zero(n_vars);

    // TODO : the desired sigma is wrong..
    std::cout << "desired_sigma : " << std::endl;
    std::cout << desired_sigma << std::endl;
    // _d_sigma[0] = desired_sigma[2];
    // _d_sigma[1] = desired_sigma[5];
    // _d_sigma[2] = 1.5;
    // std::cout << "d_sigma : " << std::endl;
    // std::cout << _d_sigma << std::endl;

    // compute the force here, not outside..
    Eigen::VectorXd delta = desired_sigma - init_pos;
    std::cout << "delta :" << std::endl;
    std::cout << delta << std::endl;    
    // // Eigen::VectorXd delta = desired_sigma - sigma_0;
    // std::cout << delta << std::endl;
    Eigen::VectorXd rhs = Eigen::VectorXd::Zero(n_constraints);
    Eigen::MatrixXd dense_jac((*jac_mat));
    // std::cout << "jacobian : " << std::endl;
    // std::cout << dense_jac << std::endl;
    // // rhs = (*jac_mat)*sigma_0;
    // std::cout << "sigma_0 : " << std::endl;
    // std::cout << sigma_0 << std::endl;
    // // TODO : debug if this is jac*sigma_0 or jac*ideal_force
    // // rhs = dense_jac*sigma_0;
    rhs = dense_jac*delta;
    // std::cout << "rhs : " << std::endl;
    // std::cout << rhs << std::endl;
    // // negate or not
    // // rhs *= -1.0;
    // // lagrange_multiplier
    Eigen::VectorXd lag_multi = Eigen::VectorXd::Zero(n_constraints);
    // solve for X: (A * A^t) * X = B
    SpMat Jt = (jac_mat->transpose());
    SpMat JJt = (*jac_mat)*(Jt);
    Eigen::MatrixXd dJt = dense_jac.transpose();
    Eigen::MatrixXd dJJt = dense_jac*Jt;
    // std::cout << "dJt : " << std::endl;
    // std::cout << dJt << std::endl;
    // std::cout << "dJJt : " << std::endl;
    // std::cout << dJJt << std::endl;

    Eigen::ConjugateGradient<SpMat, Eigen::Upper> cgsolver;
    lag_multi = cgsolver.compute(JJt).solve(rhs);
    ideal_sigma = delta - Jt*lag_multi;
    std::cout << "ideal sigma : " << std::endl;
    std::cout << ideal_sigma << std::endl;
    // closest output 
    // out = sigma_0 + ideal_sigma;
    out = init_pos + ideal_sigma;
    std::cout << "out : " << std::endl;
    std::cout << out << std::endl;
    store_position(out);
    // assign back 
    all_vals = extract_values();
    std::cout << all_vals << std::endl;
    return all_vals;
}

Eigen::VectorXd DMSolver::snap_constraints(Eigen::VectorXd cur_vals) {
    std::cout << "snap to constraints using gradient descent" << std::endl;
    Eigen::VectorXd grad = Eigen::VectorXd::Zero(n_vars);
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
        for (int k = 0; k < n_vars; k++) {
            vals[k] += grad[k];
        }
    }
    return vals;
}