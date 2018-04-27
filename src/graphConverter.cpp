#include "graphConverter.h"
#include <boost/algorithm/string.hpp>

// #include <boost/type_index.hpp>
#include <typeinfo>

graphConverter::graphConverter(Tree *tree) {
    // m_pBTree = new bTree;
}

graphConverter::~graphConverter() {}

Response graphConverter::visit(State &state, const AbstractNode &node) {
    std::cout << "type id test : " << typeid(node).name() << std::endl;
    return Response::ContinueTraversal;
}

Response graphConverter::visit(State &state, const RootNode &node) {
    std::cout << "type id test : " << typeid(node).name() << std::endl;
    vertex_d root;
    if (state.isPostfix()) {
        // vertex_d root;
        root = add_vertex(m_BTree);
        m_BTree[root].type = "root";
        m_BTree[root].idx = node.index();
        // m_BTree[root].aux_name = node.aux_name;
        // add the child and set the parent things.
        set_relation(node, root);
    }
    handleVisitedChildren(state, node, root);
    return Response::ContinueTraversal;
}

Response graphConverter::visit(State &state, const TransformNode &node) {
    std::cout << "type id test : " << typeid(node).name() << std::endl;
    vertex_d trans;
    if (state.isPostfix()) {
        // vertex_d trans;
        trans = add_vertex(m_BTree);
        m_BTree[trans].type = "trans";
        m_BTree[trans].idx = node.index();
        // m_BTree[trans].aux_name = node.aux_name;
        set_relation(node, trans);
    }
    handleVisitedChildren(state, node, trans);
    return Response::ContinueTraversal;
}
Response graphConverter::visit(State &state, const CsgOpNode &node) {
    std::cout << "type id test : " << typeid(node).name() << std::endl;
    vertex_d csgopt;
    if (state.isPostfix()) {
        csgopt = add_vertex(m_BTree);
        m_BTree[csgopt].type = "csg_opt";
        m_BTree[csgopt].idx = node.index();
        // m_BTree[csgopt].aux_name = node.aux_name;
        set_relation(node, csgopt);
    }
    handleVisitedChildren(state, node, csgopt);
    return Response::ContinueTraversal;
}
Response graphConverter::visit(State &state, const GroupNode &node) {
    std::cout << "type id test : " << typeid(node).name() << std::endl;
    vertex_d group;
    if (state.isPostfix()) {
        group = add_vertex(m_BTree);
        m_BTree[group].type = "group";
        m_BTree[group].idx = node.index();
        // m_BTree[group].aux_name = node.aux_name;
        set_relation(node, group);
    }
    handleVisitedChildren(state, node, group);
    return Response::ContinueTraversal;
}
Response graphConverter::visit(State &state, const AbstractPolyNode &node) {
    std::cout << "type id test : " << typeid(node).name() << std::endl;
    vertex_d abpoly;
    if (state.isPostfix()) {
        abpoly = add_vertex(m_BTree);
        m_BTree[abpoly].type = "poly";
        m_BTree[abpoly].idx = node.index();
        // m_BTree[abpoly].aux_name = node.aux_name;
        set_relation(node, abpoly);
    }
    handleVisitedChildren(state, node, abpoly);
    return Response::ContinueTraversal;
}

void graphConverter::set_relation(const AbstractNode& node, vertex_d vert) {
    // std::cout << "type id test : " << typeid(node).name() << std::endl;
    for (auto child : this->visitedchildren[node.index()]) {
        add_edge(vert, child, m_BTree);
    }
}

