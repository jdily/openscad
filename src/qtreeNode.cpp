#include "qtreeNode.h"
#include "qtreeEdge.h"
#include "treeViewer.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>



qtreeNode::qtreeNode(treeViewer *viewer) {
    m_pViewer = viewer;   
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setAcceptHoverEvents(true);
    setZValue(-1);
    type = "";
}

qtreeNode::qtreeNode(treeViewer *viewer, string node_type) {
    m_pViewer = viewer;   
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
    type = node_type;

    color_map["root"] = QColor(Qt::red);
    color_map["poly"] = QColor(Qt::blue);
    color_map["trans"] = QColor(Qt::green);
    color_map["group"] = QColor(Qt::darkGray);
    color_map["csg_opt"] = QColor(Qt::yellow);
    color_map["cgal_adv"] =QColor(Qt::magenta);
}

qtreeNode::~qtreeNode() {}

void qtreeNode::addEdge(qtreeEdge *edge)
{
    edgeList << edge;
    // edge->adjust();
}

QList<qtreeEdge*> qtreeNode::edges() const
{
    return edgeList;
}

QRectF qtreeNode::boundingRect() const
{
    qreal adjust = 2;
    return QRectF( -10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);
}

QPainterPath qtreeNode::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 20, 20);
    return path;
}

// add different paint style for different node...
void qtreeNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    // painter->setPen(Qt::NoPen);
    // painter->setBrush(Qt::darkGray);
    // painter->drawEllipse(-7, -7, 20, 20);

    // // QRadialGradient gradient(-3, -3, 10);
    // if (option->state & QStyle::State_Sunken) {
    //     gradient.setCenter(3, 3);
    //     gradient.setFocalPoint(3, 3);
    //     gradient.setColorAt(1, QColor(Qt::yellow).light(120));
    //     gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));
    // } else {
    //     gradient.setColorAt(0, Qt::yellow);
    //     gradient.setColorAt(1, Qt::darkYellow);
    // }
    // painter->setBrush(gradient);

    // painter->setPen(QPen(Qt::black, 0));
    painter->setPen(QPen(Qt::black, 1));
    painter->setBrush(QBrush(color_map[type]));
    // painter->setPen(QPen(color_map[type], 1));
    painter->drawEllipse(-10, -10, 20, 20);
}

void qtreeNode::set_id(int id) { idx = id; }
int qtreeNode::get_id() { return idx; }

QVariant qtreeNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        foreach (qtreeEdge *edge, edgeList) {
            edge->updatePosition();
        }
    } else if (change == QGraphicsItem::ItemSelectedChange ) {
        if (value == true) {
            std::cout << "node " << idx << " is selected " << std::endl;
        }
    }
    return QGraphicsItem::itemChange(change, value);
    // return value;
}

// void qtreeNode::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
//     std::cout << "hovering" << std::endl;
//     update();
// }

void qtreeNode::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    std::cout << "press" << std::endl;
    std::cout << idx << " " << type << std::endl;
}