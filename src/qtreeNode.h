#pragma once

#include <QGraphicsItem>
#include <QList>
#include <string>

class qtreeEdge;
class treeViewer;
using namespace std;

class qtreeNode : public QObject, public QGraphicsItem {
    Q_OBJECT
    public:
        qtreeNode(treeViewer *viewer);
        qtreeNode(treeViewer *viewer, string node_type);
        ~qtreeNode();
        void addEdge(qtreeEdge *edge);
        QList<qtreeEdge *> edges() const;
        QRectF boundingRect() const override;
        QPainterPath shape() const override;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
        // set the node idx in the corresponding Tree
        void set_id(int idx);
        int get_id();
        
        bool my_selected;
    // private:
        QList<qtreeEdge*> edgeList;
        QPointF newPos;
        treeViewer *m_pViewer;
        string type;
        QList<qtreeNode*> childList;
        qtreeNode *parent_node;
        QMap<string, QColor> color_map;
        int idx;
        int parent_idx;

        // bool my_selected;
    protected:  
        QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
        // void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    signals:
        void select_childrens(int, bool);
};