#include "texturemanager.h"

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
    qDeleteAll(m_uniqueTextures.begin(), m_uniqueTextures.end());
}

bool TextureManager::createNew1DTextureOrUseExisting(const QString &uniqueName,
                                                     const size_t &dataSize,
                                                     QOpenGLTexture::TextureFormat serverFormat,
                                                     QOpenGLTexture::Filter minificationFilter,
                                                     QOpenGLTexture::Filter magnificationFilter)
{
    TextureInfo* info = getTextureThatCanBeShared(uniqueName,
                                                  serverFormat,
                                                  minificationFilter,
                                                  magnificationFilter);

    if(info == nullptr) {
        QOpenGLTexture* texture = new QOpenGLTexture(QOpenGLTexture::Target1D);
        texture->setFormat(serverFormat);
        texture->setMinificationFilter(minificationFilter);
        texture->setMagnificationFilter(magnificationFilter);
        texture->setSize((int)dataSize);
        texture->allocateStorage();
        texture->release();

        info = new TextureInfo(uniqueName, texture);
        m_uniqueTextures.append(info);
    } else {
        if(static_cast<int>(dataSize) > info->texture->width()) {
            info->texture->destroy();
            info->texture->setSize((int)dataSize);
            info->texture->allocateStorage();
            info->texture->release();

            // to be updated !
            info->lastData = TextureData();
        }
    }

    return info->texture->isStorageAllocated();
}

QOpenGLTexture* TextureManager::getTextureAndUpdateDataIfNecessary(const QString &uniqueName,
                                                                   QOpenGLTexture::PixelFormat clientFormat,
                                                                   QOpenGLTexture::PixelType clientPixelType,
                                                                   const size_t &dataSize,
                                                                   void *data) const
{
    TextureInfo* info = getTextureByName(uniqueName);

    Q_ASSERT(info != nullptr);

    TextureData newData(data, dataSize);

    // if data must be updated
    if(info->lastData != newData) {
        info->lastData = newData;

        info->texture->bind();
        glTexSubImage1D(GL_TEXTURE_1D, 0, 0, (GLsizei)dataSize, clientFormat, clientPixelType, data); // transfer new data to it
        info->texture->release();
    }

    return info->texture;
}

void TextureManager::setDataToBeUpdated(const QString &uniqueName)
{
    TextureInfo* info = getTextureByName(uniqueName);

    Q_ASSERT(info != nullptr);

    info->lastData = TextureData();
}

TextureManager::TextureInfo* TextureManager::getTextureThatCanBeShared(const QString &uniqueName,
                                                                       QOpenGLTexture::TextureFormat serverFormat,
                                                                       QOpenGLTexture::Filter minificationFilter,
                                                                       QOpenGLTexture::Filter magnificationFilter) const
{
    TextureCollectionIterator it(m_uniqueTextures);

    while(it.hasNext()) {
        TextureInfo* info = it.next();

        if((info->uniqueName == uniqueName)
                && (info->texture->format() == serverFormat)
                && (info->texture->minificationFilter() == minificationFilter)
                && (info->texture->magnificationFilter() == magnificationFilter))
            return info;
    }

    return nullptr;
}

TextureManager::TextureInfo* TextureManager::getTextureByName(const QString &uniqueName) const
{
    TextureCollectionIterator it(m_uniqueTextures);

    while(it.hasNext()) {
        TextureInfo* info = it.next();

        if(info->uniqueName == uniqueName)
            return info;
    }

    return nullptr;
}

TextureManager::TextureInfo::TextureInfo(const QString &un,
                                         QOpenGLTexture *tex)
{
    uniqueName = un;
    texture = tex;
}

TextureManager::TextureInfo::~TextureInfo()
{
    delete texture;
}

TextureManager::TextureData::TextureData()
{
    data = nullptr;
    dataSize = 0;
}

TextureManager::TextureData::TextureData(void *d, const size_t &dS)
{
    data = d;
    dataSize = dS;
}

bool TextureManager::TextureData::operator !=(const TextureManager::TextureData &other) const
{
    return !(this->operator ==(other));
}

bool TextureManager::TextureData::operator ==(const TextureManager::TextureData &other) const
{
    return (other.data == data) && (other.dataSize == dataSize);
}
