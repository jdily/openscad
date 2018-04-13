#include "vizTools.h"
#include <typeinfo>

void vizTools::vizTree_graphviz(tree_hnode *tree, QString filename) {
    // TODO : deal with the path..
    QString dot_filename = QString("/mnt/c/Users/jdily/Desktop/project/ddCAD/data/graph_viz/%1.dot").arg(filename);
    QFile file(dot_filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
	QTextStream out(&file);
	out<<"digraph G{"<<endl;
    tree_hnode::sibling_iterator children;
	tree_hnode::iterator iterator;
	iterator = tree->begin();
    std::vector<QString> node_info;
    std::vector<QString> edge_info;

    while(iterator!= tree->end()) {
        std::string type = (*iterator)->type;
        int index = (*iterator)->idx;
        // TODO : check should it start from 0
        QString indexstr = QString::number(index);
        QString label = QString("[label=%1]").arg(type.c_str());
        QString parent_id = QString("[parent_id=%1]").arg((*iterator)->parent_idx);
        QString f = QString("%1%2%3;").arg(indexstr).arg(label).arg(parent_id);
        node_info.push_back(f);

        children = tree->begin(iterator);
        while (children != tree->end(iterator)) {
            QString e = QString("%1 --> %2;").arg((*iterator)->idx).arg((*children)->idx);
            edge_info.push_back(e);
            ++children;
        }
        // out << f << endl;
    //     QString f = "", fl = "";
    //     for each(int index in (*iterator)->obj->origIdx) {
	// 		f += "n" + QString::number(index);
	// 		//fl += QString::number(index)+",";
	// 	}
        ++iterator;
    }
    for (auto node : node_info) {
        out << node << endl;
    }
    for (auto edge : edge_info) {
        out << edge << endl;
    }

    out << "}" << endl;
    file.close();
}