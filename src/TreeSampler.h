#pragma once 

#include <QMap>
#include "node.h"
#include "NodeVisitor.h"
#include <algorithm>
#include "Tree.h"
#include "GeometryEvaluator.h"
#include "Camera.h"
#include "QGLView.h"
#include "transformnode.h"

class TreeSampler : public NodeVisitor {
public:
    TreeSampler();
    TreeSampler(Tree* tree);
    TreeSampler(Tree* tree, class GeometryEvaluator *geomevaluator = nullptr);
    ~TreeSampler();
    Response visit(State &state, const RootNode &node) override;
    Response visit(State &state, const AbstractNode &node) override;
    // Response visit(State &state, const AbstractPolyNode &node) override;
    Response visit(State &state, const LeafNode &node) override;
    Response visit(State &state, const GroupNode &node) override;
    Response visit(State &state, const TransformNode &node) override;

    QMap<int, std::vector<Eigen::Vector3d>> get_samples(const AbstractNode &node, QGLView* viewer, bool proj);
    
private:
    std::map<int, Eigen::Vector3d> centroids;
    QMap<int, std::vector<Eigen::Vector3d> > sample_dict;
    QMap<int, std::vector<Eigen::Vector3d> > proj_sample_dict;
    class GeometryEvaluator *m_geomeval;
    const Tree *m_tree;

    QMap<int, std::vector<int> > visitedchildren;
    // std::map<int, std::vector<const Geometry*> > visitedchildren;
    void addToParent(const State &state, const AbstractNode &node);
	void applyToChildren(State &state, const AbstractNode &node);
};