#ifndef POLYGONITEMSPICKERBYMODEL_H
#define POLYGONITEMSPICKERBYMODEL_H

#include "itemspickerbymodel.h"
#include "geometry/triangleplane.h"

class PolygonItemsPickerByModel : public ItemsPickerByModel
{
public:
    PolygonItemsPickerByModel(const QVector<Plane>& polygonPlanes, const QVector<TrianglePlane> &triangles);

private:
    QVector<Plane>              m_polygonPlanes;
    QVector<TrianglePlane>      m_triangles;

protected:

    /**
     * @brief Create a new picker to use with a model
     */
    GenericPicker* createNewPicker();
};

#endif // POLYGONITEMSPICKERBYMODEL_H
