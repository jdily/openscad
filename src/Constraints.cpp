#include "Constraints.h"


Constraints::Constraints() {
    _num_eqs = 0;
    _num_vars = 0;
}
Constraints::~Constraints() {}


////////////////////////////////////////////
// EqualNumConsts
////////////////////////////////////////////

EqualNumConsts::EqualNumConsts() { }
EqualNumConsts::EqualNumConsts(Var a, Var b) {
    _a = a;
    _b = b;
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
EqualPtsConsts::EqualPtsConsts(std::vector<Var> as, std::vector<Var> bs) {
    _as = as;
    _bs = bs;
}
EqualPtsConsts::~EqualPtsConsts() {}

void EqualPtsConsts::write_jacobian(SpMat *jac_mat, int _row) {

}