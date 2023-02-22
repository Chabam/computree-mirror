#ifndef GENERICGLRENDERER_H
#define GENERICGLRENDERER_H

#include <QtOpenGL/QtOpenGL>

#include "renderer/tools/flagspropertymanager.h"

namespace GenericGLRendererNM {
    /**
     * @brief Use this class if you don't have an array of indices
     */
    class DirectIndices {
    public:

        template<typename IndexType>
        inline const GLuint& operator[](const IndexType& i) const {
            const_cast<GLuint&>(m_currentIndex) = (GLuint)i;
            return m_currentIndex;
        }

    private:
        GLuint m_currentIndex;
    };

    /**
     * @brief Use this class if you have only the pointer to the first index
     */
    class IndicesFromPointer {
    public:
        IndicesFromPointer(GLuint* indicesArray) : m_indices(indicesArray) {}

        inline const GLuint& operator[](const size_t& i) const { return m_indices[i]; }

    private:
        GLuint* m_indices;
    };
}

/**
 * @brief Use this class to render element with opengl
 */
class GenericGLRenderer
{
private:
    template<typename InfoT, typename ColorT>
    static void staticInlineSetFlagColorWhenHasColor(const size_t& localIndex,
                                                     ColorT* fc,
                                                     const InfoT& info,
                                                     const FlagsPropertyManager& fpM)
    {
        if(const ObjectsFlagsTool::Flag devFlag = ObjectsFlagsTool::staticFirstDevelopperFlagSet(info)) { // if at least one developper's flag is set
            const QColor color = fpM.getProperty(devFlag).objectColorWhenFlagIsON;
            glColor4f(color.redF(), color.greenF(), color.blueF(), color.alphaF());
        } else
            glColor4ubv(&(fc[localIndex][0]));
    }

    template<typename InfoT, typename ColorT>
    static void staticInlineSetColorWhenHasColor(const size_t& localIndex,
                                                       ColorT* fc,
                                                       const InfoT& info,
                                                       const FlagsPropertyManager& fpM,
                                                       float selectionColor[4])
    {
        if(ObjectsFlagsTool::staticIsObjectSelected(info))
            glColor4fv(selectionColor);
        else if(const ObjectsFlagsTool::Flag devFlag = ObjectsFlagsTool::staticFirstDevelopperFlagSet(info)) { // if at least one developper's flag is set
            const QColor color = fpM.getProperty(devFlag).objectColorWhenFlagIsON;
            glColor4f(color.redF(), color.greenF(), color.blueF(), color.alphaF());
        } else
            glColor4ubv(&(fc[localIndex][0]));
    }

    template<typename InfoT>
    static void staticInlineSetColorWhenNoColor(const InfoT& info,
                                                      const FlagsPropertyManager& fpM,
                                                      float selectionColor[4],
                                                      float currentColor[4],
                                                      bool& modified)
    {
        if(ObjectsFlagsTool::staticIsObjectSelected(info)) {
            glColor4fv(selectionColor);
            modified = true;
        } else if(const ObjectsFlagsTool::Flag devFlag = ObjectsFlagsTool::staticFirstDevelopperFlagSet(info)) { // if at least one developper's flag is set
            const QColor color = fpM.getProperty(devFlag).objectColorWhenFlagIsON;
            glColor4f(color.redF(), color.greenF(), color.blueF(), color.alphaF());
            modified = true;
        } else if(modified) {
            glColor4fv(currentColor);
            modified = false;
        }
    }

    template<typename VertexT, typename InfoT, typename ColorT, typename NormalT, typename GLuintIndexArray>
    static void staticInlineVertexColorNormal(const size_t& i,
                                              VertexT* fv,
                                              InfoT* fi,
                                              ColorT* fc,
                                              NormalT* fn,
                                              const FlagsPropertyManager& fpM,
                                              const GLuintIndexArray &indices,
                                              float selectionColor[4])
    {
        const GLuint &localIndex = indices[i];
        const InfoT& info = fi[localIndex];

        if(ObjectsFlagsTool::staticIsObjectVisible(info)) {

            staticInlineSetColorWhenHasColor(localIndex, fc, info, fpM, selectionColor);

            glNormal3fv(&(fn[localIndex][0]));
            glVertex3fv(&(fv[localIndex][0]));
        }
    }

    template<typename VertexT, typename InfoT, typename NormalT, typename GLuintIndexArray>
    static void staticInlineVertexNormal(const size_t& i,
                                         VertexT* fv,
                                         InfoT* fi,
                                         NormalT* fn,
                                         const FlagsPropertyManager& fpM,
                                         const GLuintIndexArray &indices,
                                         float selectionColor[4],
                                         float currentColor[4],
                                         bool& modified)
    {
        const GLuint &localIndex = indices[i];
        const InfoT& info = fi[localIndex];

        if(ObjectsFlagsTool::staticIsObjectVisible(info)) {

            staticInlineSetColorWhenNoColor(info, fpM, selectionColor, currentColor, modified);

            glNormal3fv(&(fn[localIndex][0]));
            glVertex3fv(&(fv[localIndex][0]));
        }
    }

    template<typename VertexT, typename InfoT, typename ColorT, typename GLuintIndexArray>
    static void staticInlineVertexColor(const size_t& i,
                                        VertexT* fv,
                                        InfoT* fi,
                                        ColorT* fc,
                                        const FlagsPropertyManager& fpM,
                                        const GLuintIndexArray &indices,
                                        float selectionColor[4])
    {
        const GLuint &localIndex = indices[i];
        const InfoT& info = fi[localIndex];

        if(ObjectsFlagsTool::staticIsObjectVisible(info)) {

            staticInlineSetColorWhenHasColor(localIndex, fc, info, fpM, selectionColor);

            glVertex3fv(&(fv[localIndex][0]));
        }
    }

