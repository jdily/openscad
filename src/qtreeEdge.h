#pragma once

#include <QGraphicsItem>
#include <QList>

class qtreeNode;
class treeViewer;

class qtreeEdge : public QGraphicsItem {
    public:
        qtreeEdge(qtreeNode *sourceNode, qtreeNode *destNode); 
        qtreeNode *sourceNode() const;
        qtreeNode *destNode() const;
        // qtreeEdge(treeViewer *viewer);
        ~qtreeEdge();
        QRectF boundingRect() const override;
        // QPainterPath shape() const override;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    private:
        qtreeNode *source, *dest;
        QPointF sourcePoint;
        QPointF destPoint;
        qreal arrowSize;
    protected:
        // QRectF boundingRect() const override;
        // void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

};