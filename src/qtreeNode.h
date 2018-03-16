#pragma once

#include <QGraphicsItem>
#include <QList>

class qtreeEdge;
class treeViewer;

class qtreeNode : public QGraphicsItem {
    public:
        qtreeNode(treeViewer *viewer);
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
    protected:


};