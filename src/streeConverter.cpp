#include "streeConverter.h"
#include <boost/type_index.hpp>
#include <typeinfo>

streeConverter::streeConverter(Tree *tree) {
    this->m_Tree = new tree_hnode();

}
streeConverter::~streeConverter() {}


Response streeConverter::visit(State &state, const AbstractNode &node) {
    return Response::ContinueTraversal;
}
Response streeConverter::visit(State &state, const RootNode &node) {
    hnode* _node = new hnode();
    if (state.isPostfix()) {
        _node->type = "root";
        _node->idx = node.index();
        _node->node = &node;
        _node->parent_idx = -1;
        hnodes.insert(node.index(), _node);
        for (auto child : visitedchildren[node.index()]) {
            child->parent_idx = node.index();
        }
        // hnodes.push_back(_node);
        // tree_hnode::iterator newNodeIter;
        // newNodeIter = m_Tree->begin();
        // newNodeIter = m_Tree->insert(newNodeIter, *_node);
        // node_iters.push_back(newNodeIter);
        // add_children(newNodeIter, visitedchildren[node.index()]);
        // _node.
        // root = add_vertex(m_BTree);
        // m_BTree[root].type = "root";
        // m_BTree[root].idx = node.index();
        // set_relation(node, root);
    }
    handleVisitedChildren(state, node, _node);
    return Response::ContinueTraversal;
}
Response streeConverter::visit(State &state, const AbstractPolyNode &node) {
    hnode* _node = new hnode();
    if (state.isPostfix()) {
        _node->type = "poly";
        _node->idx = node.index();
        _node->node = &node;
        hnodes.insert(node.index(), _node);
        for (auto child : visitedchildren[node.index()]) {
            child->parent_idx = node.index();
        }
    }
    handleVisitedChildren(state, node, _node);
    return Response::ContinueTraversal;
}
Response streeConverter::visit(State &state, const TransformNode &node) {
    hnode* _node = new hnode();
    if (state.isPostfix()) {
        _node->type = "trans";
        _node->idx = node.index();
        _node->node = &node;
        hnodes.insert(node.index(), _node);
        for (auto child : visitedchildren[node.index()]) {
            child->parent_idx = node.index();
        }
    }
    handleVisitedChildren(state, node, _node);
    return Response::ContinueTraversal;
}
Response streeConverter::visit(State &state, const CsgOpNode &node) {
    hnode* _node = new hnode();
    if (state.isPostfix()) {
        _node->type = "csg_opt";
        _node->idx = node.index();
        _node->node = &node;
        hnodes.insert(node.index(), _node);
        for (auto child : visitedchildren[node.index()]) {
            child->parent_idx = node.index();
        }
    }
    handleVisitedChildren(state, node, _node);
    return Response::ContinueTraversal;
}
Response streeConverter::visit(State &state, const GroupNode &node) {
    hnode* _node = new hnode();
    if (state.isPostfix()) {
        _node->type = "group";
        _node->idx = node.index();
        _node->node = &node;
        hnodes.insert(node.index(), _node);
        for (auto child : visitedchildren[node.index()]) {
            child->parent_idx = node.index();
        }
    }
    handleVisitedChildren(state, node, _node);
    return Response::ContinueTraversal;
}

void streeConverter::handleVisitedChildren(const State &state, const AbstractNode &node, hnode* hier_node) {
    if (state.isPostfix()) {
        this->visitedchildren.erase(node.index());
        if (!state.parent()) {
        } else {
            this->visitedchildren[state.parent()->index()].push_back(hier_node);
            hier_node->parent_idx = state.parent()->index();
        }
    }
}



void streeConverter::add_children(tree_hnode::iterator parent_node, childNodeList childs) {
    // for (auto child : childs) {
    //     // m_Tree->append_child(parent_node, *child);
    // }
}

tree_hnode* streeConverter::convert_graph(Tree *tree) {
    traverse(*tree->root());
    // should have a map with all hnodes w/ parent info..
    int node_count = hnodes.size();
    for (int i = 0; i < node_count; i++) {
        tree_hnode::iterator newNodeIter;
        int parent_id = hnodes[i]->parent_idx;
        if (parent_id == -1) {
            newNodeIter = m_Tree->begin();
			newNodeIter = m_Tree->insert(newNodeIter, *hnodes[i]);
        } else {
            newNodeIter = m_Tree->append_child(hnode_iters[parent_id], *hnodes[i]);
        }
        hnode_iters.insert(i, newNodeIter);
    }
    return m_Tree;
}
