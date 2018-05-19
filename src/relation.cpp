#include "relation.h"


relation::relation() {

}

relation::relation(int id0, int id1, QString r) {
    rkey.first = id0;
    rkey.second = id1;
    int i = rtypes.indexOf(r);
    if (i == -1) {
        rtypes.append(r);
    }
    std::cout << "this constructor" << std::endl;
    std::cout << rtypes.length() << std::endl;
} 

relation::relation(int id0, int id1, QStringList rs) {
    rkey.first = id0;
    rkey.second = id1;
    rtypes = rs;
}

relation::~relation() {
    
}

void relation::add_relation(int id0, int id1, QString r) {
    int i = rtypes.indexOf(r);
    if (i == -1) {
        rtypes.append(r);
    }
} 

bool relation::operator==(const relation& a) const {
    return (a.rkey == rkey);
}