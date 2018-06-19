#pragma once

class Polygon2d;
class PolySet;
#include <vector>
#include "linalg.h"
namespace PolysetUtils {

	Polygon2d *project(const PolySet &ps);
	void tessellate_faces(const PolySet &inps, PolySet &outps);
	bool is_approximately_convex(const PolySet &ps);

	std::vector<Eigen::Vector3d> random_sample(const PolySet &ps);
};