    template<typename VertexT, typename InfoT, typename GLuintIndexArray>
    static void staticInlineVertex(const size_t& i,
                                   VertexT* fv,
                                   InfoT* fi,
                                   const FlagsPropertyManager& fpM,
                                   const GLuintIndexArray &indices,
                                   float selectionColor[4],
                                   float currentColor[4],
                                   bool& modified)
    {
        const GLuint &localIndex = indices[i];
        const InfoT& info = fi[localIndex];

        if(ObjectsFlagsTool::staticIsObjectVisible(info)) {
            staticInlineSetColorWhenNoColor(info, fpM, selectionColor, currentColor, modified);

            glVertex3fv(&(fv[localIndex][0]));
        }
    }

public:
    /**
     * @brief Render with call to basic opengl function
     * @param glMode : the opengl mode to use (GL_POINTS, GL_LINES, etc...)
     * @param fv : the first vertex in the array, can not be nullptr !
     * @param fi : the first info in the array, can not be nullptr !
     * @param fc : the first color in the array, can be nullptr
     * @param fn : the first normal in the array, can be nullptr
     * @param fpM : the flags property manager
     * @param indices : array of indices to render
     * @param size : number of indices in the array to render
     * @param selectionColor : selection color to use
     * @param currentColor : current color used
     */
    template<typename VertexT, typename InfoT, typename ColorT, typename NormalT, typename GLuintIndexArray>
    static void staticBasicRender(GLenum glMode,
                                  VertexT* fv,
                                  InfoT* fi,
                                  ColorT* fc,
                                  NormalT* fn,
                                  const FlagsPropertyManager& fpM,
                                  const GLuintIndexArray &indices,
                                  const size_t &size,
                                  float selectionColor[4],
                                  float currentColor[4]) {

        Q_ASSERT(fv != nullptr);
        if (fv == nullptr) {qDebug() << "GenericGLRenderer::staticBasicRender" << ", " <<  "fv == nullptr"; return;}

        Q_ASSERT(fi != nullptr);
        if (fi == nullptr) {qDebug() << "GenericGLRenderer::staticBasicRender" << ", " <<  "fi == nullptr"; return;}

        glColor4fv(currentColor);

        if(fn != nullptr) {
            if(fc != nullptr) {
                // vertex + color + normal
                glBegin(glMode);
                for(size_t i=0; i<size; ++i) {
                    staticInlineVertexColorNormal(i, fv, fi, fc, fn, fpM, indices, selectionColor);
                }
                glEnd();
                glColor4fv(currentColor);
                return;
            }

            bool modified = false;

            // vertex + normal
            glBegin(glMode);
            for(size_t i=0; i<size; ++i) {
                staticInlineVertexNormal(i, fv, fi, fn, fpM, indices, selectionColor, currentColor, modified);
            }
            glEnd();
            glColor4fv(currentColor);
            return;
        }

        if(fc != nullptr) {
            // vertex + color
            glBegin(glMode);
            for(size_t i=0; i<size; ++i) {
                staticInlineVertexColor(i, fv, fi, fc, fpM, indices, selectionColor);
            }
            glEnd();
            glColor4fv(currentColor);
            return;
        }

        bool modified = false;

        // vertex
        glBegin(glMode);
        for(size_t i=0; i<size; ++i) {
            staticInlineVertex(i, fv, fi, fpM, indices, selectionColor, currentColor, modified);

        }
        glEnd();
        glColor4fv(currentColor);
    }

    /**
     * @brief Fast render with call to basic opengl function (don't render all indices)
     * @param glMode : the opengl mode to use (GL_POINTS, GL_LINES, etc...)
     * @param fv : the first vertex in the array, can not be nullptr !
     * @param fi : the first info in the array, can not be nullptr !
     * @param fc : the first color in the array, can be nullptr
     * @param fn : the first normal in the array, can be nullptr
     * @param indices : array of indices to render
     * @param size : number of indices in the array to render
     * @param stride : jump to next index
     * @param selectionColor : selection color to use
     * @param currentColor : current color used
     */
    template<typename VertexT, typename InfoT, typename ColorT, typename NormalT, typename GLuintIndexArray>
    static void staticBasicFastRender(GLenum glMode,
                                      VertexT* fv,
                                      InfoT* fi,
                                      ColorT* fc,
                                      NormalT* fn,
                                      const FlagsPropertyManager& fpM,
                                      const GLuintIndexArray &indices,
                                      const size_t &size,
                                      const size_t &stride,
                                      float selectionColor[4],
                                      float currentColor[4]) {

        Q_ASSERT(fv != nullptr);
        if (fv == nullptr) {qDebug() << "GenericGLRenderer::staticBasicFastRender" << ", " <<  "fv == nullptr"; return;}

        Q_ASSERT(fi != nullptr);
        if (fi == nullptr) {qDebug() << "GenericGLRenderer::staticBasicFastRender" << ", " <<  "fi == nullptr"; return;}

        glColor4fv(currentColor);

        if(fn != nullptr) {
            if(fc != nullptr) {
                // vertex + color + normal
                glBegin(glMode);
                for(size_t i=0; i<size; i += stride) {
                    staticInlineVertexColorNormal(i, fv, fi, fc, fn, fpM, indices, selectionColor);
                }
                glEnd();
                glColor4fv(currentColor);
                return;
            }

            bool modified = false;

            // vertex + normal
            glBegin(glMode);
            for(size_t i=0; i<size; i += stride) {
                staticInlineVertexNormal(i, fv, fi, fn, fpM, indices, selectionColor, currentColor, modified);
            }
            glEnd();
            glColor4fv(currentColor);
            return;
        }

        if(fc != nullptr) {
            // vertex + color
            glBegin(glMode);
            for(size_t i=0; i<size; i += stride) {
                staticInlineVertexColor(i, fv, fi, fc, fpM, indices, selectionColor);
            }
            glEnd();
            glColor4fv(currentColor);
            return;
        }

        bool modified = false;

        // vertex
        glBegin(glMode);
        for(size_t i=0; i<size; i += stride) {
            staticInlineVertex(i, fv, fi, fpM, indices, selectionColor, currentColor, modified);
        }
        glEnd();
        glColor4fv(currentColor);
    }

