#include "iTree.h"
// #include "graphConverter.h"


// convert the Tree to bTree first, and use it as internal data.
iTree::iTree(Tree* _tree, QString tree_name) {
    // this->tree_name = tree_name;
    // graphConverter *converter = new graphConverter(_tree);

}
iTree::iTree(bTree* _tree, QString tree_name) {
    // copy constructor..
    // m_Tree = bTree(*_tree);
    // this->tree_name = tree_name;
}
iTree::~iTree() {}


void iTree::clear() {
    // m_Tree.clear();
}