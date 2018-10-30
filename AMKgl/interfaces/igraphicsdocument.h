#ifndef IGRAPHICSDOCUMENT_H
#define IGRAPHICSDOCUMENT_H

#include <QColor>
#include <QOpenGLWidget>

class PermanentItemSceneByModel;

/**
 * @brief This class represent the document that contains one or multiple viewers (IGLViewer)
 */
class IGraphicsDocument {
public:
    typedef PermanentItemSceneByModel   PermanentSceneToRender;

    virtual ~IGraphicsDocument() {}

    /**
     * @brief Returns the scene used by this document
     */
    virtual PermanentSceneToRender* getPermanentSceneToRender() const = 0;

    /**
     * @brief Returns the QOpenGLWidget that use the specified context
     */
    virtual QOpenGLWidget* getOpengWidgetWithContext(const QOpenGLContext* context) const = 0;

    /**
     * @brief Returns the selection color
     */
    virtual QColor getSelectionColor() const = 0;

    /**
     * @brief Returns the color to use for normals
     */
    virtual QColor getNormalsColor() const = 0;

    /**
     * @brief Returns the length to use for normals
     */
    virtual float getNormalsLength() const = 0;

    /**
     * @brief Returns true if normals must be drawned
     */
    virtual bool mustShowNormals() const = 0;
};

#endif // IGRAPHICSDOCUMENT_H
