#pragma once

#include <iostream>
#include <vector>
#include <QMap>
#include <QVector>
#include <QPair>
#include <map>
// 1-to-1 correspondece?
class correspondence {
    public:
        correspondence();
        ~correspondence();
        void add_corr(int id0, int id1);
        // to string function????
    // private:
        // QMap<int, int> corr;
        QVector<QPair<int, int>> corr;

        // QMap<QVector<int>, QVector<int>> corrs;
};
