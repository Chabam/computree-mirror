#ifndef BASICTYPEDEF_H
#define BASICTYPEDEF_H

#include "Eigen/Core"

#include "elementinfo.h"
#include "renderer/object/iobjectcloud.h"
#include "definecolor.h"
#include "definenormal.h"

namespace Basic {

    typedef Eigen::Vector3f                             LocalVertex;
    typedef AMKgl::GlobalColor                          LocalColor;
    typedef ElementInfo                                 LocalInfo;
    typedef AMKgl::GlobalNormal                         LocalNormal;
    typedef GLuint                                      LocalIndex;

    typedef std::vector<LocalVertex>                    VertexArray;
    typedef std::vector<LocalColor>                     ColorArray;
    typedef std::vector<LocalInfo>                      InfoArray;
    typedef std::vector<LocalNormal>                    NormalArray;

    typedef ObjectCloud< ColorArray >                   ColorCloud;
    typedef ObjectCloud< InfoArray >                    InfoCloud;
    typedef ObjectCloud< NormalArray >                  NormalCloud;

    typedef AbstractObjectCloud< LocalColor >           AbstractColorCloud;
    typedef AbstractObjectCloud< LocalInfo >            AbstractInfoCloud;
    typedef AbstractObjectCloud< LocalNormal >          AbstractNormalCloud;
}

#endif // BASICTYPEDEF_H
