#ifndef CT_NORMAL_H
#define CT_NORMAL_H

#include "ctlibclouds_global.h"

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

class CT_Normal : public Eigen::Vector4f
{
public:

    CT_Normal(void):Eigen::Vector4f() {}
    // This constructor allows you to construct CT_Normal from Eigen expressions
    template<typename OtherDerived>
    CT_Normal(const Eigen::MatrixBase<OtherDerived>& other)
        : Eigen::Vector4f(other)
    { }
    // This method allows you to assign Eigen expressions to CT_Normal
    template<typename OtherDerived>
    CT_Normal& operator=(const Eigen::MatrixBase <OtherDerived>& other)
    {
        this->Eigen::Vector4f::operator=(other);
        return *this;
    }

    inline CT_Normal(const float& x,
                     const float& y,
                     const float& z,
                     const float& curvature) : Eigen::Vector4f(x, y, z, curvature)
    {
    }


    inline void set(const float& x,
                    const float& y,
                    const float& z,
                    const float& curvature = 1) {
        (*this)(0) = x;
        (*this)(1) = y;
        (*this)(2) = z;
        (*this)(3) = curvature;
    }
};

#endif // CT_NORMAL_H
