#ifndef PERMANENTSCENESCENEELEMENTTYPE_H
#define PERMANENTSCENESCENEELEMENTTYPE_H

#include <QString>

namespace Scene {

    /**
     * @brief Type of object that a scene can draw
     */
    enum ObjectType {
        Point = 0,
        Ellipse,
        Line,
        Box,
        Sphere,
        Cylinder,
        Quads,
        Triangle,
        Pyramid,
        PointGlobalCloud,
        LineGlobalCloud,
        TriangleGlobalCloud,
        NumberOfElements,    // Don't use it ! just to know how many elements are in this enum
    };

    QString objectTypeToString(const ObjectType& type);
}

#endif // PERMANENTSCENESCENEELEMENTTYPE_H
