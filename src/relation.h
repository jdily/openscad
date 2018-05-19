#pragma once

#include <iostream>
#include <vector>
#include <QMap>
#include <QVector>
#include <QPair>
#include <map>


class relation {
public:
    relation();
    relation(int id0, int id1, QString r);
    relation(int id0, int id1, QStringList rs);
    ~relation();
    void add_relation(int id0, int id1, QString r);

    bool operator==(const relation& a) const;
// private:
    QPair<int, int> rkey;
    QStringList rtypes;
    // QMap<QPair<int, int>, QString> data;
};