#ifndef DUMMYGLOBALCLOUD_H
#define DUMMYGLOBALCLOUD_H

#include <vector>

#include "elementinfo.h"
#include "definecolor.h"
#include "definenormal.h"
#include "includecolor.h"
#include "includenormal.h"

class DummyGlobalInfoCloud : public std::vector<ElementInfo> {

public:
    ElementInfo& operator [](const size_t& index) { return info; }

private:
    ElementInfo info;
};

class DummyGlobalColorCloud : public std::vector<AMKgl::GlobalColor> {

public:
    AMKgl::GlobalColor& operator [](const size_t& index) { return color; }

private:
    AMKgl::GlobalColor color;
};

class DummyGlobalNormalCloud : public std::vector<AMKgl::GlobalNormal> {

public:
    AMKgl::GlobalNormal& operator [](const size_t& index) { return normal; }

private:
    AMKgl::GlobalNormal normal;
};

#endif // DUMMYGLOBALCLOUD_H
