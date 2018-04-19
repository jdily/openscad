#pragma once

#include "iTree.h"
#include <QPair>
#include <Eigen/Dense>

#include "stree.h"
#include <QVector>
typedef stree<hnode> tree_hnode;
typedef tree_hnode::iterator node;
class commonSubtree {

public:
    commonSubtree(iTree *tree1, iTree *tree2);
    ~commonSubtree();
	void findMaxMatch();
	double distance();

private:
	commonSubtree findAnchoredMaxMatch(node n1, node n2);
	QVector<int> findBestAssignment(Eigen::MatrixXd S);
    iTree *tree1;
    iTree *tree2;
    QVector< QPair<node, node> > mapping;
	QVector< double > pairSimilarity;
    double similarity;
	double totalWeight;
};

class DistMeasure {
public:
    DistMeasure();
    ~DistMeasure();
    static commonSubtree between_trees(iTree *tree1, iTree *tree2);
    static double between_nodes(node n1, node n2);
private:
};
