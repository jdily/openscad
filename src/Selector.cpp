#include "Selector.h"
#include <numeric>      // std::accumulate
#include <algorithm>

Selector::Selector() {}
Selector::Selector(QList<QPolygonF> stroke_polys, QPainterPath stroke) : m_poly(stroke_polys), m_stroke(stroke) {
    // get all stroke pnts.
    // QList<QPointF> ps;
    ps.clear();
    std::cout << m_stroke.elementCount() << " elements in the stroke" << std::endl;
    for (int i = 0; i < m_stroke.elementCount(); i++) {
        QPointF cur_pos = m_stroke.elementAt(i);
        ps.append(cur_pos);
    }
    std::cout << ps.length() << " stroke pnts" << std::endl;
    mean_scs.clear();
    mean_css.clear();
    stdev_scs.clear();
    stdev_css.clear();
    beta = 0.5;
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
        }
    }
}

QList<QList<int>> Selector::make_group(QMap<int, std::vector<Eigen::Vector3d>> samples, tree_hnode* tree) {
    // [TODO]: group them based on the tree
    // currently, naive grouping
    int index = 0;

    QList<int> key = samples.keys();
    int _size = key.length();
    std::sort(key.begin(), key.end());

    QList<QList<int>> groups;
    for (int i = 1; i <= _size; i++) {
        QList<QList<int>> combs = comb(key, i);
        for (int c = 0; c < combs.length(); c++) {
            groups.append(combs[c]);
        }
    }
    return groups;
}


QList<int> Selector::smart_select(QMap<int, std::vector<Eigen::Vector3d>> samples, tree_hnode* tree) {
    // clear and reset.
    mean_scs.clear();
    mean_css.clear();
    stdev_scs.clear();
    stdev_css.clear();
    
    // first do the passing test....
    QList<int> covered_geom_id = cover_select(samples);
    if (covered_geom_id.length() == 0) {
        return covered_geom_id;
    }


    QMap<int, std::vector<Eigen::Vector3d>> covered_samples;
    for (int &id : covered_geom_id) {
        covered_samples.insert(id, samples[id]);
    }
    std::cout << covered_samples.keys().length() << " geometries are covered and sampled.." << std::endl;

    // 1. grouping geometry -> each group is represented as node id (QList<int> group) 
    QList<QList<int>> groups = this->make_group(covered_samples, tree);
    std::cout << "finish grouping " << std::endl;
    // std::cout << groups.length() << " groups are grouped" << std::endl;
    // for (int i = 0; i < groups.length(); i++) {
    //     for (int j = 0; j < groups[i].length(); j++) {
    //         std::cout << groups[i][j] << " ";
    //     }
    //     std::cout << std::endl;
    // }
    // seems fine here..

    // 2. evaluate each group
    for (int k = 0; k < groups.length(); k++) {
        // std::cout << " k : " << k << std::endl;
    // for (int &k : groups.keys()) {
        eval_group(k, groups[k], covered_samples);
    }
    std::cout << "finish evaluate group" << std::endl;

    // re-normalize the mean and stdev
    double avg_mean_sc = std::accumulate(mean_scs.begin(), mean_scs.end(), 0.0) / mean_scs.length();
    double avg_mean_cs = std::accumulate(mean_css.begin(), mean_css.end(), 0.0) / mean_css.length();
    double avg_stdev_sc = std::accumulate(stdev_scs.begin(), stdev_scs.end(), 0.0) / stdev_scs.length();
    double avg_stdev_cs = std::accumulate(stdev_css.begin(), stdev_css.end(), 0.0) / stdev_css.length();
    for (int k = 0; k < groups.length(); k++) {
        mean_scs[k] /= avg_mean_sc;
        stdev_scs[k] /= avg_stdev_sc;
        mean_css[k] /= avg_mean_cs;
        stdev_css[k] /= avg_stdev_cs;
    }
    rscore.clear();
    double score = 0.0;
    for (int k = 0; k < groups.length(); k++) {
        score = beta*(mean_scs[k]+mean_css[k]) + (1-beta)*(stdev_scs[k]+stdev_css[k]);
        rscore.append(score);
    }
    // find the smallest index;
    double min_score = *std::min_element(rscore.begin(), rscore.end());
    double min_ind = rscore.indexOf(min_score);
    // QList<int> out = groups[min_ind];
    // return out;
    return groups[min_ind];
}

