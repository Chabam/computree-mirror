#include "offsetpreparator.h"

#include "drawinfo.h"

#include <QMatrix4x4>

OffsetPreparator::OffsetPreparator()
{
}

void OffsetPreparator::staticPushMatrix(const Eigen::Vector3d &offset, DrawInfo &info)
{
    Eigen::Vector4d camTranslation = info.getCameraTranslation();

    camTranslation[0] = (offset[0] - camTranslation[0]);
    camTranslation[1] = (offset[1] - camTranslation[1]);
    camTranslation[2] = (offset[2] - camTranslation[2]);

    GLdouble modelViewMatrix[16];
    info.getModelViewMatrix(modelViewMatrix);

    // convert to qt matrix
    QMatrix4x4 matrix(modelViewMatrix[0], modelViewMatrix[4], modelViewMatrix[8], modelViewMatrix[12],
                      modelViewMatrix[1], modelViewMatrix[5], modelViewMatrix[9], modelViewMatrix[13],
                      modelViewMatrix[2], modelViewMatrix[6], modelViewMatrix[10], modelViewMatrix[14],
                      modelViewMatrix[3], modelViewMatrix[7], modelViewMatrix[11], modelViewMatrix[15]);

    matrix(0,3) = 0;
    matrix(1,3) = 0;
    matrix(2,3) = 0;
    matrix(3,3) = 1;

    glPushMatrix();
    glMultMatrixf(matrix.data());
    glTranslatef(camTranslation[0], camTranslation[1], camTranslation[2]);
}

void OffsetPreparator::staticPopMatrix()
{
    glPopMatrix();
}
