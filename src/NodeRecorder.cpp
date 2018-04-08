#include "NodeRecorder.h"
#include <QString>

Response NodeRecoder::visit(State &state, const AbstractNode &node) {
    // if (state.isPrefix()) {
        // QString aux = QString("%1_%2").arg(QString(cur_prefix.c_str())).arg(QString::number(node.index()));
        // const_cast<AbstractNode&>(node).aux_name = aux.toStdString();
        // record.insert(aux.toStdString(), &node);
    // }
    // add parent information
    if (state.isPostfix()) {
        QString aux = QString("%1_%2").arg(QString(cur_prefix.c_str())).arg(QString::number(node.index()));
        const_cast<AbstractNode&>(node).aux_name = aux.toStdString();
        record.insert(aux.toStdString(), &node);
        // record the parent information as well
        for (auto child : this->visitedchildren[node.index()]) {
            const_cast<AbstractNode*>(child)->parent = &const_cast<AbstractNode&>(node);
        }
    }
    handleVisitedChildren(state, node);
    return Response::ContinueTraversal;
}

// the prefix usually the "src" or "tar_example_n"
QMap<std::string, const AbstractNode*> NodeRecoder::append_aux_name(std::string prefix, const AbstractNode& node) {
    cur_prefix = prefix;
    traverse(node);
    return record;
}

// void record_aux_to_map(QMap<std::string, const AbstractNode&> map, const AbstractNode& node) {

// }
void NodeRecoder::handleVisitedChildren(const State &state, const AbstractNode &node) {
    	if (state.isPostfix()) {
		this->visitedchildren.erase(node.index());
		if (!state.parent()) {
			// this->root = &node;
		}
		else {
			this->visitedchildren[state.parent()->index()].push_back(&node);
		}
	}
}