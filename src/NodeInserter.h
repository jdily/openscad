#pragma once

#include <string>
#include <map>
#include <list>
#include "NodeVisitor.h"
#include "node.h"
#include "nodecache.h"
#include "Tree.h"

// TODO 
// 1. if we only want to inset part of the nodes rooted ny node, what can we do?
class NodeInserter : public NodeVisitor {
public:
    NodeInserter(Tree *tree) : m_pTree(tree), cache(tree->get_cache()), idprefix(false) {
        cur_id = (int)cache.cur_size();
    }
    ~NodeInserter() {}
    // insert "insert_node" into m_pTree at parent_node as parent
    void insert_node(const AbstractNode &parent_node, const AbstractNode &insert_node);
    Response visit(State &state, const AbstractNode &node);
private:
    Tree *m_pTree;
    NodeCache &cache;
    std::string currindent;
    bool idprefix;
    typedef std::vector<const AbstractNode *> ChildList;
    std::map<int, ChildList> visitedchildren;
    // the id that can be inserted into the tree.
    int cur_id;
    void insert_children(const AbstractNode &node);
    void insert_children_block(const AbstractNode &node);
    void handleVisitedChildren(const State &state, const AbstractNode &node);
    void handleIndent(const State &state);
};