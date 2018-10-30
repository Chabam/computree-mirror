#ifndef FLAGSPROPERTYMANAGER_H
#define FLAGSPROPERTYMANAGER_H

#include "interfaces/igraphicsdocument.h"
#include "picker/tools/objectsflagstool.h"
#include "renderer/tools/objectsflagproperty.h"

#include <QHash>

/**
 * @brief Class used to manage flags property
 */
class FlagsPropertyManager
{
public:
    FlagsPropertyManager();

    /**
     * @brief Set property for the specified flag
     */
    void setProperty(const ObjectsFlagsTool::Flag& flag,
                     const ObjectsFlagProperty& property);

    /**
     * @brief Returns the flag property currently used for the specified flag and specified object
     */
    ObjectsFlagProperty getProperty(const ObjectsFlagsTool::Flag& flag) const;

    /**
     * @brief Returns a list of colors for all flags, so you can use it in a shader per example
     */
    std::vector<QVector4D> getColorsForAllFlags(const IGraphicsDocument *document) const;

private:
    QHash<ObjectsFlagsTool::Flag, ObjectsFlagProperty>   m_properties;

    static QVector4D staticColorToVector4D(const QColor& Color);
};

#endif // FLAGSPROPERTYMANAGER_H
