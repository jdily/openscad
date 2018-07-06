#include "simpTreeConverter.h"
#include <boost/type_index.hpp>
#include <typeinfo>


simpTreeConverter::simpTreeConverter() {
    this->m_Tree = new tree_hnode();
    this->has_csginfo = false;
}

simpTreeConverter::simpTreeConverter(Tree *tree) {
    this->m_Tree = new tree_hnode();
    this->has_csginfo = false;
}

simpTreeConverter::simpTreeConverter(Tree *tree, class GeometryEvaluator *geomevaluator) : m_geomeval(geomevaluator) {
    this->m_Tree = new tree_hnode();
    this->has_csginfo = false;
}

simpTreeConverter::~simpTreeConverter() {}

Response simpTreeConverter::visit(State &state, const AbstractNode &node) {
    return Response::ContinueTraversal;
}

Response simpTreeConverter::visit(State &state, const RootNode &node) {
    hnode* _node = new hnode();
    if (state.isPrefix()) {
        parent_db[node.index()] = -1;
    }
    if (state.isPostfix()) {
        _node->type = "root";
        _node->idx = node.index();
        _node->node = &node;
        if (this->has_csginfo) {
            _node->csgnode = this->tree_stored_leaf_term[node.index()];
            _node->obj_filename = QString("%1.obj").arg(node.index()).toStdString();
        }
        // _node.parent_idx = -1;
        // // hnodes[node.index()] = _node;
        hnodes.insert(node.index(), _node);
        for (auto child : visitedchildren[node.index()]) {
            child->parent_idx = node.index();
        }
    }
    handleVisitedChildren(state, node, _node);
    return Response::ContinueTraversal;
}

Response simpTreeConverter::visit(State &state, const TransformNode &node) {
    hnode* _node = new hnode();
    if (state.isPrefix()) {
        parent_db[node.index()] = state.parent()->index();
        state.setMatrix(state.matrix() * node.matrix);
    }
    if (state.isPostfix()) {
        _node->type = "trans";
        _node->idx = node.index();
        _node->node = &node;
        if (this->has_csginfo) {
            _node->csgnode = this->tree_stored_leaf_term[node.index()];
            _node->obj_filename = QString("%1.obj").arg(node.index()).toStdString();
        }
        _node->centroid = Eigen::Vector3d::Zero();
        hnodes.insert(node.index(), _node);
        for (auto child : visitedchildren[node.index()]) {
            child->parent_idx = node.index();
        }
        applyToChildren(state, node);
    }
    handleVisitedChildren(state, node, _node);
    return Response::ContinueTraversal;
}

Response simpTreeConverter::visit(State &state, const AbstractPolyNode &node) {
    hnode* _node = new hnode();
    if (state.isPrefix()) {
        parent_db[node.index()] = state.parent()->index();
    }
    if (state.isPostfix()) {
        _node->type = "poly";
        _node->idx = node.index();
        _node->node = &node;
        if (this->has_csginfo) {
            _node->csgnode = this->tree_stored_leaf_term[node.index()];
            _node->obj_filename = QString("%1.obj").arg(node.index()).toStdString();
        }
        hnodes.insert(node.index(), _node);
        for (auto child : visitedchildren[node.index()]) {
            child->parent_idx = node.index();
        }
        // do the geom eval
        if (this->m_geomeval) {
            std::cout << "do the geom eval for node idx : " << node.idx << std::endl;
            auto geom = this->m_geomeval->evaluateGeometry(node, false);
            // let's transform them...
            _node->geom = geom;
        }
    }
    handleVisitedChildren(state, node, _node);
    return Response::ContinueTraversal;
}

Response simpTreeConverter::visit(State &state, const CsgOpNode &node) {
    hnode* _node = new hnode();
    if (state.isPrefix()) {
        parent_db[node.index()] = state.parent()->index();
    }
    if (state.isPostfix()) {
        _node->type = "csg_opt";
        _node->idx = node.index();
        _node->node = &node;
        if (this->has_csginfo) {
            _node->csgnode = this->tree_stored_leaf_term[node.index()];
            _node->obj_filename = QString("%1.obj").arg(node.index()).toStdString();
        }
        hnodes.insert(node.index(), _node);
        for (auto child : visitedchildren[node.index()]) {
            child->parent_idx = node.index();
        }
    }
    handleVisitedChildren(state, node, _node);
    return Response::ContinueTraversal;
}

Response simpTreeConverter::visit(State &state, const CgaladvNode &node) {
    hnode* _node = new hnode();
    if (state.isPrefix()) {
        parent_db[node.index()] = state.parent()->index();
    }
    if (state.isPostfix()) {
        _node->type = "cgal_adv";
        _node->idx = node.index();
        _node->node = &node;
        // _node->centroi
        if (this->has_csginfo) {
            _node->csgnode = this->tree_stored_leaf_term[node.index()];
            _node->obj_filename = QString("%1.obj").arg(node.index()).toStdString();
        }
        hnodes.insert(node.index(), _node);
        for (auto child : visitedchildren[node.index()]) {
            child->parent_idx = node.index();
        }
        applyToChildren(state, node);
    }
    handleVisitedChildren(state, node, _node);
    return Response::ContinueTraversal;
}

