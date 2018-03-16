#include "myVisitor.h"


// myVisitor::myVisitor()


shared_ptr<CSGNode> myVisitor::extractCSGTree(const AbstractNode &node) {
	this->traverse(node);
	// std::cout << "store size : " << this->stored_term.size() << std::endl;
	// this->stored_term[node.index()];
	// std::cout << "store size : " << this->stored_term.size() << std::endl;
	// if (this->stored_term[node.index()] == nullptr) {
	// 	std::cout << "NULL" << std::endl;
	// }

	shared_ptr<CSGNode> t(this->stored_term[node.index()]);
	if (!t) {
		std::cout << "t gg " << std::endl;
	}
	// const BoundingBox& bbox = t->getBoundingBox();
	// std::cout << t->getFlags() << std::endl;
	return this->rootNode = t;
}

Response myVisitor::visit(State &state, const AbstractNode &node)
{
	// if (state.isPostfix()) {
	// 	applyToChildren(state, node, OpenSCADOperator::UNION);
	// 	addToParent(state, node);
	// }
	return Response::ContinueTraversal;
}

const shared_ptr<CSGNode>& myVisitor::getRootNode() const {
	return this->rootNode;
}

// void myVisitor::A() {
//     std::cout << "A" << std::endl;
// } 