#include "geomTransferer.h"
#include <QFile>
#include <QTextStream>
#include "NodeDeleter.h"
#include "NodeInserter.h"

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

// 1. first version use the node id pair 
Tree* geomTransferer::transfer(int self_node_id, int exp_node_id) {
    std::cout << "do transfer from " << self_node_id << " to " << exp_node_id << std::endl;
    // Tree *out_tree = new Tree(m_pSelf->root());
    Tree *out_tree = new Tree(*m_pSelf);
    // std::cout << "original node count : " << m_pSelf->node_count() << std::endl;
    // std::cout << "out node count : " << out_tree->node_count() << std::endl;
    
    // test some tree manipulation.....
    // e.g. remove the self node...
    
    // std::cout << "original node count : " << m_pSelf->node_count() << std::endl;
    // std::cout << "root child count : " << m_pSelf->root()->children.size() << std::endl;
    // std::cout << "child 0 id : " << m_pSelf->root()->children[0]->idx << std::endl;
    // std::cout << "child 1 id : " << m_pSelf->root()->children[1]->idx << std::endl;

    // std::cout << "self tree contains ? " << m_pSelf->contains(m_pSelf->root()->children[0]) << std::endl;
    // // std::cout << "self tree contains ? " << m_pSelf->contains(m_pSelf->root()->children[1]->children[0]) << std::endl;
    // m_pSelf->remove_node(m_pSelf->root()->children[0]);
    // std::cout << "after node count : " << m_pSelf->node_count() << std::endl;
    // std::cout << "child 0 id : " << m_pSelf->root()->children[0]->idx << std::endl;
    // std::cout << "self tree still contains ? " << m_pSelf->contains(m_pSelf->root()->children[0]) << std::endl;
    // std::cout << "self tree still contains ? " << m_pSelf->contains(m_pSelf->root()->children[1]->children[0]) << std::endl;

    // Process : replace a subtree
    // 1. remove it from the original tree..
    // TODO : how to propagate this modification back to the rendered and text editor...
    NodeDeleter *deleter = new NodeDeleter(out_tree);
    deleter->remove_node(*out_tree->root()->children[0], *out_tree->root());
    std::cout << "remain node count : " << out_tree->get_cache().count() << std::endl;
    // 2. insert the desire subtree into the original tree
    // NodeInserter *inserter = new NodeInserter(m_pSelf);
    // TODO : deal with the index compatible prolbme, i.e. the index in two trees will not be sync
    // create a dummy node
    // PrimitiveNode *dummpy_poly = new PrimitiveNode(primitive_type_e::CUBE);
    // inserter->insert_node(*m_pSelf->root(), *m_pExample->root()->children[0]);
    // 3. adapt the geometry..


    

    return out_tree;
}

void geomTransferer::param_change(std::string target, int node_id, std::string property, float tar_val) {

}
