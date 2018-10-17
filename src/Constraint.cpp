#include "Constraint.h"


Constraint::Constraint() {
    _num_eqs = 0;
    _num_vars = 0;
}
Constraint::~Constraint() {}


////////////////////////////////////////////
// EqualNumConsts
////////////////////////////////////////////

EqualNumConsts::EqualNumConsts() { 
    _num_eqs = 1;
}
EqualNumConsts::EqualNumConsts(Var a, Var b) {
    _a = a;
    _b = b;
    _num_eqs = 1;
}
EqualNumConsts::EqualNumConsts(Var a, Var b, double tar_val) {
    _a = a;
    _b = b;
    _num_eqs = 2;
    _tar_val = tar_val;
}

EqualNumConsts::~EqualNumConsts() {}
void EqualNumConsts::save_indices() {
    // for (int v = 0; v < _variables.size(); v++) {
    //     _indices.push_back(_variables[v]->_solver_id);
    // }
}

void EqualNumConsts::write_jacobian(SpMat *jac_mat, int _row, Eigen::VectorXd pos) {
    // sm1.coeffRef(i,j) = v_ij
    std::cout << _row << " " << _a._solver_id << " " << _b._solver_id << std::endl;
    jac_mat->coeffRef(_row, _a._solver_id) = 1;
    jac_mat->coeffRef(_row, _b._solver_id) = -1;
}

double EqualNumConsts::violate_distance(Eigen::VectorXd pos) {
    double d = 0.0;
    d = fabs(pos[_a._solver_id]-pos[_b._solver_id]);
    return d;
}

void EqualNumConsts::accumulate_enforcement_grad(float step_size, Eigen::VectorXd &grad, Eigen::VectorXd pos) {
    double d = pos[_a._solver_id]-pos[_b._solver_id];
    // TODO: check here.
    std::cout << "d : " << d << std::endl;
    std::cout << _a._solver_id << " " << _b._solver_id << std::endl;
    // std::cout << "step size : " 
    grad[_a._solver_id] = grad[_a._solver_id] - step_size * d;
    grad[_b._solver_id] = grad[_b._solver_id] + step_size * d;
    std::cout << "grad inside : " << std::endl;
    std::cout << grad << std::endl;
}

std::vector<Var*> EqualNumConsts::variables() {

}

// int EqualNumConsts::num_eqs() {
//     return 1;
// }

// int EqualNumConsts::num_vars() {
//     return 2;
// }

// ////////////////////////////////////////////
// // EqualPtsConsts
// ////////////////////////////////////////////
// int EqualPtsConsts::num_eqs() {
//     return 2;
// }
// int EqualPtsConsts::num_vars() {
//     return 4;
// }

EqualPtsConsts::EqualPtsConsts() {}

// [cube] the ws should be a 1x3 vector -> represent each vertex using the x, y, z variables.
EqualPtsConsts::EqualPtsConsts(std::vector<Var> as, std::vector<Var> bs, std::vector<float> ws_a, std::vector<float> ws_b) {
    _as = as;
    _bs = bs;

    // in 3D 
    _num_eqs = 3;
}
EqualPtsConsts::~EqualPtsConsts() {}

void EqualPtsConsts::write_jacobian(SpMat *jac_mat, int _row, Eigen::VectorXd pos) {
    int x_row = _row;
    int y_row = _row+1;
    int z_row = _row+2;
    
    // jac_mat->coeffRef(x_row, _a._solver_id) = 1;
}

void EqualPtsConsts::accumulate_enforcement_grad(float step_size, Eigen::VectorXd &grad, Eigen::VectorXd pos) {
}
void EqualPtsConsts::save_indices() {}
std::vector<Var*> EqualPtsConsts::variables() {

}


// ////////////////////////////////////////////
// // ParallelLineConsts
// ////////////////////////////////////////////
ParallelLineConsts::ParallelLineConsts() {}

ParallelLineConsts::~ParallelLineConsts() {}

void ParallelLineConsts::write_jacobian(SpMat *jac_mat, int _row, Eigen::VectorXd pos) {

}



void ParallelLineConsts::accumulate_enforcement_grad(float step_size, Eigen::VectorXd &grad, Eigen::VectorXd pos) {
 
}

void ParallelLineConsts::save_indices() {}

std::vector<Var*> ParallelLineConsts::variables() {

}