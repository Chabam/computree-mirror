#ifndef DUMMYITEMPICKINGPAINTER_H
#define DUMMYITEMPICKINGPAINTER_H

#include "defineitempainter.h"
#include "includeitempainter.h"

class ObjectsProcessorForPicker;

class DummyItemPickingPainter : public AMKgl::ItemPainter {
public:
    DummyItemPickingPainter(const ObjectsProcessorForPicker &processor) {}

    /**
     * @brief Returns the used objects processor
     */
    ObjectsProcessorForPicker* getObjectsProcessorForPicker() const { return nullptr; }

    /**
     * @brief Returns true if at least one compute method (of the picker) returns true
     */
    bool isSelected() const { return false; }
};

#endif // DUMMYITEMPICKINGPAINTER_H
