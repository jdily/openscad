#include "TreeSampler.h"
#include "polyset-utils.h"
#include "Camera.h"

TreeSampler::TreeSampler() {}
TreeSampler::TreeSampler(Tree *tree) {}

TreeSampler::TreeSampler(Tree* tree, class GeometryEvaluator *geomevaluator) 
    : m_geomeval(geomevaluator), m_tree(tree)
{
    sample_dict.clear();
}

TreeSampler::~TreeSampler() {

}

Response TreeSampler::visit(State &state, const RootNode &node) {
    if (state.isPostfix()) {
        std::cout << "apply to children of " << node.index() << std::endl;
        applyToChildren(state, node);
        // addToParent(state, node);
    }
    return Response::ContinueTraversal;
}

Response TreeSampler::visit(State &state, const AbstractNode &node) {
    // if (state.isPostfix()) {
    //     std::cout << "apply to children of " << node.index() << std::endl;
    //     applyToChildren(state, node);
    //     addToParent(state, node);
    // }
    return Response::ContinueTraversal;
}

Response TreeSampler::visit(State &state, const GroupNode &node) {
    if (state.isPostfix()) {
        applyToChildren(state, node);
        // TODO : debug here...
        addToParent(state, node);
    }
    return Response::ContinueTraversal;
}

Response TreeSampler::visit(State &state, const TransformNode &node) {
    if (state.isPrefix()) {
        state.setMatrix(state.matrix() * node.matrix);
    }
    if (state.isPostfix()) {
        // handle the visitedchildren -> apply the transformation on them.
        applyToChildren(state, node);
        addToParent(state, node);
    }
    return Response::ContinueTraversal;
}

// should we change to LeafNode????
Response TreeSampler::visit(State &state, const LeafNode &node) {
// Response TreeSampler::visit(State &state, const AbstractPolyNode &node) {
    if (state.isPostfix()) {
        if (this->m_geomeval) {
            std::cout << "poly node idx : " << node.idx << std::endl;
            // TODO : check the geom type? PolySet or Nef_Poly?
            auto geom = this->m_geomeval->evaluateGeometry(node, false);
            // TODO : sample points from the polyset geom??
            // TODO : check the type of the geom -> cast to const PolySet??
            // or do it in the random_sample function?
            std::vector<Vector3d> samples = PolysetUtils::random_sample((const_cast<Geometry*>(geom.get())));
            sample_dict.insert(node.idx, samples);
            // sample_dict.insert(std::make_pair(node.idx, samples));
            addToParent(state, node);
        }
    }
    return Response::ContinueTraversal;    
}

QMap<int, std::vector<Eigen::Vector3d>> TreeSampler::get_samples(const AbstractNode &node, QGLView* viewer) {
    this->traverse(node);
    std::cout << "finish traverse" << std::endl;
    if (viewer == nullptr) {
        return sample_dict; 
    } else {
        // TODO : do the projection on the 2D plane..
        std::cout << "there is viewer" << std::endl;
        proj_sample_dict.clear();
        // for (int i = 0; i < sample_dict.size(); i++) {
        for (int k : sample_dict.keys()) {
            // int k = sample_dict.keys()[k];
            auto proj_samples = viewer->project_samples(sample_dict[k], k);
            proj_sample_dict.insert(k, proj_samples);
        }
        return proj_sample_dict;
    }
}

void TreeSampler::addToParent(const State &state, const AbstractNode &node) {
        std::cout << "[addToParent]: add to parent for " << node.index() << " to " << state.parent()->index() << std::endl;
    if (this->visitedchildren.keys().contains(node.index())) {
        this->visitedchildren.remove(node.index());
    }
    if (state.parent()) {
        if (this->visitedchildren.keys().contains(state.parent()->index())) {
            this->visitedchildren[state.parent()->index()].push_back(node.index());
        } else {
            std::vector<int> childs;
            childs.push_back(node.index());
            this->visitedchildren.insert(state.parent()->index(), childs);
        }
        // this->visitedchildren[state.parent()->index()].push_back(node.index());
        std::cout << "[addToParent]: children length of " << state.parent()->index() << " : " << this->visitedchildren[state.parent()->index()].size() << std::endl;
    } else {

    }
}

void TreeSampler::applyToChildren(State &state, const AbstractNode &node) {
    int sample_count = 0;
    std::cout << "[applyToChildren]: apply to children of " << node.index() << std::endl;
    std::cout << "[applyToChildren]: children length of " << this->visitedchildren[node.index()].size() << std::endl;
    for(const int &chind : this->visitedchildren[node.index()]) {
        // check if chind is a key in sample_dict...
        if (sample_dict.keys().contains(chind)) {
            sample_count = sample_dict[chind].size();
            Transform3d trans = state.matrix();
            // chgeom->transform(state.matrix());
            // std::cout << sample_dict[chind][3][0] << " " << sample_dict[chind][3][1] << std::endl;
            for (int i = 0; i < sample_count; i++) {
                sample_dict[chind][i] = trans * sample_dict[chind][i];
            }
            // std::cout << sample_dict[chind][3][0] << " " << sample_dict[chind][3][1] << std::endl;
        }
    }
}