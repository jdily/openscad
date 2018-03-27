#pragma once

#include <string>
#include <map>
#include <list>
#include "NodeVisitor.h"
#include "node.h"
#include "nodecache.h"
#include "Tree.h"


// TODO 
// 1. if we only want to remove part of the nodes rooted ny node, what can we do?

class NodeDeleter : public NodeVisitor {
public:
    NodeDeleter(Tree* tree) : m_pTree(tree), cache(tree->get_cache()) {
        std::cout << "node deleter here here" << std::endl;
    }
    ~NodeDeleter() {}

    // remove the entire subtree rooted by this node
    void remove_node(const AbstractNode &node, const AbstractNode &parent);

    Response visit(State &state, const AbstractNode &node) override;
    Response visit(State &state, const RootNode &node) override;
private:
    Tree* m_pTree;
    typedef std::vector<const AbstractNode *> ChildList;
    std::map<int, ChildList> visitedchildren;
    NodeCache &cache;
    const AbstractNode *root;

    void handleVisitedChildren(const State &state, const AbstractNode &node);
    
};