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
EqualNumConsts::EqualNumConsts(Var* a, Var* b) {
    _a = a;
    _b = b;
    _num_eqs = 1;
}
// EqualNumConsts::EqualNumConsts(Var* a, Var* b, double tar_val) {
//     _a = a;
//     _b = b;
//     _num_eqs = 2;
//     // _tar_val = tar_val;
// }

EqualNumConsts::~EqualNumConsts() {}
void EqualNumConsts::save_indices() {
    this->_indices.push_back(_a->_solver_id);
    this->_indices.push_back(_b->_solver_id);
}

void EqualNumConsts::write_jacobian(SpMat *jac_mat, int _row, Eigen::VectorXd pos) {
    // sm1.coeffRef(i,j) = v_ij
    std::cout << _row << " " << _a->_solver_id << " " << _b->_solver_id << std::endl;
    jac_mat->coeffRef(_row, _a->_solver_id) = 1;
    jac_mat->coeffRef(_row, _b->_solver_id) = -1;
}

double EqualNumConsts::violate_distance(Eigen::VectorXd pos) {
    double d = 0.0;
    d = fabs(pos[_a->_solver_id]-pos[_b->_solver_id]);
    return d;
}

void EqualNumConsts::accumulate_enforcement_grad(float step_size, Eigen::VectorXd &grad, Eigen::VectorXd pos) {
    double d = pos[_a->_solver_id]-pos[_b->_solver_id];
    // TODO: check here.
    std::cout << "d : " << d << std::endl;
    std::cout << _a->_solver_id << " " << _b->_solver_id << std::endl;
    // std::cout << "step size : " 
    grad[_a->_solver_id] = grad[_a->_solver_id] - step_size * d;
    grad[_b->_solver_id] = grad[_b->_solver_id] + step_size * d;
    std::cout << "grad inside : " << std::endl;
    std::cout << grad << std::endl;
}

std::vector<Var*> EqualNumConsts::variables() {
    std::vector<Var*> out;
    out.push_back(_a);
    out.push_back(_b);
    return out;
}

// ////////////////////////////////////////////
// // NumDiffConsts
// ////////////////////////////////////////////
NumDiffConsts::NumDiffConsts() {}

NumDiffConsts::NumDiffConsts(Var* a, Var* b, Var* val) {
    _a = a;
    _b = b;
    _val = val;
    _num_eqs = 1;
}

NumDiffConsts::~NumDiffConsts() {}

void NumDiffConsts::write_jacobian(SpMat *jac_mat, int _row, Eigen::VectorXd pos) {
    std::cout << _row << " " << _a->_solver_id << " " << _b->_solver_id << std::endl;
    jac_mat->coeffRef(_row, this->_indices[0]) = 1;
    jac_mat->coeffRef(_row, this->_indices[1]) = -1;
    jac_mat->coeffRef(_row, this->_indices[2]) = -1;
}



void NumDiffConsts::accumulate_enforcement_grad(float step_size, Eigen::VectorXd &grad, Eigen::VectorXd pos) {
    double d = pos[_a->_solver_id] - pos[_b->_solver_id] - pos[_val->_solver_id];
    grad[_a->_solver_id] = grad[_a->_solver_id] - step_size * d;
    grad[_b->_solver_id] = grad[_b->_solver_id] + step_size * d;
    grad[_val->_solver_id] = grad[_val->_solver_id] + step_size * d;
}

double NumDiffConsts:: violate_distance(Eigen::VectorXd pos) {
    double d = pos[_a->_solver_id] - pos[_b->_solver_id] - pos[_val->_solver_id];
    return fabs(d);
}

void NumDiffConsts::save_indices() {
    this->_indices.push_back(_a->_solver_id);
    this->_indices.push_back(_b->_solver_id);
    this->_indices.push_back(_val->_solver_id);
}

std::vector<Var*> NumDiffConsts::variables() {
    std::vector<Var*> out;
    out.push_back(_a);
    out.push_back(_b);
    out.push_back(_val);
    return out;
}



EqualPtsConsts::EqualPtsConsts() {}

// [cube] the ws should be a 1x3 vector -> represent each vertex using the x, y, z variables.
EqualPtsConsts::EqualPtsConsts(std::vector<Var*> as, std::vector<Var*> bs, std::vector<float> ws_a, std::vector<float> ws_b) {
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
void EqualPtsConsts::save_indices() {
    // this->_indices.push_back(_a._solver_id);
    // this->_indices.push_back(_b._solver_id);
    for (auto v : _as) {
        this->_indices.push_back(v->_solver_id);
    }
    for (auto v : _bs) {
        this->_indices.push_back(v->_solver_id);
    }
}
std::vector<Var*> EqualPtsConsts::variables() {
    std::vector<Var*> out;
    out.insert(out.end(), _as.begin(), _as.end());
    out.insert(out.end(), _bs.begin(), _bs.end());
    return out;
}

double EqualPtsConsts:: violate_distance(Eigen::VectorXd pos) {}

// ////////////////////////////////////////////
// // AlignPoint2DConsts
// TODO : think what is the order of the varabiles??
// ////////////////////////////////////////////

AlignPoint2DConsts::AlignPoint2DConsts(std::vector<Var*> vas, std::vector<Var*> vbs,  std::vector<float> wsa, std::vector<float> wsb, std::vector<Var*> measurement) {
    _as = vas;
    _bs = vbs;
    ws_a = wsa;
    ws_b = wsb;
    _meas = measurement;
    _num_eqs = 2;
}

AlignPoint2DConsts::~AlignPoint2DConsts() {}

void AlignPoint2DConsts::write_jacobian(SpMat *jac_mat, int _row, Eigen::VectorXd pos) {

}

void AlignPoint2DConsts::accumulate_enforcement_grad(float step_size, Eigen::VectorXd &grad, Eigen::VectorXd pos) {

}

double AlignPoint2DConsts::violate_distance(Eigen::VectorXd pos) {

}


void AlignPoint2DConsts::save_indices() {
    for (auto v : _as) {
        this->_indices.push_back(v->_solver_id);
    }
    for (auto v : _bs) {
        this->_indices.push_back(v->_solver_id);
    }
    for (auto v : _meas) {
        this->_indices.push_back(v->_solver_id);
    }
}

// what if one variable is constrained by multiple coonstraints
std::vector<Var*> AlignPoint2DConsts::variables() {
    std::vector<Var*> out;
    
    out.insert(out.end(), _as.begin(), _as.end());
    out.insert(out.end(), _bs.begin(), _bs.end());
    return out;
}


// ////////////////////////////////////////////
// // ParallelLineConsts
// ////////////////////////////////////////////
ParallelLineConsts::ParallelLineConsts() {}

ParallelLineConsts::~ParallelLineConsts() {}

void ParallelLineConsts::write_jacobian(SpMat *jac_mat, int _row, Eigen::VectorXd pos) {

}

double ParallelLineConsts:: violate_distance(Eigen::VectorXd pos) {}


void ParallelLineConsts::accumulate_enforcement_grad(float step_size, Eigen::VectorXd &grad, Eigen::VectorXd pos) {
 
}

void ParallelLineConsts::save_indices() {}

std::vector<Var*> ParallelLineConsts::variables() {

}