    /**
     * @brief Render objects (triangles, lines, etc...) with call to basic opengl function
     * @param glMode : the opengl mode to use (GL_POINTS, GL_LINES, etc...)
     * @param fv : the first vertex in the array, can not be nullptr !
     * @param ofi : the first info of objects in the array, can not be nullptr !
     * @param ofc : the first color of objects in the array, can be nullptr
     * @param ofn : the first normal of objects in the array, can be nullptr
     * @param vertexIndices : array of indices of vertex to render
     * @param objectIndices : array of indices of object to use to get color, normal, info from array
     * @param nObjects : number of objects to render
     * @param selectionColor : selection color to use
     * @param currentColor : current color used
     */
    template<typename VertexT, typename InfoT, typename ColorT, typename NormalT, typename GLuintIndexArray>
    static void staticBasicObjectRender(GLenum glMode,
                                        VertexT* fv,
                                        InfoT* ofi,
                                        ColorT* ofc,
                                        NormalT* ofn,
                                        const FlagsPropertyManager& fpM,
                                        const GLuintIndexArray& vertexIndices,
                                        const GLuintIndexArray& objectIndices,
                                        const size_t& nObjects,
                                        float selectionColor[4],
                                        float currentColor[4]) {

        Q_ASSERT(fv != nullptr);
        if (fv == nullptr) {qDebug() << "GenericGLRenderer::staticBasicObjectRender" << ", " <<  "fv == nullptr"; return;}

        Q_ASSERT(ofi != nullptr);
        if (ofi == nullptr) {qDebug() << "GenericGLRenderer::staticBasicObjectRender" << ", " <<  "ofi == nullptr"; return;}

        quint8 nVertexPerObject = 0;

        if(glMode == GL_POINTS)
            nVertexPerObject = 1;
        else if(glMode == GL_LINES)
            nVertexPerObject = 2;
        else if(glMode == GL_TRIANGLES)
            nVertexPerObject = 3;

        if(nVertexPerObject == 0)
            return;

        quint8 j;
        size_t currentIndex = 0;
        bool modified = false;

        glColor4fv(currentColor);

        if(ofn != nullptr) {
            if(ofc != nullptr) {

                // vertex + color + normal + info
                glBegin(glMode);
                for(size_t i=0; i<nObjects; ++i) {

                    const GLuint& objectLocalIndex = objectIndices[i];
                    const InfoT& info = ofi[objectLocalIndex];

                    if(ObjectsFlagsTool::staticIsObjectVisible(info)) {

                        staticInlineSetColorWhenHasColor(objectLocalIndex, ofc, info, fpM, selectionColor);

                        glNormal3fv(&(ofn[objectLocalIndex][0]));

                        for(j=0; j<nVertexPerObject; ++j) {
                            const GLuint& vertexLocalIndex = vertexIndices[currentIndex++];
                            glVertex3fv(&(fv[vertexLocalIndex][0]));
                        }
                    } else {
                        currentIndex += nVertexPerObject;
                    }
                }
                glEnd();
                glColor4fv(currentColor);
                return;
            }

            // vertex + normal + info
            glBegin(glMode);
            for(size_t i=0; i<nObjects; ++i) {

                const GLuint& objectLocalIndex = objectIndices[i];
                const InfoT& info = ofi[objectLocalIndex];

                if(ObjectsFlagsTool::staticIsObjectVisible(info)) {

                    staticInlineSetColorWhenNoColor(info, fpM, selectionColor, currentColor, modified);

                    glNormal3fv(&(ofn[objectLocalIndex][0]));

                    for(j=0; j<nVertexPerObject; ++j) {
                        const GLuint& vertexLocalIndex = vertexIndices[currentIndex++];
                        glVertex3fv(&(fv[vertexLocalIndex][0]));
                    }
                } else {
                    currentIndex += nVertexPerObject;
                }
            }
            glEnd();
            glColor4fv(currentColor);
            return;
        }

        if(ofc != nullptr) {
            // vertex + color + info
            glBegin(glMode);
            for(size_t i=0; i<nObjects; ++i) {

                const GLuint& objectLocalIndex = objectIndices[i];
                const InfoT& info = ofi[objectLocalIndex];

                if(ObjectsFlagsTool::staticIsObjectVisible(info)) {

                    staticInlineSetColorWhenHasColor(objectLocalIndex, ofc, info, fpM, selectionColor);

                    for(j=0; j<nVertexPerObject; ++j) {
                        const GLuint& vertexLocalIndex = vertexIndices[currentIndex++];
                        glVertex3fv(&(fv[vertexLocalIndex][0]));
                    }
                } else {
                    currentIndex += nVertexPerObject;
                }
            }
            glEnd();
            glColor4fv(currentColor);
            return;
        }

        // vertex + info
        glBegin(glMode);
        for(size_t i=0; i<nObjects; ++i) {

            const GLuint& objectLocalIndex = objectIndices[i];
            const InfoT& info = ofi[objectLocalIndex];

            if(ObjectsFlagsTool::staticIsObjectVisible(info)) {

                staticInlineSetColorWhenNoColor(info, fpM, selectionColor, currentColor, modified);

                for(j=0; j<nVertexPerObject; ++j) {
                    const GLuint& vertexLocalIndex = vertexIndices[currentIndex++];
                    glVertex3fv(&(fv[vertexLocalIndex][0]));
                }
            } else {
                currentIndex += nVertexPerObject;
            }
        }
        glEnd();
        glColor4fv(currentColor);
    }

