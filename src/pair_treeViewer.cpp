#include "pair_treeViewer.h"

pair_treeViewer::pair_treeViewer(QWidget *parent) : QWidget(parent) {
    layout = new QHBoxLayout;
    viewer0.reset(new treeViewer(this));
    viewer1.reset(new treeViewer(this));
    layout->addWidget(viewer0.get());
    layout->addWidget(viewer1.get());
    this->setLayout(layout);
}

pair_treeViewer::~pair_treeViewer() {}


void pair_treeViewer::setSTree(tree_hnode *htree, int index) {
    std::cout << "index : " << index << std::endl;
    if (index == 0) {
        viewer0->setSTree(htree);
    } else {
        viewer1->setSTree(htree);
    }
}
