#include "treeViewer.h"
#include <math.h>
// #include <QStyle>
#include <QPainter>
#include <QStyleOption>
#include <QWheelEvent>
#include "qtreeNode.h"
#include "qtreeEdge.h"
#include "NodeVisitor.h"
#include "csgnode.h"
#include "transformnode.h"
#include "csgops.h"

treeViewer::treeViewer(QWidget *parent) : QGraphicsView(parent) {
    srand((unsigned)time(0));
    m_pScene = new QGraphicsScene(this);
    m_pScene->setItemIndexMethod(QGraphicsScene::NoIndex);
    // set background color.
    // setBackgroundBrush(QBrush(Qt::red, Qt::SolidPattern));
    // m_pScene->setSceneRect(-200, -200, 400, 400);
    // 400, 400
    // TODO: how to zoom in or change this size accorindg to different trees.
    m_pScene->setSceneRect(0, 0, 400, 400);
    scene_width = m_pScene->width();
    scene_height = m_pScene->height();
    setScene(m_pScene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    // scale(qreal(0.8), qreal(0.8));   
    setMinimumSize(200, 200);
    m_pTree = NULL;
    cur_zoom = 1.0;

    rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    rb_rect = QRect(0,0,1,1);
    rubberBand->setGeometry(rb_rect);
    // rubberBand->setGeometry(0,0,1,1);
    // ru
    draw_rubberband = false;

    // setDragMode(QGraphicsView::RubberBandDrag);
    setRubberBandSelectionMode(Qt::ContainsItemShape);

    // rubberBand->show();
    // m_selectionRectangle = new QGraphicsRectItem(0,0,1,1);
    // m_selectionRectangle->setBrush(Qt::magenta);
    // m_selectionRectangle->setOpacity(0.2);
    // std::cout << "tree viewer " << std::endl;
}

treeViewer::treeViewer(Tree *tree, QWidget *parent) : QGraphicsView(parent) {
    srand((unsigned)time(0));
    m_pScene = new QGraphicsScene(this);
    m_pScene->setItemIndexMethod(QGraphicsScene::NoIndex);
    m_pScene->setSceneRect(-200, -200, 400, 400);
    setScene(m_pScene);
    // QVBoxLayout *treeViewerLayout = new QVBoxLayout(this);
    // tree
    m_pTree = tree;
    // buildVizTree(m_pTree);
}

treeViewer::~treeViewer() {}

QPointF treeViewer::rand_pos() {
    QRectF scene_rect = m_pScene->sceneRect();
    // int width = scene_rect.width();
    // int height = scene_rect.height();
    QPointF top_left = scene_rect.topLeft();
    float rand_w = rand()%scene_width;
    float rand_h = rand()%scene_height;
    return QPointF(rand_w, rand_h);
    // return QPointF(top_left.x()+rand_w, top_left.y()+rand_h);
}

void treeViewer::setName(QString _name) {
    this->name = _name;
}

// might be problematic..
void treeViewer::setTree(Tree* tree) {
    m_pTree = tree;
    // GeometryEvaluator geomevaluator(*m_pTree);
    // CSGTreeEvaluator csgrenderer(*m_pTree, &geomevaluator);
    // this->csgRoot = csgrenderer.buildCSGTree(*m_pTree->root());
    clear_scene();
    buildVizTree(m_pTree);
}

void treeViewer::clear_scene() {
    m_pScene->clear();
}

void treeViewer::draw_and_traverse(const AbstractNode &node, qtreeNode *parent_node) {
    qtreeNode* qnode = new qtreeNode(this);
    m_pScene->addItem(qnode);
    // // connect self node with parent
    // if (!parent_node) {
    //     m_pScene->addItem(new qtreeEdge(qnode, parent_node));
    // }

    // for (const auto &chnode: node.getChildren()) {
    //     std::cout << "this node has " << node.getChildren().size() << " children" << std::endl; 
    //     draw_and_traverse(*chnode);
    // }
}

void treeViewer::setSTree(tree_hnode* htree) {
    clear_scene();
    QMap<int, qtreeNode*> qtreenodes;
	tree_hnode::sibling_iterator children;
	tree_hnode::iterator iterator;
	iterator = htree->begin();
    while(iterator!= htree->end()) {
        std::string type = (*iterator)->type;
        qtreeNode *qnode = new qtreeNode(this, type);
        qnode->set_id((*iterator)->idx);
        QPointF pos;
        pos.setX(scene_width-(*iterator)->pos_x);
        pos.setY(scene_height-(*iterator)->pos_y);
        qnode->setPos(pos);
        // qnode->setPos(rand_pos());
        qtreenodes.insert((*iterator)->idx, qnode);
        m_pScene->addItem(qnode);
        ++iterator;
    }
    iterator = htree->begin();
    while (iterator!=htree->end()) {
        children = htree->begin(iterator);
        int self_idx = (*iterator)->idx;
        while(children != htree->end(iterator)) {
            int child_idx = (*children)->idx;
            qtreeEdge *edge = new qtreeEdge(qtreenodes[self_idx], qtreenodes[child_idx]);
            m_pScene->addItem(edge);
            ++children;
        }
        ++iterator;
    }
    std::cout << "there are " << m_pScene->items().size() << " items in the scene" << std::endl;
    
}


void treeViewer::wheelEvent(QWheelEvent *event) {
    if(event->delta() > 0)
    {
        this->scale(2,2);
    }
    else
    {
        this->scale(0.5,0.5);
    }
}

void treeViewer::mousePressEvent(QMouseEvent *e) {
    if (e->button() == Qt::RightButton) {
        if (e->modifiers() == Qt::ControlModifier) {
            std::cout << "ctrl pressed " << std::endl;
            rubberBand->show();
            rb_start = e->pos();
            draw_rubberband = true;
            // if (rubberBand->geometry().contains(e->pos())) {
            //     rubberband_offset = e->pos() - rubberBand->pos();
            //     move_rubberband = true;
                
            // }
        }
    } 
    QGraphicsView::mousePressEvent(e);
}

void treeViewer::mouseMoveEvent(QMouseEvent *e) {
    if (draw_rubberband) {
        rubberBand->setGeometry(QRect(rb_start, e->pos()));
    }
    // if(move_rubberband)
    // {   
    //     rubberBand->move(e->pos() - rubberband_offset);
    // }
    // if (event->button() == Qt::RightButton) {
    //     std::cout << this->name.toStdString() << std::endl;
    // }
    QGraphicsView::mouseMoveEvent(e);
}
void treeViewer::mouseReleaseEvent(QMouseEvent *e) {
    if (e->button() == Qt::RightButton) {
        std::cout << "release event" << std::endl;
        rb_end = e->pos();
        rb_rect = QRect(rb_start, rb_end);
        rubberBand->setGeometry(rb_rect);
        draw_rubberband = false;
        rubberBand->hide();
        // check if we can get the selected Item..
        QPainterPath selectionArea;
        selectionArea.addPolygon(mapToScene(rb_rect));
        // QRectF bb_ssarea = selectionArea.boundingRect();
        // std::cout << bb_ssarea.width() << " " << bb_ssarea.height() << std::endl;
        selectionArea.closeSubpath();
        m_pScene->setSelectionArea(selectionArea, viewportTransform());
        QPainterPath return_area = m_pScene->selectionArea();
        QRectF bb_area = return_area.boundingRect();
        std::cout << bb_area.width() << " " << bb_area.height() << std::endl;

        QList<QGraphicsItem*> selected_items = m_pScene->selectedItems();
        if (selected_items.isEmpty()) { std::cout << "empty" << std::endl; }
        std::cout << selected_items.length() << " items are selected " << std::endl;
        rubberBand->setGeometry(0,0,1,1);
    }
    // if (event->button() == Qt::RightButton) {
    //     std::cout << this->name.toStdString() << std::endl;
    // }
    QGraphicsView::mouseReleaseEvent(e);
}


//////////////////////////////////
void treeViewer::buildVizTree(Tree *tree) {
    // simple test for edge
    // qtreeNode* node0 = new qtreeNode(this);
    // m_pScene->addItem(node0);
    // node0->setPos(-100, -100);
    // qtreeNode* node1 = new qtreeNode(this);
    // m_pScene->addItem(node1);
    // node1->setPos(0, 0);
    // qtreeEdge *edge = new qtreeEdge(node0, node1);
    // m_pScene->addItem(edge);
    // node0->setPos(-100, -100);
    // node1->setPos(0, 0);

    const AbstractNode* root = tree->root();
    // if (root) {
    //     this->traverse(*root);
    // }
    // std::cout << "there are " << m_pScene->items().size() << " items in the scene" << std::endl;
    // check how many children this root got
    // std::vector<AbstractNode*> childs = root->getChildren();
    // std::cout << "this root has " << childs.size() << " children" << std::endl; 
    // std::cout << "root id : " << root->idx << " " << root->name() << std::endl;
    // std::cout << "child 0 id : " << childs[0]->idx << " " << childs[0]->name() << std::endl;
    // // std::cout << "child 1 id : " << childs[1]->idx << " " << childs[1]->name() << std::endl;

    // std::vector<AbstractNode*> chchilds = childs[0]->getChildren();
    // std::cout << "this child has " << chchilds.size() << " children" << std::endl; 
    // GeometryEvaluator geomevaluator(*m_pTree);
    // CSGVisitor visitor(*tree, &geomevaluator);
    // shared_ptr<CSGNode> csgroot = visitor.buildCSGTree(*root);



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
    root_node->setPos(-100, -100);
    // int level = 3;
    // for (int i = 0; i < level; i++) {
        
    // }
}

// Response treeViewer::visit(State &state, const AbstractNode &node)
// {
// 	// if (state.isPostfix()) {
// 	// 	applyToChildren(state, node, OpenSCADOperator::UNION);
// 	// 	addToParent(state, node);
// 	// }
// 	return Response::ContinueTraversal;
// }

// Response treeViewer::visit(State &state, const AbstractPolyNode &node) {
//     if (state.isPrefix()) {
//         std::cout << "draw poly node" << std::endl;
//         qtreeNode* poly_node = new qtreeNode(this, "poly");
//         poly_node->set_id(node.idx);
//         poly_node->setPos(rand_pos());
//         m_pScene->addItem(poly_node);
//         node_map.insert(node.idx, poly_node);
//         // // find the parent and add edge
//         if (state.parent() != nullptr) {
//             int parent_id = state.parent()->idx;
//             qtreeEdge *edge = new qtreeEdge(poly_node, node_map[parent_id]);
//             m_pScene->addItem(edge);
//         }
//     }
//     return Response::ContinueTraversal;
// }
// // function for handling the visit for root node..
// Response treeViewer::visit(State &state, const RootNode &node)
// {
//     if (state.isPrefix()) {
//         std::cout << "draw root node" << std::endl;
//         qtreeNode* root_node = new qtreeNode(this, "root");
//         root_node->set_id(node.idx);
//         // root_node->setPos(-150, -150);
//         root_node->setPos(rand_pos());
//         m_pScene->addItem(root_node);
//         node_map.insert(node.idx, root_node);
//     }
// 	// if (isCached(node)) return Response::PruneTraversal;
// 	if (state.isPostfix()) {
//         // std::cout << "in treeviewer go pass root.." << std::endl;
// 	// 	std::stringstream dump;
// 	// 	dump << dumpChildren(node);
// 	// 	this->cache.insert(node, dump.str());
// 	}

// 	// handleVisitedChildren(state, node);
// 	return Response::ContinueTraversal;
// }

// Response treeViewer::visit(State &state, const TransformNode &node) {
//     if (state.isPrefix()) {
//         std::cout << "draw transformation node" << std::endl;
//         qtreeNode* trans_node = new qtreeNode(this, "trans");
//         trans_node->set_id(node.idx);
//         trans_node->setPos(rand_pos());
//         m_pScene->addItem(trans_node);
//         node_map.insert(node.idx, trans_node);
//         // // find the parent and add edge
//         if (state.parent() != nullptr) {
//             int parent_id = state.parent()->idx;
//         //     // std::cout << "self id : " << node.idx << std::endl;
//         //     // std::cout << "parent id : " << parent_id << std::endl;
//             qtreeEdge *edge = new qtreeEdge(trans_node, node_map[parent_id]);
//             m_pScene->addItem(edge);
//         }
//     }
//     return Response::ContinueTraversal;
// }

// // might be problematic..
// Response treeViewer::visit(State &state, const CsgOpNode &node) {
//     if (state.isPrefix()) {
//         std::cout << "draw csg opt node" << std::endl;
//         qtreeNode* csgopt_node = new qtreeNode(this, "csg_opt");
//         csgopt_node->set_id(node.idx);
//         csgopt_node->setPos(rand_pos());
//         m_pScene->addItem(csgopt_node);
//         node_map.insert(node.idx, csgopt_node);
//         if (state.parent() != nullptr) {
//             int parent_id = state.parent()->idx;
//             qtreeEdge *edge = new qtreeEdge(csgopt_node, node_map[parent_id]);
//             m_pScene->addItem(edge);
//         }
//     }
//     return Response::ContinueTraversal;
// }

// Response treeViewer::visit(State &state, const GroupNode &node)
// {
//     if (state.isPrefix()) {
//         std::cout << "draw group node"  << std::endl;
//         qtreeNode *group_node = new qtreeNode(this, "group");
//         group_node->set_id(node.idx);
//         group_node->setPos(rand_pos());
//         m_pScene->addItem(group_node);
//         node_map.insert(node.idx, group_node);
//         if (state.parent() != nullptr) {
//             int parent_id = state.parent()->idx;
//             qtreeEdge *edge = new qtreeEdge(group_node, node_map[parent_id]);
//             m_pScene->addItem(edge);
//         }
//     }
// 	// if (state.isPostfix()) {
// 	// 	applyToChildren(state, node, OpenSCADOperator::UNION);
// 	// 	addToParent(state, node);
// 	// }
// 	return Response::ContinueTraversal;
// }

void treeViewer::setBTree(bTree *btree) {
    clear_scene();
    QMap<int, qtreeNode*> qtreenodes;
    std::vector<qtreeEdge*> qtreeedges;
    auto vs = boost::vertices(*btree);
    int index = 0;
    for (auto vit = vs.first; vit != vs.second; ++vit) {
        // NODETYPE type = (*btree)[*vit].type;
        std::string type = (*btree)[*vit].type;
        qtreeNode* qnode = new qtreeNode(this, type);
        qnode->set_id((*btree)[*vit].idx);
        QPointF pos;
        pos.setX(scene_width-(*btree)[*vit].pos[0]);
        pos.setY(scene_height-(*btree)[*vit].pos[1]);
        qnode->setPos(pos);
        // qnode->setPos(rand_pos());
        qtreenodes.insert((*btree)[*vit].idx, qnode);
        m_pScene->addItem(qnode);
        // if (type == NODETYPE::ROOT) {
        //     qtreeNode* qnode = new qtreeNode(this, "root");
        //     qnode->set_id((*btree)[*vit].idx);
        //     // QPointF pos;
        //     // pos.setX((*btree)[*vit].pos[0]*scene_width);
        //     // pos.setY((*btree)[*vit].pos[1]*scene_height);
        //     // qnode->setPos(pos);
        //     qnode->setPos(rand_pos());
        //     qtreenodes.insert((*btree)[*vit].idx, qnode);
        //     m_pScene->addItem(qnode);
        // } else if (type == NODETYPE::GROUP) {
        //     qtreeNode* qnode = new qtreeNode(this, "group");
        //     qnode->set_id((*btree)[*vit].idx);
        //     // QPointF pos;
        //     // pos.setX((*btree)[*vit].pos[0]*scene_width);
        //     // pos.setY((*btree)[*vit].pos[1]*scene_height);
        //     // qnode->setPos(pos);
        //     qnode->setPos(rand_pos());
        //     qtreenodes.insert((*btree)[*vit].idx, qnode);
        //     m_pScene->addItem(qnode);
        // } else if (type == NODETYPE::AB_POLY) {
        //     qtreeNode* qnode = new qtreeNode(this, "poly");
        //     qnode->set_id((*btree)[*vit].idx);
        //     // QPointF pos;
        //     // pos.setX((*btree)[*vit].pos[0]*scene_width);
        //     // pos.setY((*btree)[*vit].pos[1]*scene_height);
        //     // qnode->setPos(pos);
        //     qnode->setPos(rand_pos());
        //     qtreenodes.insert((*btree)[*vit].idx, qnode);
        //     m_pScene->addItem(qnode);
        // } else if (type == NODETYPE::TRANSFORMATION) {
        //     qtreeNode* qnode = new qtreeNode(this, "trans");
        //     qnode->set_id((*btree)[*vit].idx);
        //     // QPointF pos;
        //     // pos.setX((*btree)[*vit].pos[0]*scene_width);
        //     // pos.setY((*btree)[*vit].pos[1]*scene_height);
        //     // qnode->setPos(pos);
        //     qnode->setPos(rand_pos());
        //     qtreenodes.insert((*btree)[*vit].idx, qnode);
        //     m_pScene->addItem(qnode);
        // } else if (type == NODETYPE::CSGOPT) {
        //     qtreeNode* qnode = new qtreeNode(this, "csg_opt");
        //     qnode->set_id((*btree)[*vit].idx);
        //     // QPointF pos;
        //     // pos.setX((*btree)[*vit].pos[0]*scene_width);
        //     // pos.setY((*btree)[*vit].pos[1]*scene_height);
        //     // qnode->setPos(pos);
        //     qnode->setPos(rand_pos());
        //     qtreenodes.insert((*btree)[*vit].idx, qnode);
        //     m_pScene->addItem(qnode);
        // }
    }

    auto es = boost::edges(*btree);
    for (auto eit = es.first; eit != es.second; ++eit) {
        vertex_d src = source(*eit, *btree), tar = target(*eit, *btree);
        std::cout << src << " " << tar << std::endl;
        qtreeEdge *edge = new qtreeEdge(qtreenodes[(*btree)[src].idx], qtreenodes[(*btree)[tar].idx]);
        m_pScene->addItem(edge);
    }
    std::cout << "there are " << m_pScene->items().size() << " items in the scene" << std::endl;
}