    /**
     * @brief Render objects (triangles, lines, etc...) with call to basic opengl function but use the vertex colors/normals attributes
     * @param glMode : the opengl mode to use (GL_POINTS, GL_LINES, etc...)
     * @param fv : the first vertex in the array, can not be nullptr !
     * @param ofi : the first info of objects in the array, can not be nullptr !
     * @param fc : the first color of vertexes in the array, can be nullptr
     * @param fn : the first normal of vertexes in the array, can be nullptr
     * @param vertexIndices : array of indices of vertex to render
     * @param objectIndices : array of indices of object to use to get color, normal, info from array
     * @param nObjects : number of objects to render
     * @param selectionColor : selection color to use
     * @param currentColor : current color used
     */
    template<typename VertexT, typename InfoT, typename ColorT, typename NormalT, typename GLuintIndexArray>
    static void staticBasicObjectVertexRender(GLenum glMode,
                                              VertexT* fv,
                                              InfoT* ofi,
                                              ColorT* fc,
                                              NormalT* fn,
                                              const FlagsPropertyManager& fpM,
                                              const GLuintIndexArray& vertexIndices,
                                              const GLuintIndexArray& objectIndices,
                                              const size_t& nObjects,
                                              float selectionColor[4],
                                              float currentColor[4]) {

        Q_ASSERT(fv != nullptr);
        if (fv == nullptr) {qDebug() << "GenericGLRenderer::staticBasicObjectVertexRender" << ", " <<  "fv == nullptr"; return;}

        Q_ASSERT(ofi != nullptr);
        if (ofi == nullptr) {qDebug() << "GenericGLRenderer::staticBasicObjectVertexRender" << ", " <<  "ofi == nullptr"; return;}

        quint8 nVertexPerObject = 0;

        if(glMode == GL_POINTS)
            nVertexPerObject = 1;
        else if(glMode == GL_LINES)
            nVertexPerObject = 2;
        else if(glMode == GL_TRIANGLES)
            nVertexPerObject = 3;

        Q_ASSERT(nVertexPerObject != 0);
        if (nVertexPerObject == 0) {qDebug() << "GenericGLRenderer::staticBasicObjectVertexRender" << ", " <<  "nVertexPerObject == 0"; return;}

        quint8 j;
        size_t currentIndex = 0;
        bool modified = false;

        glColor4fv(currentColor);

        if(fn != nullptr) {
            if(fc != nullptr) {

                // vertex + color + normal + info
                glBegin(glMode);
                for(size_t i=0; i<nObjects; ++i) {

                    const GLuint& objectLocalIndex = objectIndices[i];
                    const InfoT& info = ofi[objectLocalIndex];

                    if(ObjectsFlagsTool::staticIsObjectVisible(info)) {

                        const bool isSelected = ObjectsFlagsTool::staticIsObjectSelected(info);

                        for(j=0; j<nVertexPerObject; ++j) {

                            const GLuint& vertexLocalIndex = vertexIndices[currentIndex++];

                            if(!isSelected)
                                staticInlineSetFlagColorWhenHasColor(vertexLocalIndex, fc, info, fpM);
                            else
                                glColor4fv(selectionColor);

                            glNormal3fv(&(fn[vertexLocalIndex][0]));
                            glVertex3fv(&(fv[vertexLocalIndex][0]));
                        }
                    } else {
                        currentIndex += nVertexPerObject;
                    }
                }
                glEnd();
                glColor4fv(currentColor);
                return;
            }

            // vertex + normal + info
            glBegin(glMode);
            for(size_t i=0; i<nObjects; ++i) {

                const GLuint& objectLocalIndex = objectIndices[i];
                const InfoT& info = ofi[objectLocalIndex];

                if(ObjectsFlagsTool::staticIsObjectVisible(info)) {

                    staticInlineSetColorWhenNoColor(info, fpM, selectionColor, currentColor, modified);

                    for(j=0; j<nVertexPerObject; ++j) {
                        const GLuint& vertexLocalIndex = vertexIndices[currentIndex++];
                        glNormal3fv(&(fn[vertexLocalIndex][0]));
                        glVertex3fv(&(fv[vertexLocalIndex][0]));
                    }
                } else {
                    currentIndex += nVertexPerObject;
                }
            }
            glEnd();
            glColor4fv(currentColor);
            return;
        }

        if(fc != nullptr) {
            // vertex + color + info
            glBegin(glMode);
            for(size_t i=0; i<nObjects; ++i) {

                const GLuint& objectLocalIndex = objectIndices[i];
                const InfoT& info = ofi[objectLocalIndex];

                if(ObjectsFlagsTool::staticIsObjectVisible(info)) {

                    const bool isSelected = ObjectsFlagsTool::staticIsObjectSelected(info);

                    for(j=0; j<nVertexPerObject; ++j) {
                        const GLuint& vertexLocalIndex = vertexIndices[currentIndex++];

                        if(!isSelected)
                            staticInlineSetFlagColorWhenHasColor(vertexLocalIndex, fc, info, fpM);
                        else
                            glColor4fv(selectionColor);

                        glVertex3fv(&(fv[vertexLocalIndex][0]));
                    }
                } else {
                    currentIndex += nVertexPerObject;
                }
            }
            glEnd();
            glColor4fv(currentColor);
            return;
        }

        // vertex + info
        glBegin(glMode);
        for(size_t i=0; i<nObjects; ++i) {

            const GLuint& objectLocalIndex = objectIndices[i];
            const InfoT& info = ofi[objectLocalIndex];

            if(ObjectsFlagsTool::staticIsObjectVisible(info)) {

                staticInlineSetColorWhenNoColor(info, fpM, selectionColor, currentColor, modified);

                for(j=0; j<nVertexPerObject; ++j) {
                    const GLuint& vertexLocalIndex = vertexIndices[currentIndex++];
                    glVertex3fv(&(fv[vertexLocalIndex][0]));
                }
            } else {
                currentIndex += nVertexPerObject;
            }
        }
        glEnd();
        glColor4fv(currentColor);
    }

