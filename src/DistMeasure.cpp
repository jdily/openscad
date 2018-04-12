#include "DistMeasure.h"


commonSubtree::commonSubtree(iTree *tree1, iTree *tree2) {
    this->tree1 = tree1;
    this->tree2 = tree2;
    mapping.clear();
	similarity = 0;
	totalWeight = 0;
	node n1 = tree1->data->begin();
	while( n1 != tree1->data->end()) {
		totalWeight += pow(0.5, tree1->data->depth(n1));
		++n1;
	}

	node n2 = tree2->data->begin();
	while( n2 != tree2->data->end()) {
		totalWeight += pow(0.5, tree2->data->depth(n2));
		++n2;
	}
}

commonSubtree::~commonSubtree() {
    delete tree1;
    delete tree2;
}

commonSubtree commonSubtree::findAnchoredMaxMatch(node n1, node n2) {
    	// start searching
	commonSubtree subtree(tree1, tree2);
	// DistParameter distPara = (*node1)->scene->distPara;
	// DistMeasure dist(distPara);
	double weight = (pow(0.5, tree1->data->depth(n1)) + pow(0.5, tree2->data->depth(n2))) / 2.0;
	// subtree.pairSimilarity.push_back(1 - dist.betweenInteractions(*node1, *node2));
	// subtree.similarity = subtree.pairSimilarity.last()*weight;
	// subtree.mapping.push_back(QPair<node, node>(node1, node2));

	int m = n1.number_of_children();
	int n = n2.number_of_children();
	// if ( m != 0  && n != 0)
	// {
	// 	// find best matching for the children
	// 	Eigen::MatrixXd S = Eigen::MatrixXd::Zero(m, n);
	// 	QVector< QVector< QVector<QPair<node, node>> > >  M; // store all the mapping
	// 	QVector< QVector< QVector<double> > > PS;			 // store all the pair similarity
 
	// 	tree_inter::sibling_iterator child1 = node1.begin();
	// 	while ( child1 != node1.end() )
	// 	{
	// 		int i = tree1->index(child1);
	// 		QVector< QVector<QPair<node, node>> > M1;
	// 		QVector< QVector<double> > PS1;

	// 		tree_inter::sibling_iterator child2 = node2.begin();
	// 		while ( child2 != node2.end() )
	// 		{
	// 			int j = tree2->index(child2);
	// 			CommonSubtree temp = findAnchoredMaxMatch(child1, child2);		// recursively

	// 			S(i,j) = temp.similarity;
	// 			M1.push_back(temp.mapping);
	// 			PS1.push_back(temp.pairSimilarity);
	// 			child2++;
	// 		}

	// 		M.push_back(M1);
	// 		PS.push_back(PS1);
	// 		child1++;
	// 	}

	// 	QVector<int> assignment = findBestAssignment(S);

	// 	// add the matching
	// 	for (int i=0; i<assignment.size(); i++)
	// 	{
	// 		if (assignment[i] != -1)
	// 		{
	// 			subtree.mapping << M[i][assignment[i]];
	// 			subtree.pairSimilarity << PS[i][assignment[i]];
	// 			subtree.similarity += S(i,assignment[i]);
	// 		}
	// 	}
	// }

	return subtree;
}

std::vector<int> commonSubtree::findBestAssignment(Eigen::MatrixXd s) {

}

void commonSubtree::findMaxMatch() {
    node root1 = tree1->data->begin();
	node root2 = tree2->data->begin();
    node n1 = root1;
    while( n1 != tree1->data->end()) 
	{
		commonSubtree tmpSubtree = findAnchoredMaxMatch(n1, root2);
		if(tmpSubtree.similarity > similarity)
		{
			similarity = tmpSubtree.similarity;
			mapping = tmpSubtree.mapping;
			pairSimilarity = tmpSubtree.pairSimilarity;
		}
		++n1;
	}
	node n2 = root2;
	while( n2 != tree2->data->end()) 
	{
		commonSubtree tmpSubtree = findAnchoredMaxMatch(root1, n2);
		if(tmpSubtree.similarity > similarity)
		{
			similarity = tmpSubtree.similarity;
			mapping = tmpSubtree.mapping;
			pairSimilarity = tmpSubtree.pairSimilarity;
		}
		++n2;
	}
}
double commonSubtree::distance() {}


commonSubtree DistMeasure::between_trees(iTree *tree1, iTree *tree2) {
	int idx1 = -1;
	int idx2 = -1;
	double dist = 1;
    commonSubtree bestMatch(tree1, tree2);

}