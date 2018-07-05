#include "vizTools.h"
#include <typeinfo>
#include <fstream>

#include "cgalutils.h"
#include <CGAL/IO/print_wavefront.h>
#include <QDir>
#include "LFD.h"
void vizTools::vizTree_graphviz(tree_hnode *tree, QString filename) {


}

void vizTools::write_tree_with_csginfo(tree_hnode *tree, QString filename, QString basepath) {
    // 1. write the graph out
    std::cout << "in write_tree_with_csginfo" << std::endl;
    QString dot_filename = QString("%1/graph_viz/%2.dot").arg(basepath).arg(filename);
    QFile file(dot_filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
	QTextStream out(&file);
	out<<"digraph G{"<<endl;
    tree_hnode::sibling_iterator children;
	tree_hnode::iterator iterator;
	iterator = tree->begin();
    std::vector<QString> node_info;
    std::vector<QString> edge_info;
    QString csg_obj_basepath = QString("%1/csginfo/%2").arg(basepath).arg(filename);
    std::cout << csg_obj_basepath.toStdString() << std::endl;
    QDir dir(csg_obj_basepath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    
    // while (iterator != tree->end()) {
    //     std::string type = (*iterator)->type;
    //     int index = (*iterator)->idx;
    //     std::string obj = (*iterator)->obj_filename;
    //     QString indexstr = QString::number(index);
    //     QString label = QString("[label=%1]").arg(type.c_str());
    //     QString parent_id = QString("[parent_id=%1]").arg((*iterator)->parent_idx);
    //     QString obj_filename = QString("[obj=%1%2]").arg(csg_obj_basepath).arg(obj.c_str());
    //     QString f = QString("%1%2%3%4;").arg(indexstr).arg(label).arg(parent_id).arg(obj_filename);
    //     node_info.push_back(f);
    //     children = tree->begin(iterator);
    //     while (children != tree->end(iterator)) {
    //         QString e = QString("%1 -> %2;").arg((*iterator)->idx).arg((*children)->idx);
    //         edge_info.push_back(e);
    //         ++children;
    //     }
    //     ++iterator;
    // }
    // for (auto node : node_info) {
    //     out << node << endl;
    // }
    // for (auto edge : edge_info) {
    //     out << edge << endl;
    // }
    // out << "}" << endl;
    // file.close();
    // 2. export obj files out...

    iterator = tree->begin();
    while (iterator != tree->end()) {
        int index = (*iterator)->idx;
        // << " " << (*iterator)->csgnode->geom->getDimension()
        // " " << (*iterator)->csgnode->dump()  <<
        std::cout << index << " " << (*iterator)->type <<  std::endl;
        if ((*iterator)->csgnode == nullptr) {
            std::cout << index << " have null csgnode info" << std::endl;
        }
        // std::cout << ">???" <<
        // TODO : check what is the current geom type -> assume it's a polyset
        // if (index == 38 || index == 39) {
        //     std::cout << "skip" << std::endl;
        //     ++iterator;
        //     continue;
        // }
        // if (index == 38) {
        //     std::cout << (*iterator)->csgnode->geom->dump() << std::endl;
        // }



        // CGAL_Nef_polyhedron *poly = CGALUtils::createNefPolyhedronFromGeometry(*((*iterator)->csgnode->geom));	
        // if (poly == nullptr) {
        //     std::cout << "null poly" << std::endl;
        // }
        // std::cout << poly->memsize() << std::endl;
        // std::cout << "conveted to CGAL_nef" << std::endl;
        // // std::cout << poly->dump() << std::endl;

        // // TODO : check the type of the geometry...
		// CGAL_Polyhedron cgal_poly = CGALUtils::nef_to_poly_surf(poly);
        // std::cout << "conveted to CGAL_Polyhedron" << std::endl;



        
        // std::cout << "vertex number : " << cgal_poly.size_of_vertices() << std::endl;
        // std::ofstream objfile;
        // QString obj_path = QString("%1/%2").arg(csg_obj_basepath).arg((*iterator)->obj_filename.c_str());
        // objfile.open(obj_path.toStdString().c_str());
        // CGAL::print_polyhedron_wavefront(objfile, cgal_poly);

		// Surface_mesh cgal_smesh = CGALUtils::nef_to_surface(poly);
        // std::cout << "conveted to CGAL_Surface model" << std::endl;
        // std::cout << "vertex number : " << cgal_smesh.number_of_vertices() << std::endl;
        // std::ofstream objfile;
        // QString obj_path = QString("%1/%2").arg(csg_obj_basepath).arg((*iterator)->obj_filename.c_str());
        // objfile.open(obj_path.toStdString().c_str());
        ++iterator;
    }
}

tree_hnode* vizTools::make_layout_graphviz(tree_hnode *tree, QString filename, QString basepath) {
    // TODO : deal with the path..
    
    QString dot_filename = QString("%1/graph_viz/%2.dot").arg(basepath).arg(filename);
    // QString dot_filename = QString("/mnt/c/Users/jdily/Desktop/project/ddCAD/data/graph_viz/%1.dot").arg(filename);
    QFile file(dot_filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
	QTextStream out(&file);
	out<<"digraph G{"<<endl;
    tree_hnode::sibling_iterator children;
	tree_hnode::iterator iterator;
	iterator = tree->begin();
    std::vector<QString> node_info;
    std::vector<QString> edge_info;

    std::cout << "here is fine " << std::endl;

    QMap<int, shared_ptr<const Geometry>> tree_geom;
    tree_geom.clear();

    while(iterator!= tree->end()) {
        std::string type = (*iterator)->type;
        int index = (*iterator)->idx;
        tree_geom.insert(index, (*iterator)->geom);
        std::cout << index << " " << (*iterator)->type <<  std::endl;
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

    std::cout << "here is fine as well.." << std::endl;
    ///////////////////////////////////////////////
    QString layout_dot_filename = QString("%1/graph_viz/%2_layout.dot").arg(basepath).arg(filename);
    // QString layout_dot_filename = QString("/mnt/c/Users/jdily/Desktop/project/ddCAD/data/graph_viz/%1_layout.dot").arg(filename);
    std::cout << "layout dot name : " << layout_dot_filename.toStdString() << std::endl;
	QString commandq = QString("dot -Tdot %1 -o %2").arg(dot_filename).arg(layout_dot_filename);
	QByteArray ba = commandq.toLocal8Bit();
	char* command = ba.data();
    system(command);

    // read it back
    tree_hnode* layout_tree = vizTools::read_graphviz(layout_dot_filename);
    // [TODO] assign the corresponding geom if any?? between "tree" and "layout tree"
    iterator = layout_tree->begin();
    while (iterator != layout_tree->end()) {
        int idx = (*iterator)->idx;
        (*iterator)->geom = tree_geom[idx];
        ++iterator;
    }



    // tree_hnode* layout_tree = nullptr;
    return layout_tree;
}

void vizTools::parse_node_content(QString content) {
    int first = content.indexOf("[");
    int last = content.indexOf("]");
    std::cout << "first : " << first << " " << last << std::endl;
    // QString detail = content.mid(first, last-first);
    // std::cout << detail.toStdString() << std::endl;

}

tree_hnode* vizTools::read_graphviz(QString filename) {
    tree_hnode* out = new tree_hnode();
    std::ifstream myfile (filename.toUtf8());
    std::string tmp;
    char delimeter(';');
    int line_no = 0;
    QMap<int, hnode*> hnodes;
    if (myfile.is_open()) {
        while (std::getline(myfile, tmp, delimeter)) {
            // std::cout << line_no << std::endl;
            line_no += 1;
            std::size_t found1 = tmp.find("{");
            std::size_t found2 = tmp.find("}");
            if (found1==std::string::npos || found2==std::string::npos) {
                // 1. split thr string
                QString qtmp(tmp.c_str());
                QStringList tmps = qtmp.split(" ");
                // std::cout << "split size : " << tmps.size() << std::endl;
                // std::cout << tmps[0].toStdString() << std::endl;
                // 2. split into two situation, node or edge
                if (tmps.size() == 2 && tmps[0].trimmed() != "node") {
                    // node
                    hnode* _node = new hnode();
                    int node_id = tmps[0].toInt();
                    _node->idx = node_id;
                    // std::cout << "node id : " << node_id << std::endl;
                    int first = tmps[1].indexOf("[");
                    int last = tmps[1].indexOf("]");
                    QString detailstr = tmps[1].mid(first+1, last-first-1);
                    // std::cout << "detail : " << detailstr.toStdString() << std::endl;
                    QStringList details = detailstr.split(",");
                    int pos_index = 0;
                    for (int i = 0; i < details.size(); i++) {
                    // for (auto d : details) {
                        QString d = details[i].trimmed();
                        // std::cout << d.toStdString() << std::endl;
                        if (d.contains("pos")) {
                            // merge the next one...
                            pos_index = i;
                        } else if (d.contains("\"")) {
                            // std::cout << "skip" << std::endl;
                            continue;
                        } else {
                            // else properties..
                            QStringList parse = d.split("=");
                            if (parse[0] == "label") {
                                _node->type = parse[1].toStdString();
                            } else if (parse[0] == "parent_id") {
                                _node->parent_idx = parse[1].toInt();
                                // std::cout << "check parent_id : " << parse[1].toInt() << std::endl;
                            }
                        }
                    }
                    _node->pos_x = details[pos_index].split("\"")[1].toFloat();
                    _node->pos_y = details[pos_index+1].split("\"")[0].toFloat();
                    hnodes.insert(node_id, _node);
                    // std::cout << _node->pos_x  << " " << _node->pos_y << std::endl;
                    // QString pos_str = details[pos_index].split("\"")[1]+details[pos_index+1].split("\"")[0];
                    // std::cout << "pos_str : " << pos_str.toStdString() << std::endl;
                } else if (tmps.size() == 8) {
                    // edge -> looks like we don't need the edge info, cuz we record parent id in the node already.
                    // std::cout << "edge case" << std::endl;
                    // std::cout << "edge case : " << tmps[0].trimmed().toStdString() << std::endl;
                    // std::cout << "edge case : " << tmps[1].trimmed().toStdString() << std::endl;
                    // std::cout << "edge case : " << tmps[2].trimmed().toStdString() << std::endl;
                }
            }

        }
        myfile.close();
    }
    // std::cout << "line count : " << line_no << std::endl;
    QList<int> keys = hnodes.keys();    
    QMap<int, tree_hnode::iterator> hnode_iters;

    for (int k : keys) {
        tree_hnode::iterator newNodeIter;
        int parent_id = hnodes[k]->parent_idx;
        std::cout << k << "parent id : " << parent_id << std::endl;
        if (parent_id == -1) {
            newNodeIter = out->begin();
            newNodeIter = out->insert(newNodeIter, hnodes[k]);
        } else {
            newNodeIter = out->append_child(hnode_iters[parent_id], hnodes[k]);
        }
        hnode_iters.insert(k, newNodeIter);
    }
    return out;
}