bTree graphConverter::convert_tree(Tree* tree, QString tree_filename) {
    traverse(*tree->root());
    // export to dot file for GraphViz...
    QString dot_filename = QString("../../data/graph_viz/%1.dot").arg(tree_filename);
    // QString dot_filename = QString("/mnt/c/Users/jdily/Desktop/project/ddCAD/data/graph_viz/%1.dot").arg(tree_filename);
    std::cout << "dot_filename : " << dot_filename.toStdString() << std::endl;  
    // std::string dot_filename("/mnt/c/Users/jdily/Desktop/project/ddCAD/data/graph_viz/two_cylinder.dot");
    std::ofstream dotfile(dot_filename.toUtf8());
    boost::dynamic_properties dp;
    dp.property("node_id", get(&Node::idx, m_BTree));
    dp.property("type", get(&Node::type, m_BTree));
    // // dp.property("aux_name", get(&Node::aux_name, m_BTree));
    // // dp.property("type", get(&Node::type, m_BTree));
    // // dp.property("aux_name", get(&Node::aux_name, m_BTree));
    boost::write_graphviz_dp(dotfile, m_BTree, dp);

    QString layout_dot_filename = QString("../../data/graph_viz/%1_layout.dot").arg(tree_filename);
    // QString layout_dot_filename = QString("/mnt/c/Users/jdily/Desktop/project/ddCAD/data/graph_viz/%1_layout.dot").arg(tree_filename);
    QString cmd = QString("dot -Tdot %1 -o %2").arg(dot_filename).arg(layout_dot_filename);
    std::system(cmd.toUtf8());
    // boost::write_graphviz(dotfile, m_BTree, 
                        //   boost::make_label_writer(boost::get(&Node::type, m_BTree)));
    // load dot
    // std::string in_dot_filename("/mnt/c/Users/jdily/Desktop/project/ddCAD/data/graph_viz/two_cylinder_ref2.dot");
    // std::ifstream dot(in_dot_filename.c_str());
    std::ifstream dot(layout_dot_filename.toUtf8());
    bTree new_tree(0);
    boost::dynamic_properties dp_read(boost::ignore_other_properties);
    // boost::dynamic_properties dp_read;
    // boost::ref_property_map<bTree*,std::string>;
    dp_read.property("node_id",boost::get(&Node::idx, new_tree));//暗黙的なnode_idをvertex_node_idに割り当て
    dp_read.property("height",boost::get(&Node::height, new_tree));//自分で追加した属性
    dp_read.property("width", boost::get(&Node::width, new_tree));
    dp_read.property("type", boost::get(&Node::type, new_tree));
    dp_read.property("pos", boost::get(&Node::posstr, new_tree));
    // dp_read.property("bb", boost::get(&GraphProp::bb, new_tree));
    // TODO : check if the id is read in, and the type
    boost::read_graphviz(dot, new_tree, dp_read);
    std::string delimiter = ",";
    auto vs = boost::vertices(new_tree);
    for (auto vit = vs.first; vit != vs.second; ++vit) {
        std::vector<std::string> poses;
        // std::cout << (new_tree)[*vit].idx << " " << (new_tree)[*vit].width << " " << (new_tree)[*vit].height << " " << (new_tree)[*vit].posstr << std::endl;
        boost::split(poses,(new_tree)[*vit].posstr,boost::is_any_of(","));
        (new_tree)[*vit].pos[0] = std::strtof(poses[0].c_str(),0);
        (new_tree)[*vit].pos[1] = std::strtof(poses[1].c_str(),0);
        // std::cout << (new_tree)[*vit].posstr << " " << (new_tree)[*vit].pos[0] << " " << (new_tree)[*vit].pos[1] << std::endl;
    }
    clean();
    return new_tree;
    // return m_BTree;
}

std::vector<topo_point> graphConverter::make_layout(bTree* btree) {
    std::vector<topo_point> positions(num_vertices(m_BTree));
    boost::square_topology<boost::mt19937> topology;
    random_graph_layout(m_BTree, 
                make_iterator_property_map(positions.begin(), boost::identity_property_map{}),
                topology);
    // assign position to Node content
    auto vs = boost::vertices(m_BTree);
    // for (auto vit = vs.first; vit != vs.second; ++vit) {
    //     (*btree)[*vit].pos = positions[(*btree)[*vit].idx];
    // }
    for (int i = 0; i < positions.size(); ++i) {
        std::cout << positions[i][0] << " " << positions[i][1] << std::endl;
    }
    return positions;
}   

int graphConverter::count_node() {
    return num_vertices(m_BTree);
}

void graphConverter::clean() {
    this->visitedchildren.clear();
    m_BTree.clear();
}

// void graphConverter::handleVisitedChildren(const State &state, const AbstractNode &node) {
//     	if (state.isPostfix()) {
// 		this->visitedchildren.erase(node.index());
// 		if (!state.parent()) {
// 			// this->root = &node;
// 		}
// 		else {
// 			this->visitedchildren[state.parent()->index()].push_back(&node);
// 		}
// 	}
// }

void graphConverter::handleVisitedChildren(const State &state, const AbstractNode &node, vertex_d vert) {
    if (state.isPostfix()) {
		this->visitedchildren.erase(node.index());
		if (!state.parent()) {
			// this->root = &node;
		}
		else {
			this->visitedchildren[state.parent()->index()].push_back(vert);
		}
	}    
}