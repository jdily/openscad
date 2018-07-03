#pragma once

#include <vector>
#include <QList>
#include <QLine>
#include "iTree.h"

typedef stree<hnode*> tree_hnode;

class FuncEstimator {
public:
    FuncEstimator();
    FuncEstimator(tree_hnode* geom_tree);
    FuncEstimator(tree_hnode* geom_tree, QList<int> ids);
    ~FuncEstimator();
    QPair<Eigen::Vector3d, Eigen::Vector3d> find_cover_axis();
private:
    tree_hnode* m_tree;
    QList<int> selected_ids;
};