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
typedef stree<hnode*> tree_hnode;

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
    bool with_csginfo() {
        return has_csginfo;
    }    

private:
    typedef std::vector<hnode*> childNodeList;
    std::map<int, childNodeList> visitedchildren;
    void handleVisitedChildren(const State &state, const AbstractNode &node, hnode* hier_node);
    shared_ptr<CSGLeaf> convert_to_leaf(shared_ptr<CSGNode> input);


    tree_hnode *m_Tree;

    void add_children(tree_hnode::iterator parent_node, childNodeList childs);
    
    // std::vector<hnode*> hnodes;
    // std::map<hnode*> hnodes;
    QMap<int, hnode*> hnodes;
    QMap<int, tree_hnode::iterator> hnode_iters;
    // std::vector<tree_hnode::iterator> node_iters;
    
    // class GeometryEvaluator *geomevaluator;
    std::map<int, shared_ptr<CSGLeaf> > tree_stored_leaf_term;
    bool has_csginfo;
};  