void Selector::eval_group(int ind, QList<int> group, QMap<int, std::vector<Eigen::Vector3d>> samples) {
    // use path and the corresponding sample points to compute the score.
    // get all candidate points
    QList<QPointF> pc;

    // std::cout << "[eval group] " << std::endl;
    // for (int &sind: samples.keys()) {
    //     std::cout << sind << " ";
    // }
    // std::cout<< std::endl;
    // for (int &gind: group) {
    //     std::cout << gind << " ";
    // }
    // std::cout<< std::endl;

    for (int g = 0; g < group.length(); g++) {
    // for (int &gind : group) {
        int gind = group[g];
        // std::cout << "gind : " << gind <<  std::endl;
        // " " << samples[gind].size() << std::endl;
        for (int i = 0; i < samples[gind].size(); i++) {
            // std::cout << "i : " << i << std::endl; 
        // for (auto &pnt : samples[gind]) {
            QPointF p(samples[gind][i][0], samples[gind][i][1]);
            pc.append(p);
        }
    }
    // measure the closest distance from pc to ps
    QList<double> T_cs;
    for (int i = 0; i < pc.length(); i++) {
        QList<double> dists;
        double dist = 0.0;
        for (int j = 0; j < ps.length(); j++) {
            dist = sqrt((pc[i].x()-ps[j].x())*(pc[i].x()-ps[j].x())+(pc[i].y()-ps[j].y())*(pc[i].y()-ps[j].y()));
            dists.append(dist);
        }
        // find the smallest id
        double min_d = *std::min_element(dists.begin(), dists.end());
        T_cs.append(min_d);
    }
    // std::cout << "finish T_cs computation" << std::endl;

    // measure the closest distance from ps to pc
    QList<double> T_sc;
    for (int i = 0; i < ps.length(); i++) {
        QList<double> dists;
        double dist;
        for (int j = 0; j < pc.length(); j++) {
            dist = sqrt((ps[i].x()-pc[j].x())*(ps[i].x()-pc[j].x())+(ps[i].y()-pc[j].y())*(ps[i].y()-pc[j].y()));
            dists.append(dist);
        }
        double min_d = *std::min_element(dists.begin(), dists.end());
        T_sc.append(min_d);
    }
    // std::cout << "finish T_sc computation" << std::endl;
    double sum_cs = std::accumulate(T_cs.begin(), T_cs.end(), 0.0);
    double mean_cs = sum_cs / T_cs.length();
    double sum_sc = std::accumulate(T_sc.begin(), T_sc.end(), 0.0);
    double mean_sc = sum_sc / T_sc.length();

    double sq_sum_cs = std::inner_product(T_cs.begin(), T_cs.end(), T_cs.begin(), 0.0);
    double stdev_cs = std::sqrt(sq_sum_cs / T_cs.length() - mean_cs * mean_cs);
    double sq_sum_sc = std::inner_product(T_sc.begin(), T_sc.end(), T_sc.begin(), 0.0);
    double stdev_sc = std::sqrt(sq_sum_sc / T_sc.length() - mean_sc * mean_sc);
    

    mean_css.append(mean_cs);
    mean_scs.append(mean_sc);
    stdev_css.append(stdev_cs);
    stdev_scs.append(stdev_sc);

    // mean_css.insert(gind, mean_cs);
    // mean_scs.insert(gind, mean_sc);
    // stdev_css.insert(gind, stdev_cs);
    // stdev_scs.insert(gind, stdev_sc);
}

template <typename Iterator>
inline bool next_combination(const Iterator first, Iterator k, const Iterator last)
{
   /* Credits: Thomas Draper */
   // http://stackoverflow.com/a/5097100/8747
   if ((first == last) || (first == k) || (last == k))
      return false;
   Iterator itr1 = first;
   Iterator itr2 = last;
   ++itr1;
   if (last == itr1)
      return false;
   itr1 = last;
   --itr1;
   itr1 = k;
   --itr2;
   while (first != itr1)
   {
      if (*--itr1 < *itr2)
      {
         Iterator j = k;
         while (!(*itr1 < *j)) ++j;
         std::iter_swap(itr1,j);
         ++itr1;
         ++j;
         itr2 = k;
         std::rotate(itr1,j,last);
         while (last != j)
         {
            ++j;
            ++itr2;
         }
         std::rotate(k,itr2,last);
         return true;
      }
   }
   std::rotate(first,k,last);
   return false;
}
QList<QList<int>> Selector::comb(QList<int> numbers, int k) {
    int n = numbers.length();
    QList<QList<int>> out;
    do {
        QList<int> c;
        for (int i = 0; i < k; ++i) {
            c.append(numbers[i]);
        }
        out.append(c);
    } while(next_combination(numbers.begin(),numbers.begin() + k, numbers.end()));
    return out;   
}
