#pragma once


// #include <boost/graph/graphviz.hpp>
// #include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
// #include "Tree.h"
// #include </usr/include/graphviz/gvc.h>
// #include "CSGTreeEvaluator.h"
// #include "GeometryEvaluator.h"
// #include "CSGVisitor.h"
#include <random>
#include <QMouseEvent>
// #include <boost/graph/adjacency_list.hpp>
// TODO : test boost graph things

// #include "graphConverter.h"
// #include "iTree.h"

// class treeScene {
class treeScene : public QGraphicsScene {
    // Q_OBJECT
    public:
        // treeScene();
        treeScene(QObject *parent=0);
        ~treeScene();
    private:
        QGraphicsRectItem* m_selectRect;
    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
        void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
};