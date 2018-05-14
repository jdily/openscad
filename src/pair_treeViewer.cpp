#include "pair_treeViewer.h"

pair_treeViewer::pair_treeViewer(QWidget *parent) : QWidget(parent) {
    layout = new QHBoxLayout;
    viewer0.reset(new treeViewer(this));
    viewer1.reset(new treeViewer(this));
    viewer0->setName(QString("viewer0"));
    viewer1->setName(QString("viewer1"));
    viewer0->setID(0);
    viewer1->setID(1);
    layout->addWidget(viewer0.get());
    layout->addWidget(viewer1.get());
    this->setLayout(layout);

    menu = new QMenu(this);
    act_est_cor = new QAction(tr("&Make correspondences."), this);
    menu->addAction(act_est_cor);


    connect(act_est_cor, SIGNAL(triggered()), this, SLOT(est_correspondences()));
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


void pair_treeViewer::mousePressEvent(QMouseEvent* e) {
    if (e->button() == Qt::RightButton) {
        menu->exec(e->globalPos());
    }
}

void pair_treeViewer::est_correspondences() {
    std::cout << "est corr" << std::endl;
    // go to two viewer -> collect each selected node ids..
    // make correspondece, first assume only 1 nid is selected.
    std::vector<int> snids0 = viewer0->selected_nids;
    std::vector<int> snids1 = viewer1->selected_nids; 
    correspondence corr;
    corr.add_corr(snids0[0], snids1[0]);
    corrs.push_back(corr);
    std::cout << "establish correspondence between " << snids0[0] << " and " << snids1[0] << std::endl;
    // clean the selelcted
    viewer0->clear_selection();
    viewer1->clear_selection();
    std::cout << "current count of correspondences : " << corrs.size() << std::endl;
    
}