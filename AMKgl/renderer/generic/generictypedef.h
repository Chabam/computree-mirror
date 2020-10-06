#ifndef BASICTYPEDEF_H
#define BASICTYPEDEF_H

#if defined(_WIN32) && defined(_MSC_VER) // Microsoft Visual Studio Compiler
#elif (defined(__linux__) || defined(_WIN32)) && defined(__GNUC__) // GNU Compiler (gcc,g++) for Linux, Unix, and MinGW (Windows)
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#if __GNUC__ > 7
#pragma GCC diagnostic ignored "-Wdeprecated-copy"
#else
#pragma GCC diagnostic ignored "-Wattributes" // See: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=89325
#endif
#pragma GCC diagnostic ignored "-Wint-in-bool-context"
#elif defined(__APPLE__) // Clang Compiler (Apple)
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Wint-in-bool-context"
#endif
#include "Eigen/Core"
#if defined(_WIN32) && defined(_MSC_VER)
#elif (defined(__linux__) || defined(_WIN32)) && defined(__GNUC__)
#pragma GCC diagnostic warning "-Wall"
#pragma GCC diagnostic warning "-Wextra"
#if __GNUC__ > 7
#pragma GCC diagnostic warning "-Wdeprecated-copy"
#else
#pragma GCC diagnostic warning "-Wattributes" // See: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=89325
#endif
#pragma GCC diagnostic warning "-Wint-in-bool-context"
#elif defined(__APPLE__)
#pragma GCC diagnostic warning "-Wall"
#pragma GCC diagnostic warning "-Wextra"
#pragma GCC diagnostic warning "-Wint-in-bool-context"
#endif

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
