#ifndef POLYGONPOINTSPICKERBYMODEL_H
#define POLYGONPOINTSPICKERBYMODEL_H

#include "pointspickerbymodel.h"
#include "polygonpointspicker.h"

/**
 * @brief Picker that use a polygone to pick points
 */
class PolygonPointsPickerByModel : public PointsPickerByModel
{
public:
    PolygonPointsPickerByModel(const QVector<TrianglePlane> &triangles);

private:
    QVector<TrianglePlane>      m_triangles;

protected:
    /**
     * @brief Create a new picker to use with a model
     */
    GenericPicker* createNewPicker();
};

#endif // POLYGONPOINTSPICKERBYMODEL_H
