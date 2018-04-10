#pragma once

#include <QString>
#include <utility>
#include <algorithm>
#include "Tree.h"

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/random_layout.hpp>
#include <boost/graph/topology.hpp>
#include <boost/random.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/detail/read_graphviz_new.hpp>
#include <boost/property_map/dynamic_property_map.hpp>

typedef boost::square_topology<boost::mt19937>::point_type topo_point;

// class graphConverter;

struct Node {
    // NODETYPE type;
    std::string type;
    // std::string aux_name;
    int idx;
    // std::string height;
    // std::string label;
    // std::string vweight;
    float height;
    float width;
    std::string posstr;
    topo_point pos;
    // Node* parent;
    // std::vector<Node*> children;
};
struct Edge{
    // // parent
    // int src_idx;
    // // child
    // int tar_idx;
    // std::string label;
};

struct GraphProp {
    std::string bb;
};
typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS, Node, Edge > bTree;

typedef bTree::vertex_descriptor vertex_d;
typedef bTree::edge_descriptor edge_d;
typedef boost::property_map<bTree, topo_point Node::*>::type PositionMap;

// TODO : check how many other functions are needed????
// 
// 1. how to iterate sibling


class iTree {
public:
    iTree(Tree *tree, QString tree_name);
    iTree(bTree *_tree, QString tree_name);
    ~iTree();

    void clear();
private:
    // internal boost tree data structure.
    bTree m_Tree;
    QString tree_name;
};