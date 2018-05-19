#pragma once

#include <string>
#include <memory>
#include <QList>
#include <QString>
#include "Tree.h"
#include "NodeVisitor.h"
#include "node.h"

#include "transformnode.h"
#include "correspondence.h"
// class PrimitiveNode;
// should we make it as a subclass of NodeVisitor???

// class geomTransferer : public NodeVisitor {
class geomTransferer {
    public:
        geomTransferer(Tree *ori);
        geomTransferer(Tree *ori, Tree *example);
        ~geomTransferer();

        // get another example tree
        void load_example_file(QString filename);
        void add_example_tree(Tree* example);

        // how to manually assign part for testing ?
        // 1. first version use the node id pair 
        Tree* transfer(int self_node_id, int exp_node_id);
        Tree* yet_another_transfer(int self_node_id, int exp_node_id);
        Tree* transfer_corrs(std::vector<correspondence> corrs);
        // the test function for two_cylinder transfer cases.
        Tree* transfer_cylinder();

        void param_change(std::string target, int node_id, std::string property, float tar_val);
    private:
        Tree *m_pSelf;
        Tree *m_pExample;
        QList<Tree*> m_pExamples;
        QMap<std::string, const AbstractNode*> aux_to_node;
    protected:
};