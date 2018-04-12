#pragma once

#include "stree.h"
#include "node.h"
#include "NodeVisitor.h"
#include <algorithm>
#include "Tree.h"
#include <utility>                   // for std::pair
#include <list>
#include <map>
#include "iTree.h"
#include <QMap>

#include "transformnode.h"
#include "csgops.h"
typedef stree<hnode> tree_hnode;

class streeConverter : public NodeVisitor {
public:
    streeConverter();
    streeConverter(Tree *tree);
    ~streeConverter();
    Response visit(State &state, const AbstractNode &node) override;
    Response visit(State &state, const RootNode &node) override;
    Response visit(State &state, const AbstractPolyNode &node) override;
    Response visit(State &state, const TransformNode &node) override;
    Response visit(State &state, const CsgOpNode &node) override;
    Response visit(State &state, const GroupNode &node) override;

    tree_hnode* convert_tree(Tree *tree);
    // int count_node();
private:
    typedef std::vector<hnode*> childNodeList;
    std::map<int, childNodeList> visitedchildren;
    void handleVisitedChildren(const State &state, const AbstractNode &node, hnode* hier_node);

    tree_hnode *m_Tree;

    void add_children(tree_hnode::iterator parent_node, childNodeList childs);
    
    // std::vector<hnode*> hnodes;
    // std::map<hnode*> hnodes;
    QMap<int, hnode*> hnodes;
    QMap<int, tree_hnode::iterator> hnode_iters;
    // std::vector<tree_hnode::iterator> node_iters;
}; 