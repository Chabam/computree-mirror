#ifndef DRAWMODECONVERTER_H
#define DRAWMODECONVERTER_H

#include <QObject>

#include "renderer/drawmode.h"

class DrawModeConverter
{
public:
    static QString drawModeToQString(const DrawMode& dm) {
        if(dm == DM_NONE)
            return QObject::tr("None");
        else if(dm == DM_BASIC)
            return QObject::tr("Basic");
        else if(dm == DM_DISPLAY_LIST)
            return QObject::tr("Display List");
        else if(dm == DM_RAW)
            return QObject::tr("Raw");
        else if(dm == DM_VBO)
            return QObject::tr("VBO");
        else if(dm == DM_VAO)
            return QObject::tr("VAO");

        return QObject::tr("Unknown");
    }
};

#endif // DRAWMODECONVERTER_H
