#ifndef POLYGONITEMSPICKER_H
#define POLYGONITEMSPICKER_H

#include "itemspicker.h"
#include "geometry/triangleplane.h"

class PolygonObjectsOfItemPicker;

/**
 * @brief Picker that use a polygone to pick item drawable
 */
class PolygonItemsPicker : public ItemsPicker
{
public:
    PolygonItemsPicker(const QVector<Plane>& polygonPlanes,
                       const QVector<TrianglePlane> &triangles,
                       const PermanentItemScene *scene);

    /**
     * @brief Returns true if it is at least one triangle plane
     */
    virtual bool isValid() const;

    /**
     * @brief Returns true if we must continue the visit, false otherwise
     */
    bool mustContinueVisit() const { return true; }

    /**
     * @brief Call this method to add a item drawable to test
     * @param item : item to test
     * @param scene : scene that contains this item
     */
    void addItemDrawableToCompute(const Item* item);

protected:

    /**
     * @brief Call this method to compute all item drawable added
     */
    bool internalCompute();

private:
    typedef PolygonObjectsOfItemPicker*   ToCompute;
    typedef QList<ToCompute>        ItemCollection;

    PermanentItemScene*         m_scene;
    QVector<Plane>              m_polygonPlanes;
    QVector<TrianglePlane>      m_triangles;
    ItemCollection              m_elementsToCompute;
    bool                        m_hasEnoughTriangle;

    /**
     * @brief Called by a thread to compute items
     */
    static void staticTestItems(const ToCompute &params);
};


#endif // POLYGONITEMSPICKER_H
