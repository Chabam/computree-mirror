#ifndef POLYGONMESHOBJECTSPICKERBYMODEL_H
#define POLYGONMESHOBJECTSPICKERBYMODEL_H

#include "meshobjectspickerbymodel.h"
#include "polygonmeshobjectspicker.h"

/**
 * @brief Picker that use a polygone to pick objects of mesh
 */

class PolygonMeshObjectsPickerByModel : public MeshObjectsPickerByModel
{
public:
    PolygonMeshObjectsPickerByModel(const QVector<Plane>& polygonPlanes,
                                    const QVector<TrianglePlane>& triangles,
                                    const quint8& nVertexPerObject);

private:
    QVector<Plane>              m_polygonPlanes;
    QVector<TrianglePlane>      m_triangles;

protected:
    /**
     * @brief Create a new picker to use with a model
     */
    GenericPicker* createNewPicker();
};

#endif // POLYGONMESHOBJECTSPICKERBYMODEL_H
