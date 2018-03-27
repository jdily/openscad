#include "NodeInserter.h"


Response NodeInserter::visit(State &state, const AbstractNode &node) {
    if (state.isPrefix()) {
        std::cout << "insert visit abstract node " << std::endl;
        std::cout << "cur index to insert : " << cur_id << std::endl;   
        // in prefix stage, change the index of each node..
        // node.idx = cur_id;
        const_cast<AbstractNode &>(node).idx = cur_id;
        cur_id++;
    }
    handleIndent(state);
    if (state.isPostfix()) {
        // in postfix stage, insert the node and children in the cache..
        std::stringstream dump;
        dump << this->currindent;
        if (this->idprefix) dump << "n" << node.index() << ":";
    }
    // handleVisitedChildren(state, node);
    return Response::ContinueTraversal;
}


// Response NodeInserter::visit(State &state, const RootNode &node) {
//     // handleVisitedChildren(state, node);
//     return Response::ContinueTraversal;
// }
void NodeInserter::handleIndent(const State &state) {
	if (state.isPrefix()) {
		this->currindent += "\t";
	}
	else if (state.isPostfix()) {
		this->currindent.erase((this->currindent.length() >= 1) ? 
													 this->currindent.length() - 1 : 0);
	}
}

void NodeInserter::insert_node(const AbstractNode &parent_node, const AbstractNode &insert_node) {
    std::cout << insert_node.index() << " " << insert_node.name() << std::endl;
    std::cout << "the insert node has " << (int)insert_node.children.size() << " child" << std::endl;

    // parent_node.children.push_back(&insert_node);
    // add insert_node and all the children into cache..
    // traverse(insert_node);

    const_cast<AbstractNode&>(parent_node).children.push_back(&const_cast<AbstractNode&>(insert_node));
}

void NodeInserter::insert_children(const AbstractNode &node) {

}

void NodeInserter::insert_children_block(const AbstractNode &node) {

    
}

void NodeInserter::handleVisitedChildren(const State &state, const AbstractNode &node) {
    if (state.isPostfix()) {
        // perform the remove here????
        // int child_count = this->visitedchildren[node.index()].size();
        // std::cout << node.name() << std::endl;
        // std::cout << "child count : " << child_count << " in handleVisitedChildren func." << std::endl;
        // for (int i = 0; i < child_count; i++) {
        //     cache.remove(*this->visitedchildren[node.index()][i]);
        // }
        this->visitedchildren.erase(node.index());
		if (!state.parent()) {
			// this->root = &node;
		} else {
			this->visitedchildren[state.parent()->index()].push_back(&node);
		}
    }
}