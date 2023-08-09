/****************************************************************************

 Copyright (C) 2010-2015 the Office National des Forêts (ONF), France
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Alexandre Piboule (ONF)

 This file is part of PluginShared library 2.0.

 PluginShared is free library: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 PluginShared is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with PluginShared.  If not, see <http://www.gnu.org/licenses/lgpl.html>.

*****************************************************************************/

#ifndef CT_DELAUNAY2DTRIANGULATION_H
#define CT_DELAUNAY2DTRIANGULATION_H

#include <boost/polygon/voronoi.hpp>

#include "ct_delaunay2dvertex.h"


///**
// * DelaunayTriangulation - Delaunay triangulation using boost library.
// *
// * @author A. Piboule - August 2023
// */
//class CTLIBSTRUCTUREADDON_EXPORT CT_Delaunay2DTriangulation
//{

//public:
//    CT_Delaunay2DTriangulation ();
//    ~CT_Delaunay2DTriangulation ();

//    void clear(bool clearToInsert = true);


//    // Construct the 4 corners of the triangulation, and the 2 first triangles
//    void init (double x1, double y1, double x2, double y2);


//    // idem but corners are calculated from others points
//    bool init ();


//    // Accesors:

//    // gives all vertices successfully inserted in the triangulation
//    inline const QList<CT_Delaunay2DVertex*> &getInsertedVertices () {return _insertedVertices;}

//    // gives all out of bounds vertices (which have NOT been inserted)
//    inline const QList<CT_Delaunay2DVertex*> &getOutOfBoundsVertices () {return _outOfBoundsVertices;}

//    // the returned hashMap gives for each (not inserted) duplicated vertex (in term of position)
//    // a link to the same position vertex which has been inserted.
//    inline const QMap<CT_Delaunay2DVertex*, CT_Delaunay2DVertex*> &getDuplicatePositions () {return _duplicatePositions;}

//    // gives the 4 bounds conners which have been added in init (),
//    // these 4 points are NOT in insertedVertex List
//    inline const QVector<CT_Delaunay2DVertex*> &getCorners () {return _corners;}

//    // gives the triangle used to enter in triangulation to find "to insert" vertex triangle
//    const CT_DelaunayTriangle* getRefTriangle () {return _refTriangle;}

//    inline bool isInitialized () {return _initialized;}



//    // gives the up to date list of all triangles
//    const QList<CT_DelaunayTriangle*> &getTriangles ();

//    // gives the insertedVertices list with up to date computed neighbors for each vertex
//    const QList<CT_Delaunay2DVertex*> &getVerticesNeighbors ();

//    // gives the insertedVertices list with up to date computed Voronoi Diagram for each vertex
//    // Voronoi diagram consist for each vertex, in a sorted list of DelaunayVoroVertex defining the Voronoi Polygon for this vertex
//    const QList<CT_Delaunay2DVertex*> &getVoronoiDiagram ();


//    // gives the same thing that getVoronoiDiagram
//    // but without voronoi polygon of corners neighbors (to avoid border effects)

//    const QList<CT_Delaunay2DVertex*> &getCleanVoronoiDiagram ();

//    // gives the outline defined by the triangulation (corners excluded)
//    const CT_DelaunayOutline &getOutline();

//    QMultiMap<CT_Delaunay2DVertex*, CT_Delaunay2DVertex*> getEdges();

//    // compute for each vertex its neighbors
//    const QList<CT_Delaunay2DVertex*> &computeVerticesNeighbors ();


//    // create an Map : the keys are inserted vertex
//    //					   the values are voronoi's polygons for these inserted vertex
//    const QList<CT_Delaunay2DVertex*> &computeVoronoiDiagram ();


//    // compute the Outline of the triangulation
//    const CT_DelaunayOutline &computeOutline ();


//    // add a DelaunayVertex to the toInsert list
//    CT_Delaunay2DVertex* addVertex (Eigen::Vector3d *data, bool deleteData);

//    // add all vertices present in toInsert list to the triangulation
//    bool doInsertion (bool sort = false, double cellSize = 0);

//    void updateCornersZValues();

//    CT_Delaunay2DTriangulation *copy();

//    CT_DelaunayTriangle *findTriangleContainingPoint(double x, double y, CT_DelaunayTriangle *refTriangle = nullptr);

//    CT_DelaunayTriangle *getZCoordForXY(double x, double y, double &outZ, CT_DelaunayTriangle *refTriangle = nullptr, bool cornersIncluded = true);

//    QList<CT_Delaunay2DVertex *> getNeighboursForCoordinates(double x, double y, bool cornersIncluded = true);

//    bool isCorner(CT_Delaunay2DVertex *vertex) const;

//    inline double getMinX() const {return _minx;}
//    inline double getMinY() const {return _miny;}
//    inline double getMaxX() const {return _maxx;}
//    inline double getMaxY() const {return _maxy;}

//private:
//    QList<CT_Delaunay2DVertex*> _toInsert; // working list of to be inserted vertex


//    QList<CT_Delaunay2DVertex*>                  _insertedVertices;     // list of inserted vertices
//    QList<CT_Delaunay2DVertex*>                  _outOfBoundsVertices;  // list of not inserted vertices because a position out of bounds
//    QMap<CT_Delaunay2DVertex*, CT_Delaunay2DVertex*>   _duplicatePositions;   // list of not inserted vertices because an already existing position
//    QList<CT_DelaunayTriangle*>                _triangles;            // list of all triangles of the triangulation (created in compute ())

//    QVector<CT_Delaunay2DVertex*>                _corners;              // list of the 4 created corners (not present in insertedVertices list)

//    double _minx; // working bounds of the triangulation
//    double _miny;
//    double _maxx;
//    double _maxy;

//    CT_DelaunayTriangle* _refTriangle; // actual reference triangle
//                                    // used as start point to find the inserted vertex triangle

//    CT_DelaunayOutline      _outline; // outline defined by the triangulation

//    bool _initialized; // check if bounds have been set
//    bool _neighborsComputed; // check if neighbors are up to date
//    bool _voronoiDiagramComputed; // check if voronoi diagram is up to date





//    static bool compareDelaunayVertices(const CT_Delaunay2DVertex* p1, const CT_Delaunay2DVertex* p2)
//    {
//         return (qFuzzyCompare(p1->x(), p2->x()) && p1->y() < p2->y()) ||  (p1->x() < p2->x());
//    }

//    static double cross(const CT_Delaunay2DVertex* O, const CT_Delaunay2DVertex* A, const CT_Delaunay2DVertex* B);


//};


#endif // CT_DELAUNAY2DTRIANGULATION_H
