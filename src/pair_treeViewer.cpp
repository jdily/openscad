#include "pair_treeViewer.h"

pair_treeViewer::pair_treeViewer(QWidget *parent) : QWidget(parent) {
    layout = new QHBoxLayout;
    grid_layout = new QGridLayout;
    viewer0.reset(new treeViewer(this));
    viewer1.reset(new treeViewer(this));
    viewer0->setName(QString("viewer0"));
    viewer1->setName(QString("viewer1"));
    viewer0->setID(0);
    viewer1->setID(1);
    // layout->addWidget(viewer0.get());
    // layout->addWidget(viewer1.get());
    // this->setLayout(layout);

    // change to grid layout
    grid_layout->addWidget(viewer0.get(), 0, 0, 1, 1);
    grid_layout->addWidget(viewer1.get(), 0, 1, 1, 1);

    corr_list = new QTableWidget(this);
    corr_list->setColumnCount(2);
    QStringList colHeaders;
    colHeaders.push_back(QString("src"));
    colHeaders.push_back(QString("example"));
    corr_list->setHorizontalHeaderLabels(colHeaders);
    corr_list->horizontalHeader()->setStretchLastSection(true);

    relation_list = new QTableWidget(this);
    relation_list->setColumnCount(3);
    colHeaders.clear();
    colHeaders.push_back(QString("first"));
    colHeaders.push_back(QString("second"));
    colHeaders.push_back(QString("relationship"));
    relation_list->setHorizontalHeaderLabels(colHeaders);
    relation_list->horizontalHeader()->setStretchLastSection(true);
    // info_groupbox = new QGroupBox(tr(""))
    // QString str = tablist->horizontalHeaderItem(1)->text();
    // std::cout << "test column header : " << str.toStdString() << std::endl;
    grid_layout->addWidget(corr_list, 1, 0, 1, 1);
    // TODO : add buttons..
    // but_group = new QButtonGroup(this);
    cor_button = new QPushButton("Correspondence", this);
    connect(cor_button, SIGNAL(clicked()), this, SLOT(est_correspondences()));
    trans_button = new QPushButton("Transfer", this);
    relation_button = new QPushButton("Relation", this);
    connect(relation_button, SIGNAL(clicked()), this, SLOT(est_relations()));

    but_layout = new QBoxLayout(QBoxLayout::TopToBottom, this);
    but_layout->addWidget(relation_list);
    but_layout->addWidget(cor_button);
    but_layout->addWidget(trans_button);
    but_layout->addWidget(relation_button);
    but_groupbox = new QGroupBox(tr("Manipulation buttons"));
    but_groupbox->setLayout(but_layout);
    
    grid_layout->addWidget(but_groupbox, 1, 1, 1, 1);
    // grid_layout->addWidget(cor_button, 1, 1, 1, 1);
    // grid_layout->addWidget(trans_button, 1, 1, 1, 1);
    this->setLayout(grid_layout);

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

    // add item into tablewidget..
    int cur_row = corr_list->rowCount();
    int cur_col = corr_list->columnCount();
    // std::cout << "row count : " << cur_row << std::endl;
    // std::cout << "col count : " << cur_col << std::endl;
    corr_list->insertRow(cur_row);
    // QString num0(QString::number(1));
    QTableWidgetItem item0(QString::number(1));
    corr_list->setItem(cur_row, 0, new QTableWidgetItem(QString::number(snids0[0])));
    QTableWidgetItem item1(QString::number(2));
    corr_list->setItem(cur_row, 1, new QTableWidgetItem(QString::number(snids1[0])));

    // QString str = tablist->horizontalHeaderItem(1)->text();
    // std::cout << "test column header : " << str.toStdString() << std::endl;
}

void pair_treeViewer::est_relations() {
    std::cout << "est relation" << std::endl;
    std::vector<int> snids0 = viewer0->selected_nids;
    std::vector<int> snids1 = viewer1->selected_nids; 
    relation r(snids0[0], snids0[1], QString("cover"));
    relations.push_back(r);
    // clean the selelcted
    viewer0->clear_selection();
    // viewer1->clear_selection();
    // relation r2(snids0[0], snids0[1], QString("cover"));
    // if (r == r2 ) { 
    //     std::cout << "equal test success" << std::endl;
    // }
    // add item into tablewidget..
    int cur_row = relation_list->rowCount();
    int cur_col = relation_list->columnCount();
    relation_list->insertRow(cur_row);
    QTableWidgetItem item0(QString::number(1));
    relation_list->setItem(cur_row, 0, new QTableWidgetItem(QString::number(snids0[0])));
    QTableWidgetItem item1(QString::number(2));
    relation_list->setItem(cur_row, 1, new QTableWidgetItem(QString::number(snids0[1])));

    std::cout << r.rtypes[0].toStdString() << std::endl;
    QString rstring = r.rtypes.join(",");
    relation_list->setItem(cur_row, 2, new QTableWidgetItem(rstring));
}