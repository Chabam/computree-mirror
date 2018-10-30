#ifndef OBJECTSFLAGPROPERTY_H
#define OBJECTSFLAGPROPERTY_H

#include <QColor>

/**
 * @brief Container for flags property. Used for visualisation
 *       information. See "ObjectsFlagsTool" and "PermanentScene" for more information.
 */
struct ObjectsFlagProperty
{
    ObjectsFlagProperty() {
        objectColorWhenFlagIsON = Qt::white;
    }

    ObjectsFlagProperty(const QColor& color) {
        objectColorWhenFlagIsON = color;
    }

    /**
     * @brief Color of the object when the associated flag is ON
     */
    QColor  objectColorWhenFlagIsON;

    bool operator !=(const ObjectsFlagProperty& other) {
        return this->objectColorWhenFlagIsON != other.objectColorWhenFlagIsON;
    }

    bool operator ==(const ObjectsFlagProperty& other) {
        return !((*this) != other);
    }
};

#endif // OBJECTSFLAGPROPERTY_H
