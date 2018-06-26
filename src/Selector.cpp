#include "Selector.h"

Selector::Selector() {}
Selector::Selector(QList<QPolygonF> stroke_polys, QPainterPath stroke) : m_poly(stroke_polys), m_stroke(stroke) {
}

Selector::~Selector() {}

QList<int> Selector::cover_select(QMap<int, std::vector<Eigen::Vector3d>> samples) {
    QList<int> out;
    // use sample_geom_map and m_stroke to check contains things.
    build_sample_geom_map(samples);
    for (int i = 0; i < sample_geom_list.length(); i++) {
        bool covered = m_poly[0].containsPoint(sample_geom_list[i]._pt, Qt::WindingFill);
        if (covered) {
            if (!out.contains(sample_geom_list[i]._id)) {
                out.append(sample_geom_list[i]._id);
            }
        }
    }
    sample_geom_list.clear();
    return out;
}

void Selector::build_sample_geom_map(QMap<int, std::vector<Eigen::Vector3d>> samples) {
    // convert to QPointF, and build the map sample_geom_map
    // int count = samples.keys().length();
    // int index = 0;
    for (auto &k : samples.keys()) {
        for (auto &s : samples[k]) {
            QPointF pt(s[0], s[1]);
            IndexedPoint ipt(k, pt);
            sample_geom_list.append(ipt);
            // sample_geom_map.insert(ipt, k);
            // index++;
        }
    }
}