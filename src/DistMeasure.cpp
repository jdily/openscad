#include "DistMeasure.h"


commonSubTree::commonSubTree(iTree *tree1, iTree *tree2) {
    this->tree1 = tree1;
    this->tree2 = tree2;
}

commonSubTree::~commonSubTree() {
    delete tree1;
    delete tree2;
}

commonSubTree commonSubTree::findAnchoredMaxMatch(Node n1, Node n2) {

}

std::vector<int> commonSubTree::findBestAssignment(Eigen::MatrixXd s) {

}

void commonSubTree::findMaxMatch() {}
double commonSubTree::distance() {}


commonSubTree DistMeasure::between_trees(iTree *tree1, iTree *tree2) {

}