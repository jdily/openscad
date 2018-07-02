#pragma once 

#include <QMap>
#include <QPainterPath>
#include <QList>

class GeomGroup {
public:
    GeomGroup(QList<int> selected_ids, int ftype);
    ~GeomGroup();

// private:
    int func_type;
    QList<int> selected_nids;
};