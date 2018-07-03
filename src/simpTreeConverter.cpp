#include "simpTreeConverter.h"
#include <boost/type_index.hpp>
#include <typeinfo>


simpTreeConverter::simpTreeConverter() {
    this->m_Tree = new tree_hnode();
    this->has_csginfo = false;
}

simpTreeConverter::simpTreeConverter(Tree *tree) {
    this->m_Tree = new tree_hnode();
}

simpTreeConverter::simpTreeConverter(Tree *tree, class GeometryEvaluator *geomevaluator) : m_geomeval(geomevaluator) {}

simpTreeConverter::~simpTreeConverter() {}

Response simpTreeConverter::visit(State &state, const AbstractNode &node) {
    return Response::ContinueTraversal;
}

Response simpTreeConverter::visit(State &state, const RootNode &node) {
    hnode* _node = new hnode();
    if (state.isPrefix()) {
        parent_db[node.index()] = -1;
    }
    if (state.isPostfix()) {
        _node->type = "root";
        _node->idx = node.index();
        _node->node = &node;
        if (this->has_csginfo) {
            _node->csgnode = this->tree_stored_leaf_term[node.index()];
            _node->obj_filename = QString("%1.obj").arg(node.index()).toStdString();
        }
        // _node.parent_idx = -1;
        // // hnodes[node.index()] = _node;
        hnodes.insert(node.index(), _node);
        for (auto child : visitedchildren[node.index()]) {
            child->parent_idx = node.index();
        }
    }
    handleVisitedChildren(state, node, _node);
    return Response::ContinueTraversal;
}

Response simpTreeConverter::visit(State &state, const TransformNode &node) {
    hnode* _node = new hnode();
    if (state.isPrefix()) {
        parent_db[node.index()] = state.parent()->index();
    }
    if (state.isPostfix()) {
        _node->type = "trans";
        _node->idx = node.index();
        _node->node = &node;
        if (this->has_csginfo) {
            _node->csgnode = this->tree_stored_leaf_term[node.index()];
            _node->obj_filename = QString("%1.obj").arg(node.index()).toStdString();
        }
        hnodes.insert(node.index(), _node);
        for (auto child : visitedchildren[node.index()]) {
            child->parent_idx = node.index();
        }
    }
    handleVisitedChildren(state, node, _node);
    return Response::ContinueTraversal;
}

Response simpTreeConverter::visit(State &state, const AbstractPolyNode &node) {
    hnode* _node = new hnode();
    if (state.isPrefix()) {
        parent_db[node.index()] = state.parent()->index();
    }
    if (state.isPostfix()) {
        _node->type = "poly";
        _node->idx = node.index();
        _node->node = &node;
        if (this->has_csginfo) {
            _node->csgnode = this->tree_stored_leaf_term[node.index()];
            _node->obj_filename = QString("%1.obj").arg(node.index()).toStdString();
        }
        hnodes.insert(node.index(), _node);
        for (auto child : visitedchildren[node.index()]) {
            child->parent_idx = node.index();
        }
        // // do the geom eval
        // if (this->m_geomeval) {
        //     std::cout << "do the geom eval for node idx : " << node.idx << std::endl;
        //     auto geom = this->m_geomeval->evaluateGeometry(node, false);
        //     _node->geom = geom;
        // }
    }
    handleVisitedChildren(state, node, _node);
    return Response::ContinueTraversal;
}

Response simpTreeConverter::visit(State &state, const CsgOpNode &node) {
    hnode* _node = new hnode();
    if (state.isPrefix()) {
        parent_db[node.index()] = state.parent()->index();
    }
    if (state.isPostfix()) {
        _node->type = "csg_opt";
        _node->idx = node.index();
        _node->node = &node;
        if (this->has_csginfo) {
            _node->csgnode = this->tree_stored_leaf_term[node.index()];
            _node->obj_filename = QString("%1.obj").arg(node.index()).toStdString();
        }
        hnodes.insert(node.index(), _node);
        for (auto child : visitedchildren[node.index()]) {
            child->parent_idx = node.index();
        }
    }
    handleVisitedChildren(state, node, _node);
    return Response::ContinueTraversal;
}

