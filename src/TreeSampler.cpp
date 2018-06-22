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

Response TreeSampler::visit(State &state, const AbstractNode &node) {
    if (state.isPostfix()) {
        
    }
    return Response::ContinueTraversal;
}

Response TreeSampler::visit(State &state, const AbstractPolyNode &node) {
    if (state.isPostfix()) {
        if (this->m_geomeval) {
            std::cout << "poly node idx : " << node.idx << std::endl;
            // TODO : check the geom type? PolySet or Nef_Poly?
            auto geom = this->m_geomeval->evaluateGeometry(node, false);
            // TODO : sample points from the polyset geom??
            // TODO : check the type of the geom -> cast to const PolySet??
            // or do it in the random_sample function?
            std::vector<Vector3d> samples = PolysetUtils::random_sample((const_cast<Geometry*>(geom.get())));
            // sample_dict.insert(std::make_pair(node.idx, samples));
        }
    }
    return Response::ContinueTraversal;    
}

std::map<int, std::vector<Eigen::Vector3d>> TreeSampler::get_samples(const AbstractNode &node, QGLView* cam) {
    this->traverse(node);
    std::cout << "finish traverse" << std::endl;

    if (cam == nullptr) {
        return sample_dict; 
    } else {
        // TODO : do the projection on the 2D plane..
        std::cout << "there is camera" << std::endl;
        return sample_dict;
    }
}

// std::map<int, std::