#include "qtreeEdge.h"
#include "qtreeNode.h"
#include "treeViewer.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>


qtreeEdge::qtreeEdge(qtreeNode *sourceNode, qtreeNode *destNode) {
    setAcceptedMouseButtons(0);
    source = sourceNode;
    dest = destNode;
    sourcePoint = source->pos();
    destPoint = dest->pos();
    source->addEdge(this);
    dest->addEdge(this);
    // adjust();
}
qtreeEdge::~qtreeEdge() {
}

qtreeNode* qtreeEdge::sourceNode() const {
    return source;
}        
qtreeNode* qtreeEdge::destNode() const {
    return dest;
}

QRectF qtreeEdge::boundingRect() const {
    if (!source || !dest)
        return QRectF();
    qreal penWidth = 1;
    qreal extra = (penWidth + arrowSize) / 2.0;

    return QRectF(source->pos(), QSizeF(dest->pos().x()-source->pos().x(),
                                       dest->pos().y()-source->pos().y()))
        .normalized().adjusted(-extra, -extra, extra, extra);
    // return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
    //                                   destPoint.y() - sourcePoint.y()))
    //     .normalized()
    //     .adjusted(-extra, -extra, extra, extra);
}

// TODO : make the edge stick with two end nodes .
void qtreeEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    if (!source || !dest)
        return;
    line.setP1(source->pos());
    line.setP2(dest->pos());
    // QLineF line(sourcePoint, destPoint);
    // QLineF line(source->pos(), dest->pos());
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;
    // Draw the line itself
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);

    // // Draw the arrows
    double angle = std::atan2(-line.dy(), line.dx());

    QPointF sourceArrowP1 = sourcePoint + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                                  cos(angle + M_PI / 3) * arrowSize);
    QPointF sourceArrowP2 = sourcePoint + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                                  cos(angle + M_PI - M_PI / 3) * arrowSize);
    QPointF destArrowP1 = destPoint + QPointF(sin(angle - M_PI / 3) * arrowSize,
                                              cos(angle - M_PI / 3) * arrowSize);
    QPointF destArrowP2 = destPoint + QPointF(sin(angle - M_PI + M_PI / 3) * arrowSize,
                                              cos(angle - M_PI + M_PI / 3) * arrowSize);

    // painter->setBrush(Qt::black);
    // painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
    // painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
}