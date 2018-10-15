#include "Constraints.h"


Constraints::Constraints() {
    _num_eqs = 0;
    _num_vars = 0;
}
Constraints::~Constraints() {}


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

void EqualNumConsts::write_jacobian(SpMat *jac_mat, int _row) {
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
// 
EqualPtsConsts::EqualPtsConsts(std::vector<Var> as, std::vector<Var> bs, std::vector<float> ws_a, std::vector<float> ws_b) {
    _as = as;
    _bs = bs;
}
EqualPtsConsts::~EqualPtsConsts() {}

void EqualPtsConsts::write_jacobian(SpMat *jac_mat, int _row) {

}

void EqualPtsConsts::accumulate_enforcement_grad(float step_size, Eigen::VectorXd &grad, Eigen::VectorXd pos) {
}