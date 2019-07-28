#ifndef OPENGLINFO_H
#define OPENGLINFO_H

#include <QtOpenGL>

#define OPENGL_INFO OpenGlInfo::staticInstance()

class OpenGlInfo
{
public:
    static OpenGlInfo* staticInstance() {
        if(UNIQUE_INSTANCE == nullptr)
            UNIQUE_INSTANCE = new OpenGlInfo();

        return UNIQUE_INSTANCE;
    }

    void init();

    GLint getMaxStride() const;
    GLuint getTotalMemoryAvailableInKByte() const;
    GLint getMaxTextureSizeInByte() const;

    static bool staticCheckOpenglVersionMinimumVersion(const QOpenGLContext* context, int majorV, int minorV);
private:
    struct Info {
        GLint   maxStride = 1;
        GLint   totalMemAvailable = 0;
    };

    static OpenGlInfo*  UNIQUE_INSTANCE;
    bool                m_initialized;
    Info                m_info;

    OpenGlInfo();
};

#endif // OPENGLINFO_H
