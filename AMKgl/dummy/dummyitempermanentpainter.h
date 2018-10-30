#ifndef DUMMYITEMPERMANENTPAINTER_H
#define DUMMYITEMPERMANENTPAINTER_H

#include "defineitempainter.h"
#include "includeitempainter.h"
#include "definegraphicsview.h"

class PermanentScene;
class PermanentItemInformation;

class DummyItemPermanentPainter : public AMKgl::ItemPainter {
public:
    DummyItemPermanentPainter(const AMKgl::GraphicsView& gv) {}

    void setItemInformation(const PermanentItemInformation* info) {}
    void setPermanentScene(const PermanentScene* scene) {}
};

#endif // DUMMYITEMPERMANENTPAINTER_H
