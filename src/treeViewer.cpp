#include "treeViewer.h"
#include <math.h>
// #include <QStyle>
#include <QPainter>
#include <QStyleOption>
#include "qtreeNode.h"
#include "qtreeEdge.h"
#include "NodeVisitor.h"


treeViewer::treeViewer(QWidget *parent) : QGraphicsView(parent) {
    m_pScene = new QGraphicsScene(this);
    m_pScene->setItemIndexMethod(QGraphicsScene::NoIndex);
    // set background color.
    // setBackgroundBrush(QBrush(Qt::red, Qt::SolidPattern));
    m_pScene->setSceneRect(-200, -200, 200, 200);
    setScene(m_pScene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(0.8), qreal(0.8));
    setMinimumSize(200, 200);
    m_pTree = NULL;
    std::cout << "tree viewer " << std::endl;
}

treeViewer::treeViewer(Tree *tree, QWidget *parent) : QGraphicsView(parent) {
    m_pScene = new QGraphicsScene(this);
    m_pScene->setItemIndexMethod(QGraphicsScene::NoIndex);
    m_pScene->setSceneRect(-200, -200, 200, 200);
    setScene(m_pScene);
    // QVBoxLayout *treeViewerLayout = new QVBoxLayout(this);
    // tree
    m_pTree = tree;
    // buildVizTree(m_pTree);
}

treeViewer::~treeViewer() {}

void treeViewer::setTree(Tree* tree) {
    m_pTree = tree;
    GeometryEvaluator geomevaluator(*m_pTree);
    CSGTreeEvaluator csgrenderer(*m_pTree, &geomevaluator);
    // this->csgRoot = csgrenderer.buildCSGTree(*m_pTree->root());
    buildVizTree(m_pTree);
}

void treeViewer::draw_and_traverse(const AbstractNode &node) {
    qtreeNode* qnode = new qtreeNode(this);
    m_pScene->addItem(qnode);
    for (const auto &chnode: node.getChildren()) {
        std::cout << "this node has " << node.getChildren().size() << " children" << std::endl; 
        draw_and_traverse(*chnode);
    }
}

void treeViewer::buildVizTree(Tree *tree) {
    const AbstractNode* root = tree->root();
    // check how many children this root got
    std::vector<AbstractNode*> childs = root->getChildren();
    std::cout << "this root has " << childs.size() << " children" << std::endl; 
    std::cout << "root id : " << root->idx << " " << root->name() << std::endl;
    std::cout << "child 0 id : " << childs[0]->idx << " " << childs[0]->name() << std::endl;
    std::cout << "child 1 id : " << childs[1]->idx << " " << childs[0]->name() << std::endl;

    GeometryEvaluator geomevaluator(*m_pTree);
    CSGVisitor visitor(*tree, &geomevaluator);
    visitor.buildCSGTree(*root);
    // shared_ptr<CSGNode> csgroot = visitor.getRootNode();
    // if (csgroot == nullptr) {
    //     std::cout << "NULL" << std::endl;
    // }
    
    // visitor.traverse(*root);
    // shared_ptr<CSGNode> csgroot = visitor.getRootNode();
    // traverse(root);
    // draw_and_traverse(*root);
    // QList<QGraphicsItem*> items = m_pScene->items();
    // std::cout << "In total " << items.length() << " items in the scene." << std::endl; 

    // qtreeNode* root_node = new qtreeNode(this);
    // m_pScene->addItem(root_node);
    // root_node->setPos(-50, -50);
    // for (const auto &chnode : node.getChildren()) {
        
    // }
}

// assume very few level of tree
void treeViewer::simpleVizTree(Tree *tree) {
    const AbstractNode* root = tree->root();
    qtreeNode* root_node = new qtreeNode(this);
    m_pScene->addItem(root_node);
    root_node->setPos(-50, -50);
    // int level = 3;
    // for (int i = 0; i < level; i++) {
        
    // }
}

// Node definition
// Node::Node(treeViewer *viewer) {
//     m_pViewer = viewer;
//     // setFlag(ItemIsMovable);
//     setFlag(ItemSendsGeometryChanges);
//     setCacheMode(DeviceCoordinateCache);
//     setZValue(-1);
// }

// QList<Edge*> Node::edges() const
// {
//     return edgeList;
// }

// QRectF Node::boundingRect() const
// {
//     qreal adjust = 2;
//     return QRectF( -10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);
// }

// QPainterPath Node::shape() const
// {
//     QPainterPath path;
//     path.addEllipse(-10, -10, 20, 20);
//     return path;
// }

// void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
// {
//     painter->setPen(Qt::NoPen);
//     painter->setBrush(Qt::darkGray);
//     painter->drawEllipse(-7, -7, 20, 20);

//     QRadialGradient gradient(-3, -3, 10);
//     if (option->state & QStyle::State_Sunken) {
//         gradient.setCenter(3, 3);
//         gradient.setFocalPoint(3, 3);
//         gradient.setColorAt(1, QColor(Qt::yellow).light(120));
//         gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));
//     } else {
//         gradient.setColorAt(0, Qt::yellow);
//         gradient.setColorAt(1, Qt::darkYellow);
//     }
//     painter->setBrush(gradient);

//     painter->setPen(QPen(Qt::black, 0));
//     painter->drawEllipse(-10, -10, 20, 20);
// }

// QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
// {
//     // switch (change) {
//     // case ItemPositionHasChanged:
//     //     foreach (Edge *edge, edgeList)
//     //         edge->adjust();
//     //     graph->itemMoved();
//     //     break;
//     // default:
//     //     break;
//     // };

//     return QGraphicsItem::itemChange(change, value);
// }

// void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
// {
//     update();
//     QGraphicsItem::mousePressEvent(event);
// }

// void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
// {
//     update();
//     QGraphicsItem::mouseReleaseEvent(event);
// }