    /**
     * @brief Render normals with call to basic opengl function
     * @param fv : the first vertex in the array, can not be nullptr !
     * @param fi : the first info in the array, can not be nullptr !
     * @param fn : the first normal in the array, can be nullptr => if so no normals will be render
     * @param indices : array of indices to render
     * @param size : number of indices in the array to render
     * @param length : length of normal
     * @param normalColor : color of normal
     * @param selectionColor : selection color to use if a normal is selected
     * @param currentColor : current color
     */
    template<typename VertexT, typename InfoT, typename NormalT, typename GLuintIndexArray>
    static void staticBasicNormalRender(VertexT* fv,
                                        InfoT* fi,
                                        NormalT* fn,
                                        const FlagsPropertyManager& fpM,
                                        const GLuintIndexArray &indices,
                                        const size_t &size,
                                        const float& length,
                                        float normalColor[4],
                                        float selectionColor[4],
                                        float currentColor[4]) {

        Q_ASSERT(fv != nullptr);
        if (fv == nullptr) {qDebug() << "GenericGLRenderer::staticBasicNormalRender" << ", " <<  "fv == nullptr"; return;}

        Q_ASSERT(fi != nullptr);
        if (fi == nullptr) {qDebug() << "GenericGLRenderer::staticBasicNormalRender" << ", " <<  "fi == nullptr"; return;}

        if(fn != nullptr) {
            bool modified = false;

            glBegin(GL_LINES);
            glColor4fv(normalColor);
            for(size_t i=0; i<size; ++i) {
                const GLuint &localIndex = indices[i];
                const InfoT& info = fi[localIndex];

                if(ObjectsFlagsTool::staticIsObjectVisible(info)) {
                    VertexT &p = fv[localIndex];
                    NormalT &n = fn[localIndex];

                    staticInlineSetColorWhenNoColor(info, fpM, selectionColor, normalColor, modified);

                    glVertex3fv(&(p[0]));
                    glVertex3f((n[0] * length) + p[0],
                                (n[1] * length) + p[1],
                                (n[2] * length) + p[2]);
                }
            }
            glEnd();
            glColor4fv(currentColor);
        }
    }

    /**
     * @brief Fast render normals with call to basic opengl function
     * @param fv : the first vertex in the array, can not be nullptr !
     * @param fi : the first info in the array, can not be nullptr !
     * @param fn : the first normal in the array, can be nullptr => if so no normals will be render
     * @param indices : array of indices to render
     * @param size : number of indices in the array to render
     * @param stride : jump to next index
     * @param length : length of normal
     * @param normalColor : color of normal
     * @param selectionColor : selection color to use if a normal is selected
     * @param currentColor : current color
     */
    template<typename VertexT, typename InfoT, typename NormalT, typename GLuintIndexArray>
    static void staticBasicNormalFastRender(VertexT* fv,
                                            InfoT* fi,
                                            NormalT* fn,
                                            const FlagsPropertyManager& fpM,
                                            const GLuintIndexArray &indices,
                                            const size_t &size,
                                            const size_t &stride,
                                            const float& length,
                                            float normalColor[4],
                                            float selectionColor[4],
                                            float currentColor[4]) {

        Q_ASSERT(fv != nullptr);
        if (fv == nullptr) {qDebug() << "GenericGLRenderer::staticBasicNormalFastRender" << ", " <<  "fv == nullptr"; return;}

        Q_ASSERT(fi != nullptr);
        if (fi == nullptr) {qDebug() << "GenericGLRenderer::staticBasicNormalFastRender" << ", " <<  "fi == nullptr"; return;}

        if(fn != nullptr) {
            bool modified = false;

            glBegin(GL_LINES);
            glColor4fv(normalColor);
            for(size_t i=0; i<size; i += stride) {
                const GLuint &localIndex = indices[i];
                const InfoT& info = fi[localIndex];

                if(ObjectsFlagsTool::staticIsObjectVisible(info)) {
                    VertexT &p = fv[localIndex];
                    NormalT &n = fn[localIndex];

                    staticInlineSetColorWhenNoColor(info, fpM, selectionColor, normalColor, modified);

                    glVertex3fv(&(p[0]));
                    glVertex3f((n[0] * length) + p[0],
                                (n[1] * length) + p[1],
                                (n[2] * length) + p[2]);
                }
            }
            glEnd();
            glColor4fv(currentColor);
        }
    }

    /**
     * @brief Render normals of objects (triangles, lines, etc...) with call to basic opengl function but use the vertex normals attributes
     * @param fv : the first vertex in the array, can not be nullptr !
     * @param foi : the first object info in the array, can not be nullptr !
     * @param fn : the first vertex normal in the array, can be nullptr => if so no normals will be render
     * @param vertexIndices : array of vertex indices to render
     * @param objectIndices : array of object indices to render
     * @param nObjects : number of objects in the array to render
     * @param nVertexPerObject : number of vertex per object
     * @param length : length of normal
     * @param normalColor : color of normal
     * @param selectionColor : selection color to use if a normal is selected
     * @param currentColor : current color
     */
    template<typename VertexT, typename InfoT, typename NormalT, typename GLuintIndexArray>
    static void staticBasicObjectVertexNormalRender(VertexT* fv,
                                                    InfoT* foi,
                                                    NormalT* fn,
                                                    const FlagsPropertyManager& fpM,
                                                    const GLuintIndexArray& vertexIndices,
                                                    const GLuintIndexArray& objectIndices,
                                                    const size_t& nObjects,
                                                    const float& length,
                                                    const quint8& nVertexPerObject,
                                                    float normalColor[4],
                                                    float selectionColor[4],
                                                    float currentColor[4]) {

        Q_ASSERT(fv != nullptr);
        if (fv == nullptr) {qDebug() << "GenericGLRenderer::staticBasicObjectVertexNormalRender" << ", " <<  "fv == nullptr"; return;}

        Q_ASSERT(foi != nullptr);
        if (foi == nullptr) {qDebug() << "GenericGLRenderer::staticBasicObjectVertexNormalRender" << ", " <<  "foi == nullptr"; return;}

        if(fn != nullptr) {
            quint8 j;
            size_t currentIndex = 0;
            bool modified = false;

            glBegin(GL_LINES);
            glColor4fv(normalColor);
            for(size_t i=0; i<nObjects; ++i) {
                const GLuint &localObjectIndex = objectIndices[i];
                const InfoT& info = foi[localObjectIndex];

                if(ObjectsFlagsTool::staticIsObjectVisible(info)) {

                    staticInlineSetColorWhenNoColor(info, fpM, selectionColor, normalColor, modified);

                    for(j=0; j<nVertexPerObject; ++j) {

                        const GLuint& vertexLocalIndex = vertexIndices[currentIndex++];

                        VertexT &p = fv[vertexLocalIndex];
                        NormalT &n = fn[vertexLocalIndex];

                        glVertex3fv(&(p[0]));
                        glVertex3f((n[0] * length) + p[0],
                                    (n[1] * length) + p[1],
                                    (n[2] * length) + p[2]);
                    }
                } else {
                    currentIndex += nVertexPerObject;
                }
            }
            glEnd();
            glColor4fv(currentColor);
        }
    }

