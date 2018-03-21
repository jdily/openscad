#pragma once

#include <QGraphicsItem>
#include <QList>
#include <string>

class qtreeEdge;
class treeViewer;
using namespace std;
class qtreeNode : public QGraphicsItem {
    public:
        qtreeNode(treeViewer *viewer);
        qtreeNode(treeViewer *viewer, string node_type);
        ~qtreeNode();
        void addEdge(qtreeEdge *edge);
        QList<qtreeEdge *> edges() const;
        QRectF boundingRect() const override;
        QPainterPath shape() const override;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    private:
        QList<qtreeEdge*> edgeList;
        QPointF newPos;
        treeViewer *m_pViewer;
        string type;
        QList<qtreeNode*> childList;
        qtreeNode *parent_node;
        QMap<string, QColor> color_map;

    protected:  
};