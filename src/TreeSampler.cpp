#include "TreeSampler.h"
#include "polyset-utils.h"

TreeSampler::TreeSampler() {}
TreeSampler::TreeSampler(Tree *tree) {}

TreeSampler::TreeSampler(Tree* tree, class GeometryEvaluator *geomevaluator) 
    : m_geomeval(geomevaluator), m_tree(tree)
{

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
            // TODO : check the geom type? PolySet or Nef_Poly?
            auto geom = this->m_geomeval->evaluateGeometry(node, false);
            // TODO : sample points from the polyset geom??
            // TODO : check the type of the geom -> cast to const PolySet??
            // or do it in the random_sample function?
            
            std::vector<Vector3d> samples = PolysetUtils::random_sample(geom);
        }
    }
    return Response::ContinueTraversal;    
}

std::map<int, Eigen::Vector3d> TreeSampler::get_centroids(const AbstractNode &node) {
    this->traverse(node);

}