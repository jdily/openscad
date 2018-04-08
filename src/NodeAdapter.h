#pragma once

#include <string>
#include <map>
#include <list>
#include "NodeVisitor.h"
#include "node.h"
// #include "nodecache.h"
#include "Tree.h"
#include <QMap>
#include "primitives.h"
#include "transformnode.h"
// class PrimitiveNode;

class NodeAdapter : public NodeVisitor {
public:
    NodeAdapter(Tree *tree) : m_pTree(tree), cache(tree->get_cache()) {

    }
    ~NodeAdapter() {}
    void adapt_param(const AbstractNode& node, QString adapt_type,  QMap<QString, VectorXd> info);
    void adapt_cube(const PrimitiveNode& node);

    Response visit(State &state, const AbstractNode &node);
    Response visit(State &state, const PrimitiveNode &node);
    Response visit(State &state, const TransformNode &node);
private:
    Tree *m_pTree;
    NodeCache &cache;
    typedef std::vector<const AbstractNode *> ChildList;
    std::map<int, ChildList> visitedchildren;

    QMap<QString, VectorXd> adapt_assignments;

    void handleVisitedChildren(const State &state, const AbstractNode &node);
};