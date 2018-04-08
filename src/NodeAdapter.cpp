#include "NodeAdapter.h"
#include "state.h"
#include "module.h"
// #include "ModuleInstantiation.h"

#include <string>
#include <sstream>
#include <assert.h>
// 
// #include "primitives.cc"

Response NodeAdapter::visit(State &state, const AbstractNode &node) {
    // if (state.isPrefix()) {
    //     std::cout << "insert visit abstract node " << std::endl;
    //     // std::cout << "cur index to insert : " << cur_id << std::endl;   
    //     // in prefix stage, change the index of each node..
    //     // node.idx = cur_id;
    //     const_cast<AbstractNode &>(node).idx = cur_id;
    //     cur_id++;
    // }
    // handleIndent(state);
    // if (state.isPostfix()) {
    //     // in postfix stage, insert the node and children in the cache..
    //     std::stringstream dump;
    //     dump << this->currindent;
    //     if (this->idprefix) dump << "n" << node.index() << ":";
    //     dump << node;
    //     dump << insert_children_block(node);
    //     this->cache.insert(node, dump.str());
    // }
    // handleVisitedChildren(state, node);
    return Response::ContinueTraversal;
}


Response NodeAdapter::visit(State &state, const PrimitiveNode &node) {
    if (state.isPrefix()) {
        std::cout << "visit primtive name :" << node.name() << std::endl;
        // QString node_name = node.name();
        switch (node.type) {
            case primitive_type_e::CUBE:
                this->adapt_cube(node);
                std::cout << "x val after adapt" << node.x << std::endl;
                break;
            default:
                break;
        }
    }
    return Response::PruneTraversal;
}

Response NodeAdapter::visit(State &state, const TransformNode &node) {
    if (state.isPrefix()) {
        std::cout << "visit transformation name :" << node.name() << std::endl;
        QList<QString> keys = adapt_assignments.keys();
        for (int i = 0; i < keys.size(); i++) {
            if (keys[i] == QString("translate")) {
                Vector3d trans(adapt_assignments[QString("translate")].data());
                const_cast<TransformNode&>(node).matrix.translate(trans);
            } else if (keys[i] == "rotation") {
                // TODO : finish the rotation adaptation.
                // Vector3d rotate(adapt_assignments[QString("rotate")].data());     
                // const_cast<TransformNode&>(node).matrix.rotate(rotate);
            } else if (keys[i] == "scale") {
                Vector3d scale(adapt_assignments[QString("scale")].data());
                const_cast<TransformNode&>(node).matrix.scale(scale);
            }
        }
        // VectorXf aa;
        // // QString node_name = node.name();
        // switch (node.type) {
        //     case primitive_type_e::CUBE:
        //         this->adapt_cube(node);
        //         std::cout << "x val after adapt" << node.x << std::endl;
        //         break;
        //     default:
        //         break;
        // }
    }
    return Response::PruneTraversal;
}

// test to adapt the polygon : cube
void NodeAdapter::adapt_param(const AbstractNode& node, QString adapt_type, QMap<QString, VectorXd> info) {
    this->adapt_assignments = info;
    traverse(node);
}

void NodeAdapter::adapt_cube(const PrimitiveNode& node) {
    // center, x, y, z
    // QMap<QString, float>::iterator iter = adapt_assignments.begin();
    QList<QString> keys = adapt_assignments.keys();
    for (int i = 0; i < keys.size(); i++) {
        if (keys[i] == (QString("x"))) {
            std::cout << "adapt x" << std::endl;
            // const_cast<PrimitiveNode&>(node).x = adapt_assignments[QString("x")];
        } else if (keys[i] == (QString("y"))) {

        } else if (keys[i] == (QString("z"))) {

        } else if (keys[i] == (QString("center"))) {
            std::cout << "adapt center" << std::endl;
            const_cast<PrimitiveNode&>(node).center = true;
            // const_cast<PrimitiveNode&>(node).center = adapt_assignments[QString()];
        } else if (keys[i] == (QString("scale"))) {
            std::cout << "scale the cube...." << std::endl;
            Vector3d scale(adapt_assignments[QString("scale")].data());
            const_cast<PrimitiveNode&>(node).x *= scale(0);
            const_cast<PrimitiveNode&>(node).y *= scale(1);
            const_cast<PrimitiveNode&>(node).z *= scale(2);
        }
    }
    // while (iter != adapt_assignments.end()) {
    //     if (iter.key() == (QString("x"))) {
    //         std::cout << "adapt x" << std::endl;
    //         const_cast<PrimitiveNode&>(node).x = adapt_assignments[QString("x")];
    //     } else if (iter.key() == (QString("y"))) {

    //     } else if (iter.key() == (QString("z"))) {

    //     } else if (iter.key() == (QString("center"))) {
    //         std::cout << "adapt center" << std::endl;
    //         const_cast<PrimitiveNode&>(node).center = true;
    //         // const_cast<PrimitiveNode&>(node).center = adapt_assignments[QString()];
    //     }       
    // }
}


void NodeAdapter::handleVisitedChildren(const State &state, const AbstractNode &node) {

}
