#include "FuncEstimator.h"
#include "polyset.h"
FuncEstimator::FuncEstimator() {}

FuncEstimator::FuncEstimator(tree_hnode* geom_tree) : m_tree(geom_tree) {
    selected_ids.clear();
}
FuncEstimator::FuncEstimator(tree_hnode* geom_tree, QList<int> ids) 
    : m_tree(geom_tree), selected_ids(ids) {}

FuncEstimator::~FuncEstimator() {}

QPair<Eigen::Vector3d, Eigen::Vector3d> FuncEstimator::find_cover_axis() {
    // QLine axis;
    QList<Eigen::Vector3d> centroids;
    // check if the node exist..
    // int num_geom = m_tree->number_of_children();
    tree_hnode::iterator iterator;
    iterator = m_tree->begin();

    
    if (selected_ids.empty()) {
        // go through all of them
        while (iterator != m_tree->end()) {
            std::string type = (*iterator)->type;
            if (type == "poly") {
                PolySet* newps = static_cast<PolySet*>(const_cast<Geometry*>((*iterator)->geom.get()));
                BoundingBox bbox = newps->getBoundingBox();
                Eigen::Vector3d center = bbox.center();
                centroids.append(center);
            }
        }
    } else {
        while (iterator != m_tree->end()) {
            int idx = (*iterator)->idx;
            if (selected_ids.contains(idx)) {
                std::string type = (*iterator)->type;
                if (type == "poly") {
                    PolySet* newps = static_cast<PolySet*>(const_cast<Geometry*>((*iterator)->geom.get()));
                    BoundingBox bbox = newps->getBoundingBox();
                    Eigen::Vector3d center = bbox.center();
                    centroids.append(center);
                }
            }
        }
    }
    Eigen::MatrixXd centers(centroids.length(), 3);
    for (int i = 0; i < centroids.length(); i++) {
        centers.row(i) = centroids[i].transpose();
    }
    Eigen::Vector3d origin = centers.colwise().mean();
    Eigen::MatrixXd centered = centers.rowwise() - origin.transpose();
	Eigen::MatrixXd cov = centered.adjoint() * centered;
	Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> eig(cov);
	Eigen::Vector3d axis = eig.eigenvectors().col(2).normalized();

    QPair<Eigen::Vector3d, Eigen::Vector3d> out(origin, axis);
    return out;
}