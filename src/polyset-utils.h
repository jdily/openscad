#pragma once

class Polygon2d;
class PolySet;
#include <vector>
#include "linalg.h"
#include "memory.h"
#include "Geometry.h"
#include "linalg.h"
#include "GeometryUtils.h"
namespace PolysetUtils {

	Polygon2d *project(const PolySet &ps);
	void tessellate_faces(const PolySet &inps, PolySet &outps);
	bool is_approximately_convex(const PolySet &ps);

	// std::vector<Eigen::Vector3d> random_sample(PolySet &ps);
	std::vector<Eigen::Vector3d> random_sample(Geometry* ps);
	Eigen::Vector3d sample_tri(Polygon p);
	Eigen::Vector3d sample_quad(Polygon p);
};
