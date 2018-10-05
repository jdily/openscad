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
#include "cgaladvnode.h"
#include "GeometryEvaluator.h"
#include "polyset.h"
#include "EditorLoc.h"
typedef stree<hnode*> tree_hnode;

class simpTreeConverter : public NodeVisitor {
public:
    simpTreeConverter();
    simpTreeConverter(Tree *tree);
    simpTreeConverter(Tree* tree, class GeometryEvaluator *geomevaluator = nullptr);
    ~simpTreeConverter();
    Response visit(State &state, const AbstractNode &node) override;
    Response visit(State &state, const RootNode &node) override;
    Response visit(State &state, const AbstractPolyNode &node) override;
    Response visit(State &state, const TransformNode &node) override;
    Response visit(State &state, const CsgOpNode &node) override;
    Response visit(State &state, const GroupNode &node) override;
    Response visit(State &state, const CgaladvNode &node) override;

    tree_hnode* convert_tree(Tree *tree);
private:
    typedef std::vector<hnode*> childNodeList;
    std::map<int, childNodeList> visitedchildren;
    void handleVisitedChildren(const State &state, const AbstractNode &node, hnode* hier_node);
    tree_hnode *m_Tree;
    void add_children(tree_hnode::iterator parent_node, childNodeList childs);
    int search_valid_parent(int id);
    
    // std::vector<hnode*> hnodes;
    // std::map<hnode*> hnodes;
    QMap<int, hnode*> hnodes;
    QMap<int, tree_hnode::iterator> hnode_iters;
    // std::vector<tree_hnode::iterator> node_iters;
    
    // class GeometryEvaluator *geomevaluator;
    std::map<int, shared_ptr<CSGLeaf> > tree_stored_leaf_term;
    bool has_csginfo;

    // record things for parenting..
    std::map<int, int> parent_db;
    GeometryEvaluator *m_geomeval;

    QMap<int, std::vector<int> > visitedchildrennode;
    // std::map<int, std::vector<const Geometry*> > visitedchildren;
    void addToParent(const State &state, const AbstractNode &node);
	void applyToChildren(State &state, const AbstractNode &node);

};