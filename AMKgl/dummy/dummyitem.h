#ifndef DUMMYITEM_H
#define DUMMYITEM_H

#include <QObject>

#include "definegraphicsview.h"
#include "defineitempainter.h"
#include "definemodel.h"

class DummyItem : public QObject {
    Q_OBJECT

public:
    bool isSelected() const { return false; }

    void draw(AMKgl::GraphicsView& graphicsView, AMKgl::ItemPainter& painter) {}

    AMKgl::ItemModel* model() const { return NULL; }

public slots:
    void setSelected(bool s) {}

signals:
    void itemSelectionChanged(bool e, DummyItem* item);
};

#endif // DUMMYITEM_H
