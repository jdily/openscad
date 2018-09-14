#pragma once

#include "module.h"
#include "node.h"
#include "polyset.h"
#include "evalcontext.h"
#include "Polygon2d.h"
#include "builtin.h"
#include "printutils.h"
#include "context.h"
#include "calc.h"
#include <sstream>
#include <assert.h>
#include <cmath>
#include <boost/assign/std/vector.hpp>
using namespace boost::assign; // bring 'operator+=()' into scope

#define F_MINIMUM 0.01

enum class primitive_type_e {
	CUBE,
	SPHERE,
	CYLINDER,
	POLYHEDRON,
	SQUARE,
	CIRCLE,
	POLYGON
};

class PrimitiveModule : public AbstractModule
{
public:
	primitive_type_e type;
	PrimitiveModule(primitive_type_e type) : type(type) { }
	AbstractNode *instantiate(const Context *ctx, const ModuleInstantiation *inst, EvalContext *evalctx) const override;
private:
	Value lookup_radius(const Context &ctx, const std::string &radius_var, const std::string &diameter_var) const;
};

class PrimitiveNode : public LeafNode
{
public:
	VISITABLE();
	PrimitiveNode(primitive_type_e type) : LeafNode(), type(type) {}
	PrimitiveNode(const ModuleInstantiation *mi, primitive_type_e type) : LeafNode(mi), type(type) { }
	std::string toString() const override;
	std::string name() const override {
		switch (this->type) {
		case primitive_type_e::CUBE:
			return "cube";
			break;
		case primitive_type_e::SPHERE:
			return "sphere";
			break;
		case primitive_type_e::CYLINDER:
			return "cylinder";
			break;
		case primitive_type_e::POLYHEDRON:
			return "polyhedron";
			break;
		case primitive_type_e::SQUARE:
			return "square";
			break;
		case primitive_type_e::CIRCLE:
			return "circle";
			break;
		case primitive_type_e::POLYGON:
			return "polygon";
			break;
		default:
			assert(false && "PrimitiveNode::name(): Unknown primitive type");
			return "unknown";
		}
	}

	void copyGeom(const PrimitiveNode* pnode);

	// how to get the center of each primitives..
	bool center;
	// cube : x, y, z -> lengthes of different sides.
	double x, y, z, h, r1, r2;
	double fn, fs, fa;
	primitive_type_e type;
	int convexity;
	ValuePtr points, paths, faces;
	const Geometry *createGeometry() const override;
};