    /**
     * @brief Render normals of objects (triangles, lines, etc...) with call to basic opengl function but use the object normals attributes
     * @param fv : the first vertex in the array, can not be nullptr !
     * @param ofi : the first object info in the array, can not be nullptr !
     * @param ofn : the first object normal in the array, can be nullptr => if so no normals will be render
     * @param vertexIndices : array of vertex indices to render
     * @param objectIndices : array of object indices to render
     * @param nObjects : number of objects in the array to render
     * @param nVertexPerObject : number of vertex per object
     * @param length : length of normal
     * @param normalColor : color of normal
     * @param selectionColor : selection color to use if a normal is selected
     * @param currentColor : current color
     */
    template<typename VertexT, typename InfoT, typename NormalT, typename GLuintIndexArray>
    static void staticBasicObjectNormalRender(VertexT* fv,
                                              InfoT* ofi,
                                              NormalT* ofn,
                                              const FlagsPropertyManager& fpM,
                                              const GLuintIndexArray& vertexIndices,
                                              const GLuintIndexArray& objectIndices,
                                              const size_t& nObjects,
                                              const float& length,
                                              const quint8& nVertexPerObject,
                                              float normalColor[4],
                                              float selectionColor[4],
                                              float currentColor[4]) {

        Q_ASSERT(fv != nullptr);
        if (fv == nullptr) {qDebug() << "GenericGLRenderer::staticBasicObjectNormalRender" << ", " <<  "fv == nullptr"; return;}

        Q_ASSERT(ofi != nullptr);
        if (ofi == nullptr) {qDebug() << "GenericGLRenderer::staticBasicObjectNormalRender" << ", " <<  "ofi == nullptr"; return;}

        if(ofn != nullptr) {
            quint8 j;
            size_t currentIndex = 0;
            bool modified = false;
            float nxOrigin, nyOrigin, nzOrigin;

            glBegin(GL_LINES);
            glColor4fv(normalColor);
            for(size_t i=0; i<nObjects; ++i) {
                const GLuint &localObjectIndex = objectIndices[i];
                const InfoT& info = ofi[localObjectIndex];

                if(ObjectsFlagsTool::staticIsObjectVisible(info)) {

                    NormalT &n = ofn[localObjectIndex];

                    staticInlineSetColorWhenNoColor(info, fpM, selectionColor, normalColor, modified);

                    nxOrigin = 0;
                    nyOrigin = 0;
                    nzOrigin = 0;

                    for(j=0; j<nVertexPerObject; ++j) {

                        const GLuint& vertexLocalIndex = vertexIndices[currentIndex++];
                        VertexT &p = fv[vertexLocalIndex];

                        nxOrigin += p[0];
                        nyOrigin += p[1];
                        nzOrigin += p[2];
                    }

                    nxOrigin /= float(nVertexPerObject);
                    nyOrigin /= float(nVertexPerObject);
                    nzOrigin /= float(nVertexPerObject);

                    glVertex3f(nxOrigin, nyOrigin, nzOrigin);
                    glVertex3f((n[0] * length) + nxOrigin,
                                (n[1] * length) + nyOrigin,
                                (n[2] * length) + nzOrigin);

                } else {
                    currentIndex += nVertexPerObject;
                }
            }
            glEnd();
            glColor4fv(currentColor);
        }
    }

    ///////////////////////// OUTLIERS /////////////////////////////

