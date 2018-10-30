#ifndef DUMMYCOLOR_H
#define DUMMYCOLOR_H

#include <QtOpenGL>
#include "Eigen/Core"

class DummyColor : public Eigen::Matrix<GLubyte, 4, 1>
{
};

#endif // DUMMYCOLOR_H
