#include "polygonitemspicker.h"

#include "amkglapp.h"
#include "polygonobjectsofitempicker.h"

#include <QtConcurrent/QtConcurrent>

PolygonItemsPicker::PolygonItemsPicker(const QVector<Plane> &polygonPlanes,
                                       const QVector<TrianglePlane> &triangles,
                                       const PermanentItemScene* scene) : ItemsPicker()
{
    m_scene = (PermanentItemScene*)scene;
    m_polygonPlanes = polygonPlanes;
    m_triangles = triangles;
    m_hasEnoughTriangle = (m_triangles.size() > 0);
}

bool PolygonItemsPicker::isValid() const
{
    if(!m_hasEnoughTriangle || (m_scene == nullptr))
        return false;

    return ItemsPicker::isValid();
}

void PolygonItemsPicker::addItemDrawableToCompute(const Item *item)
{
    if(!isValid())
        return;

    PolygonObjectsOfItemPicker* picker = new PolygonObjectsOfItemPicker(m_polygonPlanes, m_triangles);
    picker->setCheckOperation(getCheckOperation());
    picker->setSelectionOperation(getSelectionOperation());
    picker->setGraphicsView(getGraphicsView());
    picker->setItem(item, m_scene);

    m_elementsToCompute.append(picker);
}

bool PolygonItemsPicker::internalCompute()
{
    QElapsedTimer time;
    time.start();

    AMKglLOG->addInfoMessage(QString("N elements to compute : %1").arg(m_elementsToCompute.size()));

    QtConcurrent::blockingMap(m_elementsToCompute, &staticTestItems);

    qDeleteAll(m_elementsToCompute.begin(), m_elementsToCompute.end());

    AMKglLOG->addInfoMessage(QString("Picking time elapsed : %1 µs").arg(time.nsecsElapsed()/1000.0));

    return true;
}

void PolygonItemsPicker::staticTestItems(const PolygonItemsPicker::ToCompute &params)
{
    params->compute();
}
