#pragma once 

#include "node.h"
#include "NodeVisitor.h"
#include <algorithm>
#include "Tree.h"
#include "GeometryEvaluator.h"
#include "Camera.h"
#include "QGLView.h"

class TreeSampler : public NodeVisitor {
public:
    TreeSampler();
    TreeSampler(Tree* tree);
    TreeSampler(Tree* tree, class GeometryEvaluator *geomevaluator = nullptr);
    ~TreeSampler();
    Response visit(State &state, const AbstractNode &node) override;
    Response visit(State &state, const AbstractPolyNode &node) override;
    std::map<int, std::vector<Eigen::Vector3d>> get_samples(const AbstractNode &node, QGLView* cam);
    
private:
    std::map<int, Eigen::Vector3d> centroids;
    std::map<int, std::vector<Eigen::Vector3d> > sample_dict;
    class GeometryEvaluator *m_geomeval;
    const Tree *m_tree;
};