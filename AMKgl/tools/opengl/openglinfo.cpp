#include "openglinfo.h"
#include <qopenglext.h>

OpenGlInfo* OpenGlInfo::UNIQUE_INSTANCE = nullptr;

// NVIDIA
#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048

// ATI/AMD
#define VBO_FREE_MEMORY_ATI          0x87FB

// INTEL

GLint OpenGlInfo::getMaxStride() const
{
    return m_info.maxStride;
}

GLuint OpenGlInfo::getTotalMemoryAvailableInKByte() const
{
    return m_info.totalMemAvailable;
}

GLint OpenGlInfo::getMaxTextureSizeInByte() const
{
    GLint value;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &value);

    return value;
}

bool OpenGlInfo::staticCheckOpenglVersionMinimumVersion(const QOpenGLContext *context, int majorV, int minorV)
{
    Q_ASSERT(context != nullptr);

    return (context->format().majorVersion() > majorV) || ((context->format().majorVersion() == majorV) && (context->format().minorVersion() >= minorV));
}

OpenGlInfo::OpenGlInfo()
{
    m_initialized = false;
}

void OpenGlInfo::init()
{
    if(!m_initialized) {
        m_initialized = true;

        glGetIntegerv(GL_MAX_VERTEX_ATTRIB_STRIDE, &m_info.maxStride);

        // NVIDIA memory
        glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX,
                      &m_info.totalMemAvailable);

        if(glGetError() != GL_NO_ERROR) {

            // ATI memory
            GLint param[4];

            glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX,
                          &param[0]);

            if(glGetError() != GL_NO_ERROR)
                m_info.totalMemAvailable = param[2];
        }
    }
}
