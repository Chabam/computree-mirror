#include "flagspropertymanager.h"

#include <QMetaEnum>
#include <QVector4D>

FlagsPropertyManager::FlagsPropertyManager()
{
    m_properties.insert(ObjectsFlagsTool::F_DevelopperFlag1, ObjectsFlagProperty(Qt::green));
    m_properties.insert(ObjectsFlagsTool::F_DevelopperFlag2, ObjectsFlagProperty(Qt::yellow));
    m_properties.insert(ObjectsFlagsTool::F_DevelopperFlag3, ObjectsFlagProperty(Qt::blue));
    m_properties.insert(ObjectsFlagsTool::F_DevelopperFlag4, ObjectsFlagProperty(Qt::gray));
    m_properties.insert(ObjectsFlagsTool::F_DevelopperFlag5, ObjectsFlagProperty(Qt::magenta));
}

void FlagsPropertyManager::setProperty(const ObjectsFlagsTool::Flag &flag, const ObjectsFlagProperty &property)
{
    m_properties.insert(flag, property);
}

ObjectsFlagProperty FlagsPropertyManager::getProperty(const ObjectsFlagsTool::Flag &flag) const
{
    return m_properties.value(flag, ObjectsFlagProperty());
}

QVector4D FlagsPropertyManager::staticColorToVector4D(const QColor& color)
{
    QVector4D vec;
    vec[0] = color.redF();
    vec[1] = color.greenF();
    vec[2] = color.blueF();
    vec[3] = color.alphaF();

    return vec;
}

std::vector<QVector4D> FlagsPropertyManager::getColorsForAllFlags(const IGraphicsDocument *document) const
{
    std::vector<QVector4D> flagsColor(8);

    flagsColor[0] = staticColorToVector4D(document->getSelectionColor()); // selected
    flagsColor[1] = staticColorToVector4D(Qt::transparent); // visible
    flagsColor[2] = staticColorToVector4D(getProperty(ObjectsFlagsTool::F_DevelopperFlag1).objectColorWhenFlagIsON);
    flagsColor[3] = staticColorToVector4D(getProperty(ObjectsFlagsTool::F_DevelopperFlag2).objectColorWhenFlagIsON);
    flagsColor[4] = staticColorToVector4D(getProperty(ObjectsFlagsTool::F_DevelopperFlag3).objectColorWhenFlagIsON);
    flagsColor[5] = staticColorToVector4D(getProperty(ObjectsFlagsTool::F_DevelopperFlag4).objectColorWhenFlagIsON);
    flagsColor[6] = staticColorToVector4D(getProperty(ObjectsFlagsTool::F_DevelopperFlag5).objectColorWhenFlagIsON);
    flagsColor[7] = staticColorToVector4D(getProperty(ObjectsFlagsTool::F_Reserved).objectColorWhenFlagIsON);

    return flagsColor;
}
