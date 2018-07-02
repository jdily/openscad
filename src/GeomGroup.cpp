#include "GeomGroup.h"

GeomGroup::GeomGroup(QList<int> selected_ids, int ftype) : func_type(ftype), selected_nids(selected_ids) {
}

GeomGroup::~GeomGroup() {
}