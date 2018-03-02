#include "simpleArrow.h"
#include <QPen>
simpleArrow::simpleArrow(QPointF *start, QPointF *end, QGraphicsItem *parent): QGraphicsLineItem(parent) {
    m_pStart = start;
    m_pEnd = end;
    setLine(QLineF(*start, *end));
    // create arrow head
    qreal arrowSize = 100;
    double angle = std::atan2(-line().dy(), line().dx());
    // QPointF arrowP1 = line().p1() + QPointF(sin(angle + M_PI / 3) * arrowSize,
    //                                 cos(angle + M_PI / 3) * arrowSize);
    // QPointF arrowP2 = line().p1() + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
    //                                 cos(angle + M_PI - M_PI / 3) * arrowSize);
    QPointF arrowP1 = line().p2() - QPointF(sin(angle + M_PI / 3) * arrowSize,
                                    cos(angle + M_PI / 3) * arrowSize);
    QPointF arrowP2 = line().p2() - QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                    cos(angle + M_PI - M_PI / 3) * arrowSize);
    // QPointF arrowP3 = line().p2() + QPointF(sin(angle + M_PI) * arrowSize,
    //                                 cos(angle + M_PI) * arrowSize);
    arrowHead.clear();
    // arrowHead << line().p1() << arrowP1 << arrowP2;
    arrowHead << arrowP2 << line().p2() << arrowP1; 
}

QRectF simpleArrow::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

QPainterPath simpleArrow::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(arrowHead);
    return path;
}