#include "treeViewer.h"
#include <math.h>
// #include <QStyle>
#include <QPainter>
#include <QStyleOption>
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
    m_pScene->setSceneRect(-200, -200, 400, 400);
    setScene(m_pScene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(0.8), qreal(0.8));
    setMinimumSize(200, 200);
    m_pTree = NULL;
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
    int width = scene_rect.width();
    int height = scene_rect.height();
    QPointF top_left = scene_rect.topLeft();
    float rand_w = rand()%width;
    float rand_h = rand()%height;
    return QPointF(top_left.x()+rand_w, top_left.y()+rand_h);
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

void treeViewer::setBTree(bTree *btree) {
    
}

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
    if (root) {
        this->traverse(*root);
    }
    std::cout << "there are " << m_pScene->items().size() << " items in the scene" << std::endl;
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

Response treeViewer::visit(State &state, const AbstractNode &node)
{
	// if (state.isPostfix()) {
	// 	applyToChildren(state, node, OpenSCADOperator::UNION);
	// 	addToParent(state, node);
	// }
	return Response::ContinueTraversal;
}

Response treeViewer::visit(State &state, const AbstractPolyNode &node) {
    if (state.isPrefix()) {
        std::cout << "draw poly node" << std::endl;
        qtreeNode* poly_node = new qtreeNode(this, "poly");
        poly_node->set_id(node.idx);
        poly_node->setPos(rand_pos());
        m_pScene->addItem(poly_node);
        node_map.insert(node.idx, poly_node);
        // // find the parent and add edge
        if (state.parent() != nullptr) {
            int parent_id = state.parent()->idx;
            qtreeEdge *edge = new qtreeEdge(poly_node, node_map[parent_id]);
            m_pScene->addItem(edge);
        }
    }
    return Response::ContinueTraversal;
}
// function for handling the visit for root node..
Response treeViewer::visit(State &state, const RootNode &node)
{
    if (state.isPrefix()) {
        std::cout << "draw root node" << std::endl;
        qtreeNode* root_node = new qtreeNode(this, "root");
        root_node->set_id(node.idx);
        root_node->setPos(-150, -150);
        m_pScene->addItem(root_node);
        node_map.insert(node.idx, root_node);
    }
	// if (isCached(node)) return Response::PruneTraversal;
	if (state.isPostfix()) {
        // std::cout << "in treeviewer go pass root.." << std::endl;
	// 	std::stringstream dump;
	// 	dump << dumpChildren(node);
	// 	this->cache.insert(node, dump.str());
	}

	// handleVisitedChildren(state, node);
	return Response::ContinueTraversal;
}

Response treeViewer::visit(State &state, const TransformNode &node) {
    if (state.isPrefix()) {
        std::cout << "draw transformation node" << std::endl;
        qtreeNode* trans_node = new qtreeNode(this, "trans");
        trans_node->set_id(node.idx);
        trans_node->setPos(rand_pos());
        m_pScene->addItem(trans_node);
        node_map.insert(node.idx, trans_node);
        // // find the parent and add edge
        if (state.parent() != nullptr) {
            int parent_id = state.parent()->idx;
        //     // std::cout << "self id : " << node.idx << std::endl;
        //     // std::cout << "parent id : " << parent_id << std::endl;
            qtreeEdge *edge = new qtreeEdge(trans_node, node_map[parent_id]);
            m_pScene->addItem(edge);
        }
    }
    return Response::ContinueTraversal;
}

// might be problematic..
Response treeViewer::visit(State &state, const CsgOpNode &node) {
    if (state.isPrefix()) {
        std::cout << "draw csg opt node" << std::endl;
        qtreeNode* csgopt_node = new qtreeNode(this, "csg_opt");
        csgopt_node->set_id(node.idx);
        csgopt_node->setPos(rand_pos());
        m_pScene->addItem(csgopt_node);
        node_map.insert(node.idx, csgopt_node);
        if (state.parent() != nullptr) {
            int parent_id = state.parent()->idx;
            qtreeEdge *edge = new qtreeEdge(csgopt_node, node_map[parent_id]);
            m_pScene->addItem(edge);
        }
    }
    return Response::ContinueTraversal;
}

Response treeViewer::visit(State &state, const GroupNode &node)
{
    if (state.isPrefix()) {
        std::cout << "draw group node"  << std::endl;
        qtreeNode *group_node = new qtreeNode(this, "group");
        group_node->set_id(node.idx);
        group_node->setPos(rand_pos());
        m_pScene->addItem(group_node);
        node_map.insert(node.idx, group_node);
        if (state.parent() != nullptr) {
            int parent_id = state.parent()->idx;
            qtreeEdge *edge = new qtreeEdge(group_node, node_map[parent_id]);
            m_pScene->addItem(edge);
        }
    }
	// if (state.isPostfix()) {
	// 	applyToChildren(state, node, OpenSCADOperator::UNION);
	// 	addToParent(state, node);
	// }
	return Response::ContinueTraversal;
}
