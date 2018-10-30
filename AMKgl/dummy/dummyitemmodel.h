#ifndef DUMMYITEMMODEL_H
#define DUMMYITEMMODEL_H

#include "defineitem.h"

#include <vector>

class DummyItemModel {
public:
    typedef std::vector<AMKgl::Item*>::const_iterator const_iterator;

    const_iterator begin() const { return v.begin(); }
    const_iterator end() const { return v.end(); }

private:
    std::vector<AMKgl::Item*> v;
};

#endif // DUMMYITEMMODEL_H
