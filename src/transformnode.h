#pragma once

#include "node.h"
#include "linalg.h"

// different types of transform 
// GEOM for geometry one
// CONFIG for transformation as different ocnfiguration.
enum class usage_type_e {
	GEOM,
	CONFIG
};

class TransformNode : public AbstractNode
{
public:
	VISITABLE();
	TransformNode(const ModuleInstantiation *mi);
	std::string toString() const override;
	std::string name() const override;

	Transform3d matrix;
	usage_type_e use_type;
};
