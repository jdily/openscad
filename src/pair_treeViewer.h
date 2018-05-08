#pragma once 

// #include <boost/graph/graphviz.hpp>
#include <QGraphicsView>
#include <QGraphicsItem>
#include "Tree.h"
// #include </usr/include/graphviz/gvc.h>
#include "CSGTreeEvaluator.h"
#include "GeometryEvaluator.h"
#include "CSGVisitor.h"
#include <random>
#include "graphConverter.h"
#include "iTree.h"

#include "treeViewer.h"
#include <QHBoxLayout>

class qtreeNode;
typedef stree<hnode*> tree_hnode;


class pair_treeViewer : public QWidget {
    Q_OBJECT
    public:
        pair_treeViewer(QWidget *parent=0);
        ~pair_treeViewer();

        void setSTree(tree_hnode *hree, int index);
    private:
        shared_ptr<treeViewer> viewer0;
        shared_ptr<treeViewer> viewer1;

        QHBoxLayout *layout;
};