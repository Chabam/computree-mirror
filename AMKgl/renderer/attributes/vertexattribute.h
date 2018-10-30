#ifndef VERTEXATTRIBUTE
#define VERTEXATTRIBUTE

#include <string>
#include <QtOpenGL>

/**
 * @brief Structure to save custom vertex attributes
 */
struct VertexAttribute {
    std::string name;
    GLint tupleSize;
    GLenum type;
    GLboolean normalized;
    GLsizei stride;
    size_t sizeOfOneValueInByte;
    size_t arraySize;
    GLvoid *pointer;

    uint computeUniqueKey() const {
        return qHash(QString::fromStdString(name));
    }
};
#endif // VERTEXATTRIBUTE

