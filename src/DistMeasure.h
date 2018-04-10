#pragma once

#include "iTree.h"
#include <QPair>
#include <Eigen/Dense>

#include "stree.h"

class commonSubTree {
public:
    commonSubTree(iTree *tree1, iTree *tree2);
    ~commonSubTree();
	void findMaxMatch();
	double distance();

private:
	commonSubTree findAnchoredMaxMatch(Node n1, Node n2);
	std::vector<int> findBestAssignment(Eigen::MatrixXd S);
    iTree *tree1;
    iTree *tree2;
    std::vector< QPair<Node, Node> > mapping;
	std::vector< double > pairSimilarity;
};

class DistMeasure {
public:
    DistMeasure();
    ~DistMeasure();

    static commonSubTree between_trees(iTree *tree1, iTree *tree2);
private:
};
