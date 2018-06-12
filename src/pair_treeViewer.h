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
#include "correspondence.h"
#include "treeViewer.h"
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMouseEvent>
#include <QTableWidget>
#include <QPushButton>
#include <QButtonGroup>
#include <QGroupBox>
#include <QComboBox>
#include <QHeaderView>
#include "relation.h"

class qtreeNode;
typedef stree<hnode*> tree_hnode;

// TODO : desired function
// 1. establish correspondences between tree nodes.

class pair_treeViewer : public QWidget {
    Q_OBJECT
    public:
        pair_treeViewer(QWidget *parent=0);
        ~pair_treeViewer();

        void setSTree(tree_hnode *hree, int index);
    // private:
        shared_ptr<treeViewer> viewer0;
        shared_ptr<treeViewer> viewer1;
        QHBoxLayout *layout;
        QBoxLayout *but_layout;
        QGridLayout *grid_layout;
        QTableWidget *corr_list;
        QTableWidget *relation_list;
        QPushButton *cor_button;
        QPushButton *trans_button;
        QPushButton *relation_button;
        QGroupBox *but_groupbox;
        QGroupBox *info_groupbox;
        QGroupBox *corr_groupbox;
        QBoxLayout *corr_layout;
        QComboBox *rela_drop;
        QMenu *menu;
        // action for establish correspondece.
        QAction *act_est_cor;
        std::vector<correspondence> corrs;
        std::vector<relation> relations;
    protected:
        void mousePressEvent(QMouseEvent *event) override;
    public slots:
        void est_correspondences();
        void est_relations();
};