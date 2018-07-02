#include "FuncEstimator.h"
#include "polyset.h"
FuncEstimator::FuncEstimator() {}

FuncEstimator::FuncEstimator(tree_hnode* geom_tree) : m_tree(geom_tree) {}


FuncEstimator::~FuncEstimator() {}

QPair<Eigen::Vector3d, Eigen::Vector3d> FuncEstimator::find_axis() {
    // QLine axis;
    QList<Eigen::Vector3d> centroids;

    // check if the node exist..
    // int num_geom = m_tree->number_of_children();
    tree_hnode::iterator iterator;
    iterator = m_tree->begin();
    while (iterator != m_tree->end()) {
        std::string type = (*iterator)->type;
        if (type == "poly") {
    //         // get the bounding box of this geometry..
            PolySet* newps = static_cast<PolySet*>(const_cast<Geometry*>((*iterator)->geom.get()));
            BoundingBox bbox = newps->getBoundingBox();
            Eigen::Vector3d center = bbox.center();
            centroids.append(center);
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
    // // get the center of all these centroids;
    // float c_x = 0.0, c_y = 0.0, c_z = 0.0;
    // for (int j = 0; j < centroids.length(); j++) {
    //     c_x += centroids[j][0];        
    //     c_y += centroids[j][1];  
    //     c_z += centroids[j][2];
    // }
    // c_x = c_x / centroids.length();
    // c_y = c_y / centroids.length();
    // c_z = c_z / centroids.length();


    QPair<Eigen::Vector3d, Eigen::Vector3d> out;
    return out;
}