Response simpTreeConverter::visit(State &state, const CgaladvNode &node) {
    hnode* _node = new hnode();
    if (state.isPrefix()) {
        parent_db[node.index()] = state.parent()->index();
    }
    if (state.isPostfix()) {
        _node->type = "cgal_adv";
        _node->idx = node.index();
        _node->node = &node;
        if (this->has_csginfo) {
            _node->csgnode = this->tree_stored_leaf_term[node.index()];
            _node->obj_filename = QString("%1.obj").arg(node.index()).toStdString();
        }
        hnodes.insert(node.index(), _node);
        for (auto child : visitedchildren[node.index()]) {
            child->parent_idx = node.index();
        }
    }
    handleVisitedChildren(state, node, _node);
    return Response::ContinueTraversal;
}

Response simpTreeConverter::visit(State &state, const GroupNode &node) {
    hnode* _node = new hnode();
    if (state.isPrefix()) {
        parent_db[node.index()] = state.parent()->index();
    }
    if (state.isPostfix()) {
        _node->type = "group";
        _node->idx = node.index();
        _node->node = &node;
        // can we convert them all to Leaf node..
        if (this->has_csginfo) {
            _node->csgnode = this->tree_stored_leaf_term[node.index()];
            _node->obj_filename = QString("%1.obj").arg(node.index()).toStdString();
        }
        hnodes.insert(node.index(), _node);
        for (auto child : visitedchildren[node.index()]) {
            child->parent_idx = node.index();
        }
    }
    handleVisitedChildren(state, node, _node);
    return Response::ContinueTraversal;
}

void simpTreeConverter::handleVisitedChildren(const State &state, const AbstractNode &node, hnode* hier_node) {
    if (state.isPostfix()) {
        // this->visitedchildren.erase(node.index());
        if (!state.parent()) {
            
        } else {
            this->visitedchildren[state.parent()->index()].push_back(hier_node);
            // hier_node->parent_idx = state.parent()->index();
        }
    }
}



void simpTreeConverter::add_children(tree_hnode::iterator parent_node, childNodeList childs) {
    // for (auto child : childs) {
    //     // m_Tree->append_child(parent_node, *child);
    // }
}

int simpTreeConverter::search_valid_parent(int id) {
    std::cout << "search for valid parent.." << std::endl;
    bool found = false;
    int parent_id = -1;
    int cur_id = id;
    std::cout << "cur_id : " << cur_id << std::endl;
    while (!found) {
        parent_id = parent_db[cur_id];
        if (parent_id == -1) {
            return parent_id;
        }
        if (hnodes[parent_id]->type == "trans" || hnodes[parent_id]->type == "group") {
            cur_id = parent_id;
        } else {
            return parent_id;
        }
    }
    return -1;
}

tree_hnode* simpTreeConverter::convert_tree(Tree *tree) {
    // TODO : test if we can obtain the right stored information here...
    this->tree_stored_leaf_term = tree->csg_stored_leaf_term;
    // std::cout << "tree stored term size : " << this->tree_stored_term.size() << std::endl;
    if (this->tree_stored_leaf_term.size() != 0) {
        this->has_csginfo = true;
    }
    traverse(*tree->root());
    // should have a map with all hnodes w/ parent info..
    // int node_count = hnodes.size();
    // std::cout << "node count : " << node_count << std::endl;
    // std::cout << hnodes[1]->type << std::endl;
    QList<int> keys = hnodes.keys();
    for (int k : keys) {
        tree_hnode::iterator newNodeIter;
        if (hnodes[k]->type == "trans" || hnodes[k]->type == "group") {
            continue;
        }
        // int parent_id = hnodes[k]->parent_idx;
        int parent_id = search_valid_parent(k);
        std::cout << k << " parent id : " << parent_id << std::endl;
        // change the parent information.
        hnodes[k]->parent_idx = parent_id;


        // std::cout << hnodes[k]->type << std::endl;
        if (parent_id == -1) {
            newNodeIter = m_Tree->begin();
            newNodeIter = m_Tree->insert(newNodeIter, hnodes[k]);
        } else {
            // check the parent type
            newNodeIter = m_Tree->append_child(hnode_iters[parent_id], hnodes[k]);
        }
        hnode_iters.insert(k, newNodeIter);
    }

    // // check the statistics of m_Tree
    // std::cout << "number of node : " << m_Tree->size() << std::endl;
    // // test to traverse the tree...
    // tree_hnode::sibling_iterator children;
	// tree_hnode::iterator iterator;
    // iterator = m_Tree->begin();
    // while (iterator != m_Tree->end()) {
    //     int index = (*iterator)->idx;
    //     std::cout << index << " " << (*iterator)->type <<  std::endl;
    //     ++iterator;
    // }

    return m_Tree;
}

shared_ptr<CSGLeaf> simpTreeConverter::convert_to_leaf(shared_ptr<CSGNode> input) {
    
}