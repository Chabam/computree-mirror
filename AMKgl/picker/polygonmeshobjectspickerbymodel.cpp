#include "polygonmeshobjectspickerbymodel.h"

PolygonMeshObjectsPickerByModel::PolygonMeshObjectsPickerByModel(const QVector<Plane>& polygonPlanes,
                                                                 const QVector<TrianglePlane>& triangles,
                                                                 const quint8& nVertexPerObject) : MeshObjectsPickerByModel(nVertexPerObject)
{
    m_polygonPlanes = polygonPlanes;
    m_triangles = triangles;
}

GenericPicker* PolygonMeshObjectsPickerByModel::createNewPicker()
{
    return new PolygonMeshObjectsPicker(m_polygonPlanes, m_triangles, getNVertexPerObject());
}
