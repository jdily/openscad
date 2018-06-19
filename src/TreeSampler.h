#pragma once 


#include "node.h"
#include "NodeVisitor.h"
#include <algorithm>
#include "Tree.h"
#include "GeometryEvaluator.h"

class TreeSampler : public NodeVisitor {
public:
    TreeSampler();
    TreeSampler(Tree* tree);
    TreeSampler(Tree* tree, class GeometryEvaluator *geomevaluator = nullptr);
    ~TreeSampler();
    Response visit(State &state, const AbstractNode &node) override;
    Response visit(State &state, const AbstractPolyNode &node) override;
    std::map<int, Eigen::Vector3d> get_centroids(const AbstractNode &node);

private:
    std::map<int, Eigen::Vector3d> centroids;
    class GeometryEvaluator *m_geomeval;
    const Tree *m_tree;
};