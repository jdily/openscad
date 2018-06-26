#include "polyset-utils.h"
#include "polyset.h"
#include "Polygon2d.h"
#include "printutils.h"
#include "GeometryUtils.h"
#include "Reindexer.h"
#include "grid.h"
#include <algorithm>
#include "memory.h"
#include <time.h> 
#include <random>

#ifdef ENABLE_CGAL
#include "cgalutils.h"
#endif

namespace PolysetUtils {

	// Project all polygons (also back-facing) into a Polygon2d instance.
  // It's important to select all faces, since filtering by normal vector here
	// will trigger floating point incertainties and cause problems later.
	Polygon2d *project(const PolySet &ps) {
		auto poly = new Polygon2d;

		for (const auto &p : ps.polygons) {
			Outline2d outline;
			for (const auto &v : p) {
				outline.vertices.emplace_back(v[0], v[1]);
			}
			poly->addOutline(outline);
		}
		return poly;
	}

/* Tessellation of 3d PolySet faces
	 
	 This code is for tessellating the faces of a 3d PolySet, assuming that
	 the faces are near-planar polygons.
	 
	 The purpose of this code is originally to fix github issue 349. Our CGAL
	 kernel does not accept polygons for Nef_Polyhedron_3 if each of the
	 points is not exactly coplanar. "Near-planar" or "Almost planar" polygons
	 often occur due to rounding issues on, for example, polyhedron() input.
	 By tessellating the 3d polygon into individual smaller tiles that
	 are perfectly coplanar (triangles, for example), we can get CGAL to accept
	 the polyhedron() input.
*/
	
/* Given a 3D PolySet with near planar polygonal faces, tessellate the
	 faces. As of writing, our only tessellation method is triangulation
	 using CGAL's Constrained Delaunay algorithm. This code assumes the input
	 polyset has simple polygon faces with no holes.
	 The tessellation will be robust wrt. degenerate and self-intersecting
*/
	void tessellate_faces(const PolySet &inps, PolySet &outps)
	{
		int degeneratePolygons = 0;

		// Build Indexed PolyMesh
		Reindexer<Vector3f> allVertices;
		std::vector<std::vector<IndexedFace>> polygons;

		for (const auto &pgon : inps.polygons) {
			if (pgon.size() < 3) {
				degeneratePolygons++;
				continue;
			}
			
			polygons.push_back({});
			auto &faces = polygons.back();
			faces.push_back(IndexedFace());
			auto &currface = faces.back();
			for (const auto &v : pgon) {
				// Create vertex indices and remove consecutive duplicate vertices
				auto idx = allVertices.lookup(v.cast<float>());
				if (currface.empty() || idx != currface.back()) currface.push_back(idx);
			}
			if (currface.front() == currface.back()) currface.pop_back();
			if (currface.size() < 3) {
				faces.pop_back(); // Cull empty triangles
				if (faces.empty()) polygons.pop_back(); // All faces were culled
			}
		}

		// Tessellate indexed mesh
		const auto *verts = allVertices.getArray();
		std::vector<IndexedTriangle> allTriangles;
		for (const auto &faces : polygons) {
			std::vector<IndexedTriangle> triangles;
			auto err = false;
			if (faces[0].size() == 3) {
				triangles.emplace_back(faces[0][0], faces[0][1], faces[0][2]);
			}
			else {
				err = GeometryUtils::tessellatePolygonWithHoles(verts, faces, triangles, nullptr);
			}
			if (!err) {
				for (const auto &t : triangles) {
					outps.append_poly();
					outps.append_vertex(verts[t[0]]);
					outps.append_vertex(verts[t[1]]);
					outps.append_vertex(verts[t[2]]);
				}
			}
		}
		if (degeneratePolygons > 0) PRINT("WARNING: PolySet has degenerate polygons");
	}

	bool is_approximately_convex(const PolySet &ps) {
#ifdef ENABLE_CGAL
		return CGALUtils::is_approximately_convex(ps);
#else
		return false;
#endif
	}

	Eigen::Vector3d sample_tri(Polygon p) {
		srand( time( NULL ) );
		double a = 0.0, b = 0.0, c = 0.0;
		a = ((double) rand() / (RAND_MAX));
		b = ((double) rand() / (RAND_MAX));
		c = ((double) rand() / (RAND_MAX));
		double n = 1.0 / (a+b+c);
		// barycentric coordinate.
		a *= n; b *= n; c *= n;
		Eigen::Vector3d out = a*p[0] + b*p[1] + c*p[2];
		return out;
	}

	Eigen::Vector3d sample_quad(Polygon p) {
		// https://stackoverflow.com/questions/240778/random-points-inside-a-parallelogram
		// If your parallelogram is defined by the points ABCD such that AB, BC, CD and DA are the sides, then take your point as being:
		// p = A + (u * AB) + (v * AD)
        // Where AB is the vector from A to B and AD the vector from A to D.
		std::cout << "sample quad" << std::endl;
		double a = 0.0, b = 0.0, c = 0.0, d = 0.0;
		a = ((double) rand() / (RAND_MAX));
		b = ((double) rand() / (RAND_MAX));
		c = ((double) rand() / (RAND_MAX));
		d = ((double) rand() / (RAND_MAX));
		double n = 1.0 / (a+b+c+d);
		std::cout << a << " " << b << " " << c << " " << d << " " << n << std::endl;
		a *= n; b *= n; c *= n; d *= n;
		std::cout << a+b+c+d << std::endl;
		Eigen::Vector3d out = a*p[0] + b*p[1] + c*p[2] + d*p[3];
		return out;
	}

	std::vector<Vector3d> random_sample(Geometry* ps) {
		srand ( time(NULL) );
		float sample_ratio = 0.5;
		float sample_count = 10;
		std::vector<Vector3d> pnts;
		PolySet* newps = static_cast<PolySet*>(ps);
		// shared_ptr<PolySet> newps = dynamic_pointer_cast<PolySet>(ps);
		std::cout << newps->numPolygons() << " polygons exist." << std::endl;
		for (const auto &p : newps->polygons) {	
			int num_pnts = (int)p.size();
			std::cout << "num pnts in polygon : " << num_pnts << std::endl;
			for (int k = 0; k < sample_count; k++) {
				if (num_pnts == 3) {
					Eigen::Vector3d sample = sample_tri(p);
					pnts.push_back(sample);
				} else if (num_pnts == 4) {
					Eigen::Vector3d sample = sample_quad(p);
					pnts.push_back(sample);
				}
			}
			// int num_samples = int(num_pnts*sample_ratio);
			// if (num_samples < 1) {
			// 	// just return the only point you get...
			// 	pnts.push_back(p[0]);
			// } else {
			// 	std::vector<int> rand_inds;
			// 	for (int k = 0; k < num_pnts; k++) {
			// 		rand_inds.push_back(k);
			// 	}
			// 	// test for all the vertices.
			// 	// std::random_shuffle(rand_inds.begin(), rand_inds.end());
			// 	for (int k = 0; k < num_pnts; k++) {
			// 	// for (int k = 0; k < num_samples; k++) {
			// 		pnts.push_back(p[rand_inds[k]]);
			// 	}
			// }
		}
		return pnts;
	}
}
