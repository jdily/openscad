#pragma once

#include "memory.h"

class MyCSGTreeTest {

public:
    MyCSGTreeTest(size_t limit) : limit(limit) {}
    ~MyCSGTreeTest() {}
private:
    size_t limit;
    shared_ptr<class CSGNode> rootnode;
    
};