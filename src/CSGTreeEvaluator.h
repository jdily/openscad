#pragma once

// #include <QVector>
#include <map>
#include <list>
#include <vector>
#include <cstddef>
#include "NodeVisitor.h"
#include "memory.h"
#include "csgnode.h"
// #include <QList>

class CSGTreeEvaluator : public NodeVisitor
{
public:
	CSGTreeEvaluator(const class Tree &tree, class GeometryEvaluator *geomevaluator = nullptr)
		: tree(tree), geomevaluator(geomevaluator) {
	}
  	~CSGTreeEvaluator() {}
	CSGTreeEvaluator(const CSGTreeEvaluator &evaluator);

  	Response visit(State &state, const class AbstractNode &node) override;
 	Response visit(State &state, const class AbstractIntersectionNode &node) override;
 	Response visit(State &state, const class AbstractPolyNode &node) override;
 	Response visit(State &state, const class CsgOpNode &node) override;
 	Response visit(State &state, const class TransformNode &node) override;
	Response visit(State &state, const class ColorNode &node) override;
 	Response visit(State &state, const class RenderNode &node) override;
 	Response visit(State &state, const class CgaladvNode &node) override;

	shared_ptr<class CSGNode> buildCSGTree(const AbstractNode &node);
	shared_ptr<class CSGNode> buildCSGTree_w_hb(const AbstractNode &node, std::vector<int> hids);
	const shared_ptr<CSGNode> &getRootNode() const {
		return this->rootNode;
	}
	shared_ptr<class CSGNode> buildCSGTree_sub(const AbstractNode &node, Transform3d update_trans);
	
	const std::vector<shared_ptr<CSGNode>> &getHighlightNodes() const {
		return this->highlightNodes;
	}
	const std::vector<shared_ptr<CSGNode>> &getBackgroundNodes() const {
		return this->backgroundNodes;
	}
	std::map<int, shared_ptr<CSGLeaf>> get_stored_leaf_term() {
		return this->stored_leaf_term;
	}
	shared_ptr<class CSGNode> update_transform(const AbstractNode &node, std::vector<int> ids, Transform3d update_trans);
	// ichao added : check the stored term after
	void check_stored_term();
private:
  	void addToParent(const State &state, const AbstractNode &node);
	void applyToChildren(State &state, const AbstractNode &node, OpenSCADOperator op);
	shared_ptr<CSGNode> evaluateCSGNodeFromGeometry(State &state, 
																									const shared_ptr<const class Geometry> &geom,
																									const class ModuleInstantiation *modinst, 
																									const AbstractNode &node);
	void applyBackgroundAndHighlight(State &state, const AbstractNode &node);

	const AbstractNode *root;
	typedef std::list<const AbstractNode *> ChildList;
	std::map<int, ChildList> visitedchildren;




protected:
	const Tree &tree;
	class GeometryEvaluator *geomevaluator;
	shared_ptr<CSGNode> rootNode;
	std::vector<shared_ptr<CSGNode>> highlightNodes;
	std::vector<shared_ptr<CSGNode>> backgroundNodes;
	std::map<int, shared_ptr<CSGNode>> stored_term; // The term evaluated from each node index
	// ichao added
	// GOAL : provide all evaluated geometry at any level....
	std::map<int, shared_ptr<CSGLeaf>> stored_leaf_term;
};