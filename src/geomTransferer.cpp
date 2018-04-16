#include "geomTransferer.h"
#include <QFile>
#include <QTextStream>
#include "NodeDeleter.h"
#include "NodeInserter.h"
#include "NodeAdapter.h"
#include "NodeRecorder.h"


geomTransferer::geomTransferer(Tree *ori) : m_pSelf(ori), m_pExample(nullptr) {}

geomTransferer::geomTransferer(Tree *ori, Tree *example) : m_pSelf(ori), m_pExample(example) {


}

geomTransferer::~geomTransferer() {

}

void geomTransferer::load_example_file(QString filename) {
    // QFile file(filename);
    // // load the file
    // if (file.exists()) {
    //     QTextStream 
    // }
}

void geomTransferer::add_example_tree(Tree* example) {
    m_pExample = example;
    // m_pExamples.append(example);
}

Tree* geomTransferer::transfer_cylinder() {
    std::cout << "transfer cylinder example" << std::endl;
    NodeRecoder *recorder = new NodeRecoder(m_pSelf, aux_to_node);
    Tree *out_tree = new Tree(*m_pSelf);
    aux_to_node = recorder->append_aux_name("src", *out_tree->root());
    aux_to_node = recorder->append_aux_name("exp", *m_pExample->root());
    // check the count of the map
    std::cout << "size of the recorder : " << aux_to_node.size() << std::endl;
    // test about the parant things.
    std::cout << "parent of first child : " << aux_to_node["src_2"]->parent->aux_name << std::endl;
    std::cout << "parent of src_3 : " << aux_to_node["src_3"]->parent->aux_name << std::endl;

    NodeDeleter *deleter = new NodeDeleter(out_tree);
    deleter->remove_node(*aux_to_node["src_2"], *out_tree->root());
    // deleter->remove_node(*out_tree->root()->children[0], *out_tree->root());
    NodeInserter *inserter = new NodeInserter(out_tree);
    inserter->insert_node(*out_tree->root(), *aux_to_node["exp_2"]);
    // inserter->insert_node(*out_tree->root(), *m_pExample->root()->children[0]);

    deleter->remove_node(*aux_to_node["src_4"], *out_tree->root());
    inserter->insert_node(*out_tree->root(), *aux_to_node["exp_7"]);

    return out_tree;
}

// 1. first version use the node id pair 
Tree* geomTransferer::transfer(int self_node_id, int exp_node_id) {
    std::cout << "do transfer from " << self_node_id << " to " << exp_node_id << std::endl;
    // debug here
    NodeRecoder *recorder = new NodeRecoder(m_pSelf, aux_to_node);
    Tree *out_tree = new Tree(*m_pSelf);
    aux_to_node = recorder->append_aux_name("src", *out_tree->root());
    aux_to_node = recorder->append_aux_name("exp", *m_pExample->root());
    std::cout << "finish record the data" << std::endl;
    // check the count of the map
    std::cout << "size of the recorder : " << aux_to_node.size() << std::endl;
    // src_3
    std::cout << aux_to_node[std::string("src_3")]->name() << std::endl;
    PrimitiveNode* src_3 = (class PrimitiveNode*)aux_to_node[std::string("src_3")];
    std::cout << src_3->x << " " << src_3->y << " " << src_3->z << std::endl;
    // exp_4
    std::cout << aux_to_node[std::string("exp_4")]->name() << std::endl;
    PrimitiveNode* exp_4 = (class PrimitiveNode*)aux_to_node[std::string("exp_4")];
    std::cout << exp_4->x << " " << exp_4->y << " " << exp_4->z << std::endl;
    PrimitiveNode* exp_5 = (class PrimitiveNode*)aux_to_node[std::string("exp_5")];
    PrimitiveNode* exp_6 = (class PrimitiveNode*)aux_to_node[std::string("exp_6")];
    // compute the scaling and xxx ratio between them...
    // TODO:different alignment
    VectorXd scale_src2tar(3);
    scale_src2tar(0) = src_3->x / exp_4->x;
    scale_src2tar(1) = src_3->y / exp_4->y;
    scale_src2tar(2) = src_3->z / exp_4->z;
    // std::cout << "scale :　" << scale_src2tar(0) << " " << scale_src2tar(1) << " " << scale_src2tar(2) << std::endl;

    // Process : replace a subtree (**box** in this example)
    // 1. remove it from the original tree..
    // TODO : how to propagate this modification back to the rendered and text editor...
    NodeDeleter *deleter = new NodeDeleter(out_tree);
    deleter->remove_node(*out_tree->root()->children[0], *out_tree->root());
    // // 2. insert the desire subtree into the original tree
    NodeInserter *inserter = new NodeInserter(out_tree);
    // // TODO : deal with the index compatible problem, i.e. the index in two trees will not be sync
    // // PrimitiveNode *dummpy_poly = new PrimitiveNode(primitive_type_e::CUBE);
    inserter->insert_node(*out_tree->root(), *m_pExample->root()->children[0]);

    // for another part of the geometry
    deleter->remove_node(*out_tree->root()->children[0], *out_tree->root());
    inserter->insert_node(*out_tree->root(), *m_pExample->root()->children[1]);
    
    // 3. adapt the geometry..
    // NodeAdapter *adapter = new NodeAdapter(out_tree);
    // QMap<QString, VectorXd> adapt_info;
    // adapt_info["scale"] = scale_src2tar;
    // adapter->adapt_param(*exp_4, QString("cube"), adapt_info);
    // adapter->adapt_param(*exp_6, QString("cube"), adapt_info);
    // // how to position the adapted geometry....
    // // TODO : https://trello.com/c/gTJFviAl
    // adapter->adapt_param(*exp_5, QString("trans"), adapt_info);
    
    // test for primitive node adaptation (cube)
    // adapt_info[QString("x")] = 10.0;
    // adapt_info[QString("center")] = 1;
    // adapter->adapt_param(*out_tree->root()->children[0]->children[0], QString("cube"), adapt_info);
    // PrimitiveNode* prim_node = (class PrimitiveNode*)m_pSelf->root()->children[0]->children[0];

    // test for transformation node adaptation (translate)
    // TransformNode* trans_node = (class TransformNode*)m_pSelf->root()->children[1];
    // VectorXd translate(3);
    // translate(0) = 0.0; translate(1) = 0.0; translate(2) = 2.0;
    // adapt_info["translate"] = translate;
    // std::cout << "before adapt transformation" << std::endl;
    // adapter->adapt_param(*trans_node, QString("transform"), adapt_info);

    // std::cout << m_pSelf->root()->children[0]->children[0]->name() << std::endl;
    // PrimitiveNode* tar_node = (class PrimitiveNode*)m_pSelf->root()->children[0]->children[0];
    // std::cout << tar_node->name() << std::endl;
    // adapter->adapt_param(*tar_node, QString("cube"), adapt_info);
    // std::cout << "the x value : " << out_tree->root()->children[0]->children[0]->x << std::endl;
    return out_tree;
}

void geomTransferer::param_change(std::string target, int node_id, std::string property, float tar_val) {

}
