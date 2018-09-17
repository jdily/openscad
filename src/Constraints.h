#pragma once 


class Constraints {
public:
    // Constraints();
    // ~Constraints();
    int num_eqs() { return _num_eqs; }
    int num_vars() { return _num_vars; }

protected:
    int _num_eqs;
    int _num_vars;
};

// Equal Number Constraints -> g(\sigma) = a-b
class EqualNumConsts : public Constraints {
public:
    // arguments should be the variable position (idx)
    EqualNumConsts();
    EqualNumConsts(int a_idx, int b_idx);
    ~EqualNumConsts();
    // int num_eqs() { return _num_eqs; }
    // int num_vars() { return _num_vars; }
};

class EqualPtsConsts : public Constraints {
public:
    EqualPtsConsts();
    ~EqualPtsConsts();

    // int num_eqs() { return _num_eqs; }
    // int num_vars() { return _num_vars; }
};