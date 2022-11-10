#include "permanentsceneelementtype.h"

#include <QObject>
#include <QDebug>

namespace Scene {
QString objectTypeToString(const ObjectType& type) {
    if(type == Point)
        return QObject::tr("Points");
    else if(type == Ellipse)
        return QObject::tr("Ellipses");
    else if(type == Line)
        return QObject::tr("Lignes");
    else if(type == Box)
        return QObject::tr("Voxels");
    else if(type == Sphere)
        return QObject::tr("Spheres");
    else if(type == Cylinder)
        return QObject::tr("Cylindres");
    else if(type == Quads)
        return QObject::tr("Quads");
    else if(type == Triangle)
        return QObject::tr("Triangles");
    else if(type == Pyramid)
        return QObject::tr("Pyramides");
    else if(type == PointGlobalCloud)
        return QObject::tr("Points du nuage global");
    else if(type == LineGlobalCloud)
        return QObject::tr("Arêtes du nuage global");
    else if(type == TriangleGlobalCloud)
        return QObject::tr("Faces du nuage global");

    Q_ASSERT_X(false, "Scene::objectTypeToString", QString("Object type to QString not implemented for type %1, contact the developper !").arg(type).toUtf8());
    qDebug() << "Scene::objectTypeToString" << ", " <<  "Object type to QString not implemented for type %1, contact the developper !"; return "";

    return QObject::tr("Unknown !");
}
}
