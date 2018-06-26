#pragma once 

#include <QMap>
#include "node.h"
#include <algorithm>
#include <QList>
#include <QPolygonF>
#include "linalg.h"

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
    Selector(QList<QPolygonF> stroke_polys);
    ~Selector();

    QList<int> cover_select(QMap<int, std::vector<Eigen::Vector3d>> samples);
private:
    QList<QPolygonF> m_stroke;
    // QMap<IndexedPoint, int> sample_geom_map;
    QList<IndexedPoint> sample_geom_list;
    void build_sample_geom_map(QMap<int, std::vector<Eigen::Vector3d>> s);
};