#pragma once 

#include <QMap>
#include <QPainterPath>
#include "node.h"
#include <algorithm>
#include <QList>
#include <QPolygonF>
#include "linalg.h"
#include "iTree.h"

typedef stree<hnode*> tree_hnode;


struct IndexedPoint {
    IndexedPoint(int id, QPointF p) : _id(id), _pt(p) {}
    int _id;
    QPointF _pt;
    friend bool operator<(const IndexedPoint & a, const IndexedPoint & b) { return (a._id < b._id); }
};

// bool operator<(const IndexedPoint & a, const IndexedPoint & b) {
//     return (a.pid < b.pid);
// }


class Selector {
public:
    Selector();
    Selector(QList<QPolygonF> stroke_polys, QPainterPath strokepath);
    ~Selector();

    QList<int> cover_select(QMap<int, std::vector<Eigen::Vector3d>> samples);
    // [TODO] -> we need sample points, and the geometry itself. (probably the tree itself)?
    // take the shape similarity into consideration.
    QList<int> smart_select(QMap<int, std::vector<Eigen::Vector3d>> samples, tree_hnode* tree);
private:
    QList<QPolygonF> m_poly;
    QPainterPath m_stroke;
    // stroke pnts...
    QList<QPointF> ps;
    // QMap<IndexedPoint, int> sample_geom_map;
    QList<IndexedPoint> sample_geom_list;
    // QMap<int, double> mean_scs;
    // QMap<int, double> mean_css;
    // QMap<int, double> stdev_scs;
    // QMap<int, double> stdev_css;

    QList<double> mean_scs;
    QList<double> mean_css;
    QList<double> stdev_scs;
    QList<double> stdev_css;
    QList<double> rscore;
    double beta;

    void build_sample_geom_map(QMap<int, std::vector<Eigen::Vector3d>> s);
    void eval_group(int gind, QList<int> group, QMap<int, std::vector<Eigen::Vector3d>> samples);
    QList<QList<int>> make_group(QMap<int, std::vector<Eigen::Vector3d>> samples, tree_hnode* tree);

    QList<QList<int>> comb(QList<int> numbers, int k);


};