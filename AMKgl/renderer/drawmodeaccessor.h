#ifndef DRAWMODEACCESSOR
#define DRAWMODEACCESSOR

#include "drawmode.h"
#include "scene/permanentsceneelementtype.h"

class DrawModeAccessor {
public:
    virtual ~DrawModeAccessor() {}

    /**
     * @brief Define the draw mode to use for all type. If you set it to VBO or VAO and the graphics card
     *        cannot do it, it will be changed to RAW after update.
     */
    virtual void setDrawModeToUse(DrawMode mode) = 0;

    /**
     * @brief Define the draw mode to use by type. If you set it to VBO or VAO and the graphics card
     *        cannot do it, it will be changed to RAW after update.
     */
    virtual void setDrawModeToUse(Scene::ObjectType type, DrawMode mode) = 0;

    /**
     * @brief Returns the draw mode to use for the type specified.
     * @warning It is not necessary the draw mode used !
     */
    virtual DrawMode getDrawModeToUse(Scene::ObjectType type) const = 0;

    /**
     * @brief Change draw mode to use from another accessor
     */
    virtual DrawModeAccessor& operator =(const DrawModeAccessor& other) = 0;

    /**
     * @brief Returns true if different from another
     */
    bool operator !=(const DrawModeAccessor &other) const
    {
        int size = Scene::NumberOfElements;

        for(int i=0; i<size; ++i) {
            if(getDrawModeToUse((Scene::ObjectType)i) != other.getDrawModeToUse((Scene::ObjectType)i))
                return true;
        }

        return false;
    }
};

#endif // DRAWMODEACCESSOR

