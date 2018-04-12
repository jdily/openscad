#pragma once

#include "iTree.h"
#include <QPair>
#include <Eigen/Dense>

#include "stree.h"


class commonSubtree {
typedef stree<hnode> tree_hnode;
typedef tree_hnode::iterator node;
public:
    commonSubtree(iTree *tree1, iTree *tree2);
    ~commonSubtree();
	void findMaxMatch();
	double distance();

private:
	commonSubtree findAnchoredMaxMatch(node n1, node n2);
	std::vector<int> findBestAssignment(Eigen::MatrixXd S);
    iTree *tree1;
    iTree *tree2;
    std::vector< QPair<node, node> > mapping;
	std::vector< double > pairSimilarity;
    double similarity;
	double totalWeight;
};

class DistMeasure {
public:
    DistMeasure();
    ~DistMeasure();
    static commonSubtree between_trees(iTree *tree1, iTree *tree2);
private:
};
