#pragma once

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/Dense>


// TODO:
// 1. collect the parameters from the given shape tree as the init state
// 2. check how to represent the constraints
// 3. check how to "pick the constraints"
// 4. make the jacobian from the constraints


class DMSolver {
public:
    DMSolver();
    ~DMSolver();
private:

};