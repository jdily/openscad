#pragma once

// TODO :
// check later if we need bool type of Vars
struct Var {
public: 
    Var() {}
    Var(int solver_id, int shape_id, double cur_val) {
        _solver_id = solver_id;
        _shape_id = shape_id;
        _cur_val = cur_val;
    }
    // the variable id in the list of all variables
    // i.e. the column id
    int _solver_id;
    double _cur_val;
    int _shape_id;
};