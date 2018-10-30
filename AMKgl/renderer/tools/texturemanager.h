#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <QOpenGLTexture>

#include "renderer/ichunk.h"

/**
 * @brief This class must be a singleton per opengl context. It will manage all texture created
 *        to keep the minimum memory consumption in the graphics card.
 */
class TextureManager
{
public:
    TextureManager();
    ~TextureManager();

    /**
     * @brief Create a new texture of type GL_TEXTURE_1D or use an existing if this attributes was same :
     *          - name
     *          - serverFormat
     *          - minificationFilter
     *          - magnificationFilter
     *
     *        If all attributes above is same but the size is different the existing texture is resized to
     *        the biggest size.
     *
     * @param uniqueName : a unique name for developper that will be used to differentiate two textures with the same attributes.
     * @param dataSize : the number of data
     * @param serverFormat : the texture format that will be used in graphics card
     * @param minificationFilter : minification filter
     * @param magnificationFilter : magnification filter
     * @return true if texture was created or same texture was found
     */
    bool createNew1DTextureOrUseExisting(const QString& uniqueName,
                                         const size_t& dataSize,
                                         QOpenGLTexture::TextureFormat serverFormat,
                                         QOpenGLTexture::Filter minificationFilter = QOpenGLTexture::Nearest,
                                         QOpenGLTexture::Filter magnificationFilter = QOpenGLTexture::Nearest);

    /**
     * @brief Returns the texture but update data before if must be.
     * @param uniqueName : unique name used when it was created
     * @param clientFormat : the pixel format of data
     * @param clientPixelType : the pixel type of data
     * @param dataSize : the number of data
     * @param data : pointer to the first data
     * @return NULL if texture was not created, otherwise return the updated texture that you can use now
     */
    QOpenGLTexture* getTextureAndUpdateDataIfNecessary(const QString& uniqueName,
                                                       QOpenGLTexture::PixelFormat clientFormat,
                                                       QOpenGLTexture::PixelType clientPixelType,
                                                       const size_t& dataSize,
                                                       void* data) const;

    /**
     * @brief Set data specidifed by it's unique name to be updated because some element has changed
     * @param uniqueName : unique name speicifed when you have created the texture
     */
    void setDataToBeUpdated(const QString& uniqueName);

private:
    struct TextureData {
        void*   data;
        size_t  dataSize;

        TextureData();
        TextureData(void* d, const size_t& dS);
        bool operator !=(const TextureData& other) const;
        bool operator ==(const TextureData& other) const;
    };

    struct TextureInfo {
        QOpenGLTexture*         texture;
        QString                 uniqueName;
        TextureData             lastData;

        TextureInfo(const QString& un, QOpenGLTexture* tex);
        ~TextureInfo();
    };

    typedef QVector<TextureInfo*>           TextureCollection;
    typedef QVectorIterator<TextureInfo*>   TextureCollectionIterator;

    TextureCollection           m_uniqueTextures;

    /**
     * @brief Returns the existing texture that can be shared if it was already created or NULL if not.
     */
    TextureInfo* getTextureThatCanBeShared(const QString& uniqueName,
                                           QOpenGLTexture::TextureFormat serverFormat,
                                           QOpenGLTexture::Filter minificationFilter,
                                           QOpenGLTexture::Filter magnificationFilter) const;

    /**
     * @brief Returns the existing texture if it was already created or NULL if not.
     */
    TextureInfo* getTextureByName(const QString& uniqueName) const;
};

#endif // TEXTUREMANAGER_H
