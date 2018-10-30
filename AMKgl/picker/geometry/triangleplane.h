#ifndef TRIANGLEPLANE_H
#define TRIANGLEPLANE_H

#include "plane.h"

template<typename Scalar>
struct TrianglePlaneT
{
    PlaneT<Scalar> planes[3];

    template<typename NewScalar>
    TrianglePlaneT<NewScalar> cast() const
    {
        TrianglePlaneT<NewScalar> thisCast;
        thisCast.planes[0] = planes[0].template cast<NewScalar>();
        thisCast.planes[1] = planes[1].template cast<NewScalar>();
        thisCast.planes[2] = planes[2].template cast<NewScalar>();

        return thisCast;
    }
};

typedef TrianglePlaneT<double>  TrianglePlane;
typedef TrianglePlaneT<float>   TrianglePlaneF;

#endif // TRIANGLEPLANE_H