    /**
     * @brief Render "outliers" objects (triangles, lines, etc...) with call to basic opengl function but use the vertex colors/normals attributes
     * @param glMode : the opengl mode to use (GL_POINTS, GL_LINES, etc...)
     * @param vertexes : collection of structure that contains vertexes with associated offset
     * @param ofi : the first info of objects in the array, can not be nullptr !
     * @param fc : the first color of vertexes in the array, can be nullptr
     * @param fn : the first normal of vertexes in the array, can be nullptr
     * @param vertexIndices : array of indices of vertex to render
     * @param objectIndices : array of indices of object to use to get color, normal, info from array
     * @param nObjects : number of objects to render
     * @param selectionColor : selection color to use
     * @param currentColor : current color used
     */
    template<typename VertexesWithOffsetCollection, typename InfoT, typename ColorT, typename NormalT, typename GLuintIndexArray>
    static void staticBasicObjectVertexOutliersRender(GLenum glMode,
                                                      const VertexesWithOffsetCollection& vertexes,
                                                      InfoT* ofi,
                                                      ColorT* fc,
                                                      NormalT* fn,
                                                      const FlagsPropertyManager& fpM,
                                                      const GLuintIndexArray& vertexIndices,
                                                      const GLuintIndexArray& objectIndices,
                                                      const size_t& nObjects,
                                                      float selectionColor[4],
                                                      float currentColor[4]) {

        Q_ASSERT(ofi != nullptr);
        if (ofi == nullptr) {qDebug() << "GenericGLRenderer::staticBasicObjectVertexOutliersRender" << ", " <<  "ofi == nullptr"; return;}

        quint8 nVertexPerObject = 0;

        if(glMode == GL_POINTS)
            nVertexPerObject = 1;
        else if(glMode == GL_LINES)
            nVertexPerObject = 2;
        else if(glMode == GL_TRIANGLES)
            nVertexPerObject = 3;

        Q_ASSERT(nVertexPerObject != 0);
        if (nVertexPerObject == 0) {qDebug() << "GenericGLRenderer::staticBasicObjectVertexOutliersRender" << ", " <<  "nVertexPerObject == 0"; return;}

        quint8 j;
        size_t currentIndex = 0;
        bool modified = false;

        glColor4fv(currentColor);

        if(fn != nullptr) {
            if(fc != nullptr) {

                // vertex + color + normal + info
                for(size_t i=0; i<nObjects; ++i) {

                    const GLuint& objectLocalIndex = objectIndices[i];
                    const InfoT& info = ofi[objectLocalIndex];

                    if(ObjectsFlagsTool::staticIsObjectVisible(info)) {

                        const bool isSelected = ObjectsFlagsTool::staticIsObjectSelected(info);

                        const typename VertexesWithOffsetCollection::value_type& vertexesAndOffset = vertexes[i];

                        glPushMatrix();
                        glTranslatef(vertexesAndOffset.offset[0], vertexesAndOffset.offset[1], vertexesAndOffset.offset[2]);
                        glBegin(glMode);

                        for(j=0; j<nVertexPerObject; ++j) {

                            const GLuint& vertexLocalIndex = vertexIndices[currentIndex++];

                            if(!isSelected)
                                staticInlineSetFlagColorWhenHasColor(vertexLocalIndex, fc, info, fpM);
                            else
                                glColor4fv(selectionColor);

                            glNormal3fv(&(fn[vertexLocalIndex][0]));
                            glVertex3fv(&(vertexesAndOffset.vertexes[j][0]));
                        }

                        glEnd();
                        glPopMatrix();

                    } else {
                        currentIndex += nVertexPerObject;
                    }
                }
                glColor4fv(currentColor);
                return;
            }

            // vertex + normal + info
            for(size_t i=0; i<nObjects; ++i) {

                const GLuint& objectLocalIndex = objectIndices[i];
                const InfoT& info = ofi[objectLocalIndex];

                if(ObjectsFlagsTool::staticIsObjectVisible(info)) {

                    staticInlineSetColorWhenNoColor(info, fpM, selectionColor, currentColor, modified);

                    const typename VertexesWithOffsetCollection::value_type& vertexesAndOffset = vertexes[i];

                    glPushMatrix();
                    glTranslatef(vertexesAndOffset.offset[0], vertexesAndOffset.offset[1], vertexesAndOffset.offset[2]);
                    glBegin(glMode);

                    for(j=0; j<nVertexPerObject; ++j) {
                        const GLuint& vertexLocalIndex = vertexIndices[currentIndex++];
                        glNormal3fv(&(fn[vertexLocalIndex][0]));
                        glVertex3fv(&(vertexesAndOffset.vertexes[j][0]));
                    }
                    glEnd();
                    glPopMatrix();
                } else {
                    currentIndex += nVertexPerObject;
                }
            }
            glColor4fv(currentColor);
            return;
        }

        if(fc != nullptr) {
            // vertex + color + info
            for(size_t i=0; i<nObjects; ++i) {

                const GLuint& objectLocalIndex = objectIndices[i];
                const InfoT& info = ofi[objectLocalIndex];

                if(ObjectsFlagsTool::staticIsObjectVisible(info)) {

                    const bool isSelected = ObjectsFlagsTool::staticIsObjectSelected(info);

                    const typename VertexesWithOffsetCollection::value_type& vertexesAndOffset = vertexes[i];

                    glPushMatrix();
                    glTranslatef(vertexesAndOffset.offset[0], vertexesAndOffset.offset[1], vertexesAndOffset.offset[2]);
                    glBegin(glMode);

                    for(j=0; j<nVertexPerObject; ++j) {
                        const GLuint& vertexLocalIndex = vertexIndices[currentIndex++];

                        if(!isSelected)
                            staticInlineSetFlagColorWhenHasColor(vertexLocalIndex, fc, info, fpM);
                        else
                            glColor4fv(selectionColor);

                        glVertex3fv(&(vertexesAndOffset.vertexes[j][0]));
                    }

                    glEnd();
                    glPopMatrix();
                } else {
                    currentIndex += nVertexPerObject;
                }
            }
            glColor4fv(currentColor);
            return;
        }

        // vertex + info
        for(size_t i=0; i<nObjects; ++i) {

            const GLuint& objectLocalIndex = objectIndices[i];
            const InfoT& info = ofi[objectLocalIndex];

            if(ObjectsFlagsTool::staticIsObjectVisible(info)) {

                staticInlineSetColorWhenNoColor(info, fpM, selectionColor, currentColor, modified);

                const typename VertexesWithOffsetCollection::value_type& vertexesAndOffset = vertexes[i];

                glPushMatrix();
                glTranslatef(vertexesAndOffset.offset[0], vertexesAndOffset.offset[1], vertexesAndOffset.offset[2]);
                glBegin(glMode);

                for(j=0; j<nVertexPerObject; ++j) {
                    glVertex3fv(&(vertexesAndOffset.vertexes[j][0]));
                }
                glEnd();
                glPopMatrix();
            } else {
                currentIndex += nVertexPerObject;
            }
        }
        glColor4fv(currentColor);
    }

