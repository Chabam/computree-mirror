#ifndef DUMMYCLOUDINDEX_H
#define DUMMYCLOUDINDEX_H

#include <vector>

class DummyCloudIndex : public std::vector<size_t>
{
public:
    size_t first() const { return *this->begin(); }
    bool doIndexesFollowEachOther() const { return false; }
};

#endif // DUMMYCLOUDINDEX_H
