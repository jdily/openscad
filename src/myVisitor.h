#pragma once

#include "NodeVisitor.h"
#include <map>
#include <vector>
#include "csgnode.h"
#include "Tree.h"


class myVisitor : public NodeVisitor {
public:
    myVisitor(Tree *tree, class GeometryEvaluator *geomevaluator = nullptr) 
        : m_pTree(tree), geomevaluator(geomevaluator) {
            this->rootNode = nullptr;
        }
    ~myVisitor() {}
    Response visit(State &state, const class AbstractNode &node) override;
    const shared_ptr<CSGNode> &getRootNode() const; 
    // void A(); 
    shared_ptr<CSGNode> extractCSGTree(const AbstractNode &node);
private:
    const Tree* m_pTree;
	class GeometryEvaluator *geomevaluator;
    const AbstractNode *root;
    shared_ptr<CSGNode> rootNode;
    std::map<int, shared_ptr<CSGNode>> stored_term; // The term evaluated from each node index

};