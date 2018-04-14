#include "vizTools.h"
#include <typeinfo>
#include <fstream>


void vizTools::vizTree_graphviz(tree_hnode *tree, QString filename) {


}

tree_hnode* vizTools::make_layout_graphviz(tree_hnode *tree, QString filename) {
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
            QString e = QString("%1 -> %2;").arg((*iterator)->idx).arg((*children)->idx);
            edge_info.push_back(e);
            ++children;
        }
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
    ///////////////////////////////////////////////
    QString layout_dot_filename = QString("/mnt/c/Users/jdily/Desktop/project/ddCAD/data/graph_viz/%1_layout.dot").arg(filename);
    std::cout << "layout dot name : " << layout_dot_filename.toStdString() << std::endl;
	QString commandq = QString("dot -Tdot %1 -o %2").arg(dot_filename).arg(layout_dot_filename);
	QByteArray ba = commandq.toLocal8Bit();
	char* command = ba.data();
    system(command);

    // read it back
    tree_hnode* layout_tree = vizTools::read_graphviz(layout_dot_filename);

    return tree;
}

void vizTools::parse_node_content(QString content) {
    int first = content.indexOf("[");
    int last = content.indexOf("]");
    std::cout << "first : " << first << " " << last << std::endl;
    // QString detail = content.mid(first, last-first);
    // std::cout << detail.toStdString() << std::endl;

}

tree_hnode* vizTools::read_graphviz(QString filename) {
    tree_hnode* out = new tree_hnode;
    std::ifstream myfile (filename.toUtf8());
    std::string tmp;
    char delimeter(';');
    int line_no = 0;
    QMap<int, hnode*> hnodes;
    if (myfile.is_open()) {
        while (std::getline(myfile, tmp, delimeter)) {
            std::cout << line_no << std::endl;
            line_no += 1;
            std::size_t found1 = tmp.find("{");
            std::size_t found2 = tmp.find("}");
            if (found1==std::string::npos || found2==std::string::npos) {
                // 1. split thr string
                QString qtmp(tmp.c_str());
                QStringList tmps = qtmp.split(" ");
                std::cout << "split size : " << tmps.size() << std::endl;
                // 2. split into two situation, node or edge
                if (tmps.size() == 2 && tmps[0] != "node") {
                    // node
                    hnode* _node = new hnode();
                    int node_id = tmps[0].toInt();
                    _node->idx = node_id;
                    // std::cout << "node id : " << node_id << std::endl;
                    int first = tmps[1].indexOf("[");
                    int last = tmps[1].indexOf("]");
                    QString detailstr = tmps[1].mid(first+1, last-first-1);
                    std::cout << "detail : " << detailstr.toStdString() << std::endl;
                    QStringList details = detailstr.split(",");
                    int pos_index = 0;
                    for (int i = 0; i < details.size(); i++) {
                    // for (auto d : details) {
                        QString d = details[i].trimmed();
                        std::cout << d.toStdString() << std::endl;
                        if (d.contains("pos")) {
                            // merge the next one...
                            pos_index = i;
                        } else if (d.contains("\"")) {
                            std::cout << "skip" << std::endl;
                            continue;
                        } else {
                            // else properties..
                            QStringList parse = d.split("=");
                            if (parse[0] == "label") {
                                _node->type = parse[1].toStdString();
                            } else if (parse[1] == "parent_id") {
                                _node->parent_idx = parse[1].toInt();
                            }
                        }
                    }
                    _node->pos_x = details[pos_index].split("\"")[1].toFloat();
                    _node->pos_y = details[pos_index+1].split("\"")[0].toFloat();
                    std::cout << _node->pos_x  << " " << _node->pos_y << std::endl;
                    // QString pos_str = details[pos_index].split("\"")[1]+details[pos_index+1].split("\"")[0];
                    // std::cout << "pos_str : " << pos_str.toStdString() << std::endl;
                } else if (tmps.size() == 8) {
                    // edge
                }
                // 3.   

            }

        }
        myfile.close();
    }
    std::cout << "line count : " << line_no << std::endl;
    
    // QFile file(filename);
    // if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    //     QTextStream in(&file);
    //     int line_no = 0;
    //     while (!in.atEnd()) {
    //         QString line = in.readLine();
    //         line_no ++;
    //     }
    //     file.close(); 
    //     std::cout << "line count : " << line_no << std::endl;
    // }
    return out;
}