    /**
     * @brief Render "outliers" objects (triangles, lines, etc...) with call to basic opengl function
     * @param glMode : the opengl mode to use (GL_POINTS, GL_LINES, etc...)
     * @param vertexes : collection of structure that contains vertexes with associated offset
     * @param ofi : the first info of objects in the array, can not be nullptr !
     * @param ofc : the first color of objects in the array, can be nullptr
     * @param ofn : the first normal of objects in the array, can be nullptr
     * @param objectIndices : array of indices of object to use to get color, normal, info from array
     * @param nObjects : number of objects to render
     * @param selectionColor : selection color to use
     * @param currentColor : current color used
     */
    template<typename VertexesWithOffsetCollection, typename InfoT, typename ColorT, typename NormalT, typename GLuintIndexArray>
    static void staticBasicObjectOutliersRender(GLenum glMode,
                                                const VertexesWithOffsetCollection& vertexes,
                                                InfoT* ofi,
                                                ColorT* ofc,
                                                NormalT* ofn,
                                                const FlagsPropertyManager& fpM,
                                                const GLuintIndexArray& objectIndices,
                                                const size_t& nObjects,
                                                float selectionColor[4],
                                                float currentColor[4]) {

        Q_ASSERT(ofi != nullptr);
        if (ofi == nullptr) {qDebug() << "GenericGLRenderer::staticBasicObjectOutliersRender" << ", " <<  "ofi == nullptr"; return;}

        quint8 nVertexPerObject = 0;

        if(glMode == GL_POINTS)
            nVertexPerObject = 1;
        else if(glMode == GL_LINES)
            nVertexPerObject = 2;
        else if(glMode == GL_TRIANGLES)
            nVertexPerObject = 3;

        if(nVertexPerObject == 0)
            return;

        quint8 j;
        //size_t currentIndex = 0;
        bool modified = false;

        glColor4fv(currentColor);

        if(ofn != nullptr) {
            if(ofc != nullptr) {

                // vertex + color + normal + info
                for(size_t i=0; i<nObjects; ++i) {

                    const GLuint& objectLocalIndex = objectIndices[i];
                    const InfoT& info = ofi[objectLocalIndex];

                    if(ObjectsFlagsTool::staticIsObjectVisible(info)) {

                        staticInlineSetColorWhenHasColor(objectLocalIndex, ofc, info, fpM, selectionColor);

                        glNormal3fv(&(ofn[objectLocalIndex][0]));

                        const typename VertexesWithOffsetCollection::value_type& vertexesAndOffset = vertexes[i];

                        glPushMatrix();
                        glTranslatef(vertexesAndOffset.offset[0], vertexesAndOffset.offset[1], vertexesAndOffset.offset[2]);
                        glBegin(glMode);

                        for(j=0; j<nVertexPerObject; ++j) {
                            glVertex3fv(&(vertexesAndOffset.vertexes[j][0]));
                        }
                        glEnd();
                        glPopMatrix();
                    }
                    //} else {
                    //    currentIndex += nVertexPerObject;
                    //}
                }
                glColor4fv(currentColor);
                return;
            }

            // vertex + normal + info
            for(size_t i=0; i<nObjects; ++i) {

                const GLuint& objectLocalIndex = objectIndices[i];
                const InfoT& info = ofi[objectLocalIndex];

                if(ObjectsFlagsTool::staticIsObjectVisible(info)) {

                    staticInlineSetColorWhenNoColor(info, fpM, selectionColor, currentColor, modified);

                    glNormal3fv(&(ofn[objectLocalIndex][0]));

                    const typename VertexesWithOffsetCollection::value_type& vertexesAndOffset = vertexes[i];

                    glPushMatrix();
                    glTranslatef(vertexesAndOffset.offset[0], vertexesAndOffset.offset[1], vertexesAndOffset.offset[2]);
                    glBegin(glMode);

                    for(j=0; j<nVertexPerObject; ++j) {
                        glVertex3fv(&(vertexesAndOffset.vertexes[j][0]));
                    }
                    glEnd();
                    glPopMatrix();
                }
                //} else {
                //    currentIndex += nVertexPerObject;
                //}
            }
            glColor4fv(currentColor);
            return;
        }

        if(ofc != nullptr) {
            // vertex + color + info
            for(size_t i=0; i<nObjects; ++i) {

                const GLuint& objectLocalIndex = objectIndices[i];
                const InfoT& info = ofi[objectLocalIndex];

                if(ObjectsFlagsTool::staticIsObjectVisible(info)) {

                    staticInlineSetColorWhenHasColor(objectLocalIndex, ofc, info, fpM, selectionColor);

                    const typename VertexesWithOffsetCollection::value_type& vertexesAndOffset = vertexes[i];

                    glPushMatrix();
                    glTranslatef(vertexesAndOffset.offset[0], vertexesAndOffset.offset[1], vertexesAndOffset.offset[2]);
                    glBegin(glMode);

                    for(j=0; j<nVertexPerObject; ++j) {
                        glVertex3fv(&(vertexesAndOffset.vertexes[j][0]));
                    }
                    glEnd();
                    glPopMatrix();
                }
                //} else {
                //    currentIndex += nVertexPerObject;
                //}
            }
            glColor4fv(currentColor);
            return;
        }

        // vertex + info
        for(size_t i=0; i<nObjects; ++i) {

            const GLuint& objectLocalIndex = objectIndices[i];
            const InfoT& info = ofi[objectLocalIndex];

            if(ObjectsFlagsTool::staticIsObjectVisible(info)) {

                staticInlineSetColorWhenNoColor(info, fpM, selectionColor, currentColor, modified);

                const typename VertexesWithOffsetCollection::value_type& vertexesAndOffset = vertexes[i];

                glPushMatrix();
                glTranslatef(vertexesAndOffset.offset[0], vertexesAndOffset.offset[1], vertexesAndOffset.offset[2]);
                glBegin(glMode);

                for(j=0; j<nVertexPerObject; ++j) {
                    glVertex3fv(&(vertexesAndOffset.vertexes[j][0]));
                }
                glEnd();
                glPopMatrix();
            }
            //else
            //{
            //    currentIndex += nVertexPerObject;
            //}
        }
        glColor4fv(currentColor);
    }
};

#endif // GENERICGLRENDERER_H
