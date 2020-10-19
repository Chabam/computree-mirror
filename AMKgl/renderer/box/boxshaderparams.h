#ifndef BOXSHADERPARAMS_H
#define BOXSHADERPARAMS_H

#include <Eigen/Dense>

#include <QtOpenGL/QtOpenGL>

struct BoxShaderParams {
    Eigen::Vector3f lengthDirection;
    Eigen::Vector3f widthDirection;
    Eigen::Vector3f center;
    float           height;

    static inline GLenum staticGetGLMode() { return GL_QUADS; }

    static inline BoxShaderParams staticConstructFromValues(const double& cx, const double& cy, const double &cz,
                                                            const float& width, const float& height, const float& length,
                                                            const Eigen::Vector3f& widthAxisDirection,
                                                            const Eigen::Vector3f& heightAxisDirection,
                                                            const Eigen::Vector3d& offset,
                                                            const double divisor = 2.0) {
        Eigen::Vector3f wDirection = widthAxisDirection.normalized();
        Eigen::Vector3f hDirection = heightAxisDirection.normalized();
        Eigen::Vector3f lDirection = wDirection.cross(hDirection);

        BoxShaderParams params;
        params.lengthDirection = (lDirection * length/divisor);
        params.widthDirection = (wDirection * width/divisor);
        params.center = Eigen::Vector3f(cx - offset[0], cy - offset[1], cz - offset[2]);
        params.height = height/divisor;

        return params;
    }

    inline void getMatrix4f(Eigen::Matrix4f &matrix4f) const {
        // Tangent  = +X axis = widthDirection
        // Normal   = +Y axis = heightDirection
        // Binormal = +Z axis = lengthDirection

        Eigen::Vector3f heightDirection = widthDirection.cross(lengthDirection).normalized() * height;

        matrix4f = Eigen::Matrix4f::Identity();
        matrix4f.block(0,3,3,1) = center;
        matrix4f(0,0) = widthDirection[0];      // Xx
        matrix4f(1,0) = widthDirection[1];      // Xy
        matrix4f(2,0) = widthDirection[2];      // Xz
        matrix4f(0,1) = heightDirection[0];     // Yx
        matrix4f(1,1) = heightDirection[1];     // Yy
        matrix4f(2,1) = heightDirection[2];     // Yz
        matrix4f(0,2) = lengthDirection[0];     // Zx
        matrix4f(1,2) = lengthDirection[1];     // Zy
        matrix4f(2,2) = lengthDirection[2];     // Zz
    }

    inline void transform(const Eigen::Matrix4f& trMatrix) {
#ifdef AMKGL_NO_TODO_WARNINGS
    Q_UNUSED(trMatrix)
    // TODO
#endif
    }

    inline void translate(const Eigen::Vector3f& translation) {
        center[0] += translation[0];
        center[1] += translation[1];
        center[2] += translation[2];
    }

    inline void rotate(const Eigen::AngleAxis<float>& rotation) {
#ifdef AMKGL_NO_TODO_WARNINGS
    Q_UNUSED(rotation)
    // TODO
#endif
    }

    inline void rotate(const Eigen::Quaternion<float>& rotation) {
#ifdef AMKGL_NO_TODO_WARNINGS
    Q_UNUSED(rotation)
    // TODO
#endif
    }

    inline void scale(const Eigen::Vector3f& scaling) {
        widthDirection.array() *= scaling[0];
        height *= scaling[1];
        lengthDirection.array() *= scaling[2];
    }

    inline float& operator()(const int &index) {
        if(index == 0)
            return lengthDirection(0);
        else if(index == 1)
            return widthDirection(0);
        else if(index == 2)
            return center(0);

        return height;
    }

    inline const float& operator()(const int &index) const {
        if(index == 0)
            return lengthDirection(0);
        else if(index == 1)
            return widthDirection(0);
        else if(index == 2)
            return center(0);

        return height;
    }
};

#endif // BOXSHADERPARAMS_H
