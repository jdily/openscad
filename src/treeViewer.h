#pragma once
// #include <boost/graph/adjacency_list.hpp>
// #include <boost/graph/graphviz.hpp>
#include <QGraphicsView>
#include <QGraphicsItem>
#include "Tree.h"
// #include </usr/include/graphviz/gvc.h>
#include "CSGTreeEvaluator.h"
#include "GeometryEvaluator.h"
#include "CSGVisitor.h"
#include <random>

class qtreeNode;

class treeViewer : public QGraphicsView, public NodeVisitor {
    Q_OBJECT
    public:
        treeViewer(QWidget *parent=0);
        treeViewer(Tree *tree, QWidget *parent=0);
        ~treeViewer();

        void setTree(Tree *tree);
        QPointF rand_pos();

        Response visit(State &state, const AbstractNode &node) override;
        Response visit(State &state, const RootNode &node) override;
        Response visit(State &state, const AbstractPolyNode &node) override;
        Response visit(State &state, const TransformNode &node) override;
        Response visit(State &state, const CsgOpNode &node) override;
        Response visit(State &state, const GroupNode &node) override;
    private:
        void buildVizTree(Tree* tree);
        void simpleVizTree(Tree* tree);
        Tree *m_pTree;
        shared_ptr<class CSGNode> csgRoot;		   // Result of the CSGTreeEvaluator
        QGraphicsScene *m_pScene;
        // potential issue : the idx of abstractnode is unique per tree only
        QMap<int, qtreeNode*> node_map; 


        void draw_and_traverse(const AbstractNode &node, qtreeNode *parent_node);
    protected:
    // public slots:


};  

// class Edge : public QGraphicsItem {
//     public:
//         Edge(treeViewer* viewer);
//         ~Edge();

// };

// class Node : public QGraphicsItem {
//     public:
//         Node(treeViewer *viewer);
//         ~Node();
//         QRectF boundingRect() const override;
//         QPainterPath shape() const override;
//         void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
//         QList<Edge *> edges() const;
//         void addEdge(Edge *edge);
//     private:
//         QList<Edge *> edgeList;
//         QPointF newPos;
//         treeViewer *m_pViewer;
//     protected:
//         QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
//         void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
//         void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
// };

