#pragma once

#include <QString>
#include "Tree.h"
#include "NodeVisitor.h"
#include <utility>                   // for std::pair
#include <algorithm>                 // for std::for_each
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/random_layout.hpp>
#include <boost/graph/topology.hpp>
#include <boost/random.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/detail/read_graphviz_new.hpp>
#include <boost/property_map/dynamic_property_map.hpp>
// #include  <libs/graph/src/read_graphviz_new.cpp>

#include "transformnode.h"
#include "csgops.h"
#include "stree.h"

enum NODETYPE { ROOT, GROUP, AB_POLY, TRANSFORMATION, CSGOPT };
typedef boost::square_topology<boost::mt19937>::point_type topo_point;

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
// using Topology = square_topology<boost::mt19937>;
// using Position = Topology::point_type;
// typedef boost::square_topology<>::point_type Point;
enum vertex_position_t { vertex_position };

// using vertex_t = graph_traits<bTree>::vertex_descriptor;
// using edge_t   = graph_traits<bTree>::edge_descriptor;

class graphConverter : public NodeVisitor {
public:
    graphConverter(Tree *tree);
    ~graphConverter();
    Response visit(State &state, const AbstractNode &node) override;
    Response visit(State &state, const RootNode &node) override;
    Response visit(State &state, const AbstractPolyNode &node) override;
    Response visit(State &state, const TransformNode &node) override;
    Response visit(State &state, const CsgOpNode &node) override;
    Response visit(State &state, const GroupNode &node) override;

    bTree convert_tree(Tree* tree, QString tree_name);
    std::vector<topo_point> make_layout(bTree* btree);

    // stree<AbstractNode*> convert_to_stree(Tree* tree, QString tree_name);
    int count_node();

private:
    // Tree* m_pTree;
    bTree m_BTree;

    // typedef std::list<const AbstractNode *> ChildList;
    // std::map<int, ChildList> visitedchildren;
    // void handleVisitedChildren(const State &state, const AbstractNode &node);
    typedef std::list<vertex_d> childNodeList;
    std::map<int, childNodeList> visitedchildren;
    void handleVisitedChildren(const State &state, const AbstractNode &node, vertex_d vert);

    void set_relation(const AbstractNode& node, vertex_d vert);
    void clean();
};