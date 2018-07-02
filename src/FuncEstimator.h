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
    ~FuncEstimator();

    QLine find_axis();
private:
    tree_hnode* m_tree;
};