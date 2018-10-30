#include "intersectionchecker.h"

#include "tools/math.h"

IntersectionChecker::IntersectionChecker()
{
}

bool IntersectionChecker::staticDoesAPlaneIntersectWithAPlane(const Eigen::Vector4d &plane1,
                                                              const Eigen::Vector4d &plane2,
                                                              Eigen::Vector3d intersectionLine[])
{
    Eigen::Vector3d plane13D = Eigen::Vector3d(plane1(0), plane1(1), plane1(2));
    Eigen::Vector3d plane23D = Eigen::Vector3d(plane2(0), plane2(1), plane2(2));
    Eigen::Vector3d dir = plane13D.cross(plane23D) ;

    double dnorm = dir.norm();

    /* If planes are parallel then fail */

    if ( dnorm < 1e-6 )
    return false;

    if(intersectionLine != NULL) {
        /* Determine intersection point with the best suited coordinate plane. */

        double abs ;
        double maxabs = fabs(dir[0]);
        int index = 0;

        if ((abs = fabs(dir[1])) > maxabs) { maxabs = abs ; index = 1; }
        if ((abs = fabs(dir[2])) > maxabs) { maxabs = abs ; index = 2; }

        switch ( index )
        {
            case 0:
              intersectionLine[0] = Eigen::Vector3d(0,
                     (plane1[2] * plane2[3] - plane2[2] * plane1[3]) / dir[0],
                     (plane2[1] * plane1[3] - plane1[1] * plane2[3]) / dir[0] );
              break;
            case 1:
              intersectionLine[0] = Eigen::Vector3d((plane2[2] * plane1[3] - plane1[2] * plane2[3]) / dir[1],
                     0,
                     (plane1[0] * plane2[3] - plane2[0] * plane1[3]) / dir[1] );
              break;
            case 2:
              intersectionLine[0] = Eigen::Vector3d((plane1[1] * plane2[3] - plane2[1] * plane1[3]) / dir[2],
                     (plane2[0] * plane1[3] - plane1[0] * plane2[3]) / dir[2],
                     0 );
              break;
            default: return false ;  /* Impossible */
        }

        /* Normalize the direction */
        intersectionLine[1] = dir;
        intersectionLine[1] *= (1.0 / dnorm);
    }

    return true;
}

bool IntersectionChecker::staticDoesAFilledTriangleIntersectWithAPlane(const Eigen::Vector3d &p1, const Eigen::Vector3d &p2, const Eigen::Vector3d &p3, const Eigen::Vector3d &v0, const Eigen::Vector3d &v1, const Eigen::Vector3d &v2, const Eigen::Vector3d &v3, Eigen::Vector3d intersectionLine[], bool infinitePlane)
{
    Eigen::Vector3d* intersectionPoint = NULL;

    if(intersectionLine != NULL)
        intersectionPoint = &intersectionLine[0];

    bool ok = staticDoesASegmentIntersectWithAPlane(p1, p2, v0, v1, v2, v3, intersectionPoint, true, infinitePlane);

    if(ok) {

        if(intersectionLine != NULL)
            intersectionPoint = &intersectionLine[1];

        ok = staticDoesASegmentIntersectWithAPlane(p2, p3, v0, v1, v2, v3, intersectionPoint, true, infinitePlane);

        if(ok)
            return true;
    }

    ok = staticDoesASegmentIntersectWithAPlane(p3, p1, v0, v1, v2, v3, intersectionPoint, true, infinitePlane);

    if(ok)
        intersectionLine[1] = intersectionLine[1]-intersectionLine[0]; // second point is direction

    return ok;
}

bool IntersectionChecker::staticDoesALineIntersectWithACylinder(const Eigen::Vector3d& p1,
                                                                const Eigen::Vector3d& p2,
                                                                Eigen::Vector3d intersectionPoint[])
{
    double t0, t1;

    if(intersectionPoint != NULL)
        intersectionPoint[1] = Eigen::Vector3d(0,0,0);

    const Eigen::Vector3d lineDirection = p2-p1;
    const Eigen::Vector3d lineStart = p1-(lineDirection*10000);

    // a=xD2+yD2, b=2xExD+2yEyD, and c=xE2+yE2-1.
    double a = std::pow(lineDirection[0], 2) + std::pow(lineDirection[2], 2);

    double b = 2 * lineStart[0] * lineDirection[0] + 2 * lineStart[2] * lineDirection[2];

    double c = std::pow(lineStart[0], 2) + std::pow(lineStart[2], 2) - 1;

    double b24ac = b*b - 4*a*c;
    if (b24ac<0)
        return false;

    double sqb24ac = sqrt(b24ac);
    t0 = (-b + sqb24ac) / (2 * a);
    t1 = (-b - sqb24ac) / (2 * a);

    if (t0>t1) {double tmp = t0;t0=t1;t1=tmp;}

    double y0 = lineStart[1] + t0 * lineDirection[1];
    double y1 = lineStart[1] + t1 * lineDirection[1];

    if(fabs(y0) > 0.5 && fabs(y1) > 0.5)
        return false;

    // hit the cap
    /*double th = t0 + (t1-t0) * (y0-0.5) / (y0-y1);

    if (th<=0)
        return false;

    if(intersectionPoint != NULL)
        intersectionPoint[0] = lineStart + (lineDirection*th);

    // hit the cylinder bit
    if (t0<=0)
        return false;

    if(intersectionPoint != NULL)
        intersectionPoint[1] = lineStart + (lineDirection*t0);

    return true;*/

    if (y0<-0.5)
    {
        if (y1<-0.5)
            return false;
        else
        {
            // hit the cap
            double th = t0 + (t1-t0) * (y0+0.5) / (y0-y1);
            if (th<=0) return false;

            if(intersectionPoint != NULL)
                intersectionPoint[0] = lineStart + (lineDirection*th);

            return true;
        }
    }
    else if (y0>=-0.5 && y0<=0.5)
    {
        // hit the cylinder bit
        if (t0<=0) return false;

        if(intersectionPoint != NULL)
            intersectionPoint[0] = lineStart + (lineDirection*t0);

        return true;
    }
    else if (y0>0.5)
    {
        if (y1>0.5)
            return false;
        else
        {
            // hit the cap
            double th = t0 + (t1-t0) * (y0-0.5) / (y0-y1);
            if (th<=0) return false;

            if(intersectionPoint != NULL)
                intersectionPoint[0] = lineStart + (lineDirection*th);

            return true;
        }
    }

    return false;
}

bool IntersectionChecker::staticDoestASegmentIntersectAUnitCircle(bool filled, const Eigen::Vector3d &p1, const Eigen::Vector3d &p2, Eigen::Vector3d *closestP)
{
    if(!filled && (p1.norm() <= 1) && (p2.norm() <= 1))
        return false;

    Eigen::Vector3d closest;
    Math::getClosestPointBetweenPointAndSegment(Eigen::Vector3d(0,0,0), p1, p2, closest);

    if(closestP != NULL)
        *closestP = closest;

    const double val = closest.norm();
    return val <= 1.0;
}