Response simpTreeConverter::visit(State &state, const GroupNode &node) {
    hnode* _node = new hnode();
    if (state.isPrefix()) {
        parent_db[node.index()] = state.parent()->index();
    }
    if (state.isPostfix()) {
        _node->type = "group";
        _node->idx = node.index();
        _node->node = &node;
        // can we convert them all to Leaf node..
        if (this->has_csginfo) {
            _node->csgnode = this->tree_stored_leaf_term[node.index()];
            _node->obj_filename = QString("%1.obj").arg(node.index()).toStdString();
        }
        hnodes.insert(node.index(), _node);
        for (auto child : visitedchildren[node.index()]) {
            child->parent_idx = node.index();
        }
        applyToChildren(state, node);
    }
    handleVisitedChildren(state, node, _node);
    return Response::ContinueTraversal;
}

void simpTreeConverter::handleVisitedChildren(const State &state, const AbstractNode &node, hnode* hier_node) {
    if (state.isPostfix()) {
        // this->visitedchildren.erase(node.index());
        if (!state.parent()) {
            
        } else {
            this->visitedchildren[state.parent()->index()].push_back(hier_node);
            // hier_node->parent_idx = state.parent()->index();
        }
    }
}

void simpTreeConverter::applyToChildren(State &state, const AbstractNode &node) {
    int sample_count = 0;
    std::cout << "[applyToChildren]: apply to children of " << node.index() << std::endl;
    std::cout << "[applyToChildren]: children length of " << this->visitedchildren[node.index()].size() << std::endl;
    for(hnode* n: this->visitedchildren[node.index()]) {
        std::cout << "child index : " << n->idx << " " << n->type << std::endl;
        if (n->type == "poly") {
            if (hnodes.keys().contains(n->idx)) {
                std::cout << "contains " << n->idx << std::endl;
                Transform3d trans = state.matrix();
                Eigen::Vector3d translation = trans.translation();
                std::cout << "translation : " << translation[0] << " " << translation[1] << " " << translation[2] << std::endl;;
                // std::cout << trans << std::endl;
                // get the current centroid.
                PolySet* newps = static_cast<PolySet*>(const_cast<Geometry*>(n->geom.get()));
                BoundingBox bbox = newps->getBoundingBox();
                Eigen::Vector3d center = bbox.center();
                // std::cout << "center : " << center[0] << " " << center[1] << " " << center[2] << " " << std::endl;
                Eigen::Vector3d trans_center = trans * center;
                n->centroid = trans_center;
                n->transform = trans;
                // n->centroid = center;
                // todo
            }
        }
    }
}


void simpTreeConverter::add_children(tree_hnode::iterator parent_node, childNodeList childs) {
    // for (auto child : childs) {
    //     // m_Tree->append_child(parent_node, *child);
    // }
}

int simpTreeConverter::search_valid_parent(int id) {
    std::cout << "search for valid parent.." << std::endl;
    bool found = false;
    int parent_id = -1;
    int cur_id = id;
    std::cout << "cur_id : " << cur_id << std::endl;
    while (!found) {
        parent_id = parent_db[cur_id];
        if (parent_id == -1) {
            return parent_id;
        }
        if (hnodes[parent_id]->type == "trans" || hnodes[parent_id]->type == "group") {
            cur_id = parent_id;
        } else {
            return parent_id;
        }
    }
    return -1;
}

tree_hnode* simpTreeConverter::convert_tree(Tree *tree) {
    // TODO : test if we can obtain the right stored information here...
    this->tree_stored_leaf_term = tree->csg_stored_leaf_term;
    // std::cout << "tree stored term size : " << this->tree_stored_term.size() << std::endl;
    if (this->tree_stored_leaf_term.size() != 0) {
        this->has_csginfo = true;
    }
    traverse(*tree->root());
    QList<int> keys = hnodes.keys();
    for (int k : keys) {
        tree_hnode::iterator newNodeIter;
        if (hnodes[k]->type == "trans" || hnodes[k]->type == "group") {
            continue;
        }
        // int parent_id = hnodes[k]->parent_idx;
        int parent_id = search_valid_parent(k);
        // std::cout << k << " parent id : " << parent_id << std::endl;
        // change the parent information.
        hnodes[k]->parent_idx = parent_id;
        std::cout << k << " " << hnodes[k]->centroid[0] << " " << hnodes[k]->centroid[1] << " " << hnodes[k]->centroid[2] << std::endl;
        // std::cout << hnodes[k]->type << std::endl;
        if (parent_id == -1) {
            newNodeIter = m_Tree->begin();
            newNodeIter = m_Tree->insert(newNodeIter, hnodes[k]);
        } else {
            // check the parent type
            newNodeIter = m_Tree->append_child(hnode_iters[parent_id], hnodes[k]);
        }
        hnode_iters.insert(k, newNodeIter);
    }

    return m_Tree;
}



void simpTreeConverter::addToParent(const State &state, const AbstractNode &node) {
    std::cout << "[addToParent]: add to parent for " << node.index() << " to " << state.parent()->index() << std::endl;
    // if (this->visitedchildren.keys().contains(node.index())) {
    //     this->visitedchildren.remove(node.index());
    // }
    // if (state.parent()) {
    //     if (this->visitedchildren.keys().contains(state.parent()->index())) {
    //         this->visitedchildren[state.parent()->index()].push_back(node.index());
    //     } else {
    //         std::vector<int> childs;
    //         childs.push_back(node.index());
    //         this->visitedchildren.insert(state.parent()->index(), childs);
    //     }
    //     std::cout << "[addToParent]: children length of " << state.parent()->index() << " : " << this->visitedchildren[state.parent()->index()].size() << std::endl;
    // } else {

    // }
}

