#pragma once
#include <QGraphicsLineItem>
class simpleArrow : public QGraphicsLineItem {
public:
    simpleArrow(QPointF *start, QPointF *end, QGraphicsItem *parent = 0);
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
private:
protected:
    QPolygonF arrowHead;
    QPointF *m_pStart;
    QPointF *m_pEnd;
};