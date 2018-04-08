#pragma once

#include <string>
#include <map>
#include <memory>
#include <list>
#include "NodeVisitor.h"
#include "node.h"
#include "nodecache.h"
#include "Tree.h"
#include <QMap>

class NodeRecoder : public NodeVisitor {
public:
    NodeRecoder(Tree *tree, QMap<std::string, const AbstractNode*>& aux_to_node) : m_pTree(tree), record(aux_to_node) {}
    ~NodeRecoder() {}

    QMap<std::string, const AbstractNode*> append_aux_name(std::string prefix, const AbstractNode& node);
    // void record_aux_to_map(QMap<std::string, const AbstractNode&> map, const AbstractNode& node);
    Response visit(State &state, const AbstractNode &node);
private:
    Tree *m_pTree;
    std::string cur_prefix;
    QMap<std::string, const AbstractNode*> record;
    typedef std::list<const AbstractNode *> ChildList;
    std::map<int, ChildList> visitedchildren;


    void handleVisitedChildren(const State &state, const AbstractNode &node);
    
};