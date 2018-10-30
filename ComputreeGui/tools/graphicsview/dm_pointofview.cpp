/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : MichaÃ«l KREBS (ARTS/ENSAM)

 This file is part of Computree version 2.0.

 Computree is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Computree is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Computree.  If not, see <http://www.gnu.org/licenses/>.

*****************************************************************************/


#include "dm_pointofview.h"
#include <QStringList>

#include "Eigen/Core"

DM_PointOfView::DM_PointOfView()
{
}

DM_PointOfView::DM_PointOfView(QString name,
                               double cx, double cy, double cz,
                               double px, double py, double pz,
                               double q0, double q1, double q2, double q3)
{
    _name = name;
    _cx = cx;
    _cy = cy;
    _cz = cz;
    _px = px;
    _py = py;
    _pz = pz;
    _q0 = q0;
    _q1 = q1;
    _q2 = q2;
    _q3 = q3;
}

DM_PointOfView::DM_PointOfView(const DM_PointOfView &pof)
{
    _name = pof._name;
    _cx = pof._cx;
    _cy = pof._cy;
    _cz = pof._cz;
    _px = pof._px;
    _py = pof._py;
    _pz = pof._pz;
    _q0 = pof._q0;
    _q1 = pof._q1;
    _q2 = pof._q2;
    _q3 = pof._q3;
}

double DM_PointOfView::distanceFromSceneCenter() const
{
    return (Eigen::Vector3d(cx(), cy(), cz()) - Eigen::Vector3d(px(), py(), pz())).norm();
}

QString DM_PointOfView::toString() const
{
    return QString("%1 (cx:%2 cy:%3 cz:%4 rx:%5 ry:%6 rz:%7 rw:%8 d:%9)").arg(name())
                                                        .arg(cx())
                                                        .arg(cy())
                                                        .arg(cz())
                                                        .arg(q0())
                                                        .arg(q1())
                                                        .arg(q2())
                                                        .arg(q3())
                                                        .arg(distanceFromSceneCenter());
}

QString DM_PointOfView::toSaveString() const
{
    return QString("v1|%1|%2;%3;%4;%5;%6;%7;%8;%9;%10;%11").arg(name())
                                             .arg(cx())
                                             .arg(cy())
                                             .arg(cz())
                                             .arg(q0())
                                             .arg(q1())
                                             .arg(q2())
                                             .arg(q3())
                                             .arg(px())
                                             .arg(py())
                                             .arg(pz());
}

DM_PointOfView DM_PointOfView::loadFromString(QString saveString, bool &ok)
{
    ok = false;

    QStringList fList = saveString.split("|");

    if(fList.size() == 3)
    {
        QStringList sList = fList.at(2).split(";");
        int size = sList.size();

        if(size == 10)
        {
            ok = true;

            QString name = fList.at(1);
            double cx = sList.at(0).toDouble();
            double cy = sList.at(1).toDouble();
            double cz = sList.at(2).toDouble();
            double q0 = sList.at(3).toDouble();
            double q1 = sList.at(4).toDouble();
            double q2 = sList.at(5).toDouble();
            double q3 = sList.at(6).toDouble();
            double px = sList.at(7).toDouble();
            double py = sList.at(8).toDouble();
            double pz = sList.at(9).toDouble();

            return DM_PointOfView(name, cx, cy, cz, px, py, pz, q0, q1, q2, q3);
        }
    }

    return DM_PointOfView();
}

bool DM_PointOfView::operator==(const DM_PointOfView &other) const
{
    return (_name == other._name)
            &&(_cx == other._cx)
            && (_cy == other._cy)
            && (_cz == other._cz)
            && (_q0 == other._q0)
            && (_q1 == other._q1)
            && (_q2 == other._q2)
            && (_q3 == other._q3)
            && (_px == other._px)
            && (_py == other._py)
            && (_pz == other._pz);
}
