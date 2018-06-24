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
            sample_dict.insert(node.idx, samples);
            // sample_dict.insert(std::make_pair(node.idx, samples));
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
            auto proj_samples = viewer->project_samples(sample_dict[k]);
            proj_sample_dict.insert(k, proj_samples);
        }
        return proj_sample_dict;
    }
}

// std::map<int, std::