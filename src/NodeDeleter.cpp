#include "NodeDeleter.h"
Response NodeDeleter::visit(State &state, const AbstractNode &node) {
    if (state.isPrefix()) {
        std::cout << "deleter visit abstract node " << std::endl;
    }
    handleVisitedChildren(state, node);

    return Response::ContinueTraversal;
}


Response NodeDeleter::visit(State &state, const RootNode &node) {
    handleVisitedChildren(state, node);
    return Response::ContinueTraversal;
}

// remove the entire subtree rooted by this node
void NodeDeleter::remove_node(const AbstractNode &node, const AbstractNode &parent) {
    // first make sure the tree has this node..
    std::cout << "before remove, cache count : " << cache.count() << std::endl;
    if (cache.contains(node)) {
        std::cout << "let's remove this node " << node.index() << std::endl;
        // childs are removed during the traverse process
        traverse(node);
        // remove node itself　in cache 
        cache.remove(node);
        // and in parent child node..
        int node_ind = node.index();
        auto e = std::find_if(parent.children.begin(), parent.children.end(),
                    [node_ind](AbstractNode* i) { return i->index() == node_ind; });
        if (e != parent.children.end()) {
            const_cast<AbstractNode &>(parent).children.erase(e);
        }
        std::cout << "after remove, cache count : " << cache.count() << std::endl;
    }
}

void NodeDeleter::handleVisitedChildren(const State &state, const AbstractNode &node) {
    if (state.isPostfix()) {
        // perform the remove here????
        int child_count = this->visitedchildren[node.index()].size();
        std::cout << node.name() << std::endl;
        std::cout << "child count : " << child_count << " in handleVisitedChildren func." << std::endl;
        for (int i = 0; i < child_count; i++) {
            cache.remove(*this->visitedchildren[node.index()][i]);
            // TODO : come back to deal with this later.. about viz the update geometry...
            // also need to remove them from the children list...
            // find the index of this child first..
            int child_ind = this->visitedchildren[node.index()][i]->index();
            int child_at = 0;
            for (int j = 0; j < (int)node.children.size(); j++) {
                if (node.children[j]->index() == child_ind) {
                    child_at = j;
                    break;
                }
            }
            const_cast<AbstractNode &>(node).remove_child(child_at);
            // auto e = std::find(node.children.begin(), node.children.end(),
            //        [&obj](MyClass * i) { return i && (*i == obj); });

            // if (e != pVector.end()) {
            //     pVector.erase(e);
            // }
        }
        this->visitedchildren.erase(node.index());
		if (!state.parent()) {
			this->root = &node;
		} else {
			this->visitedchildren[state.parent()->index()].push_back(&node);
		}
    }
}