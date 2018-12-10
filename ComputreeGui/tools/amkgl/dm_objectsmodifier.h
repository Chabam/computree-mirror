#ifndef DM_OBJECTSMODIFIER_H
#define DM_OBJECTSMODIFIER_H

#include "graphicsviewinterface.h"
#include "renderer/ichunk.h"

/**
 * @brief Use this class to modify all or a part of object of a specified chunk
 */
class DM_ObjectsModifier : public ObjectsModifierInterface
{
public:
    /**
     * @brief Create the modifier that will do modification on object between [from; from+count-1] on the spcified chunk
     * @param chunk : chunk to modify
     * @param from : index of the first object to modify
     * @param count : number of objects to modify
     */
    DM_ObjectsModifier(const IChunk* chunk,
                       const size_t& from,
                       const size_t& count,
                       bool createAsModifier = true);

    /**
     * @brief Returns the number of objects available
     */
    size_t getNumberOfObjects() const;

    /**
     * @brief Returns the number of vertex per object
     */
    size_t getNumberOfVertexPerObject() const;

    /**
     * @brief Apply a transformation matrix to all vertex. Use it if you want
     *        to translate/rotate/scale the object.
     *
     *        Do nothing for global points/edges/faces !
     *
     * @warning Be carreful when you translate or scale the object because if its
     *          coordinates change from one coordinate system to another the system
     *          won't detect it and the draw of it can be altered !
     */
    void transform(const Eigen::Matrix4f& trMatrix);

    /**
     * @brief Apply a translation to the object.
     *
     *        Do nothing for global points/edges/faces !
     *
     * @warning Be carreful when you translate the object because if its
     *          coordinates change from one coordinate system to another the system
     *          won't detect it and the draw of the object can be altered !
     */
    void translate(const Eigen::Vector3f& translation);

    /**
     * @brief Apply a rotation to the object.
     *
     *        Do nothing for global points/edges/faces !
     */
    void rotate(const Eigen::AngleAxis<float>& rotation);
    void rotate(const Eigen::Quaternion<float>& rotation);

    /**
     * @brief Scale the object.
     *
     *        Do nothing for global points/edges/faces !
     *
     * @warning Be carreful when you scale the object because if its
     *          coordinates change from one coordinate system to another the system
     *          won't detect it and the draw of the object can be altered !
     */
    void scale(const Eigen::Vector3f& scaling);

    /**
     * @brief Apply a transformation matrix to all vertex of the specified objects. Use it if you want
     *        to translate/rotate/scale objects.
     *
     *        Do nothing for global points/edges/faces !
     *
     * @warning Be carreful when you translate or scale an object because if its
     *          coordinates change from one coordinate system to another the system
     *          won't detect it and the draw of it can be altered !
     */
    void transform(const size_t& objectIndex,
                   const Eigen::Matrix4f& trMatrix);

    /**
     * @brief Apply a translation to the specified object.
     *
     *        Do nothing for global points/edges/faces !
     *
     * @warning Be carreful when you translate an object because if its
     *          coordinates change from one coordinate system to another the system
     *          won't detect it and the draw of the object can be altered !
     */
    void translate(const size_t& objectIndex,
                   const Eigen::Vector3f& translation);

    /**
     * @brief Apply a rotation to the specified object.
     *
     *        Do nothing for global points/edges/faces !
     */
    void rotate(const size_t& objectIndex,
                const Eigen::AngleAxis<float>& rotation);
    void rotate(const size_t& objectIndex,
                const Eigen::Quaternion<float>& rotation);

    /**
     * @brief Scale the specified object.
     *
     *        Do nothing for global points/edges/faces !
     *
     * @warning Be carreful when you scale the object because if its
     *          coordinates change from one coordinate system to another the system
     *          won't detect it and the draw of the object can be altered !
     */
    void scale(const size_t& objectIndex,
               const Eigen::Vector3f& scaling);

    /**
     * @brief Returns true if colors is available, false otherwise
     */
    bool isColorsAvailable() const;

    /**
     * @brief Returns the color of the specified vertex of the specified object
     * @warning If method "isColorsAvailable" return false this method will return an
     *          invalid color and generate an assert in debug mode
     */
    const CT_Color& getColor(const size_t& objectIndex,
                             const size_t& vertexIndex) const;

    /**
     * @brief Set the new color to all vertex of all objects
     */
    void setColor(const CT_Color& newColor);

    /**
     * @brief Set the new color to all vertex of the specified object
     */
    void setColor(const size_t& objectIndex,
                          const CT_Color& newColor);

    /**
     * @brief Set the new color to the specified vertex of the specified object
     */
    void setColor(const size_t& objectIndex,
                          const size_t& vertexIndex,
                          const CT_Color& newColor);

    /**
     * @brief Returns true if normals is available, false otherwise
     */
    bool isNormalsAvailable() const;

    /**
     * @brief Returns the normal of the specified vertex of the specified object
     * @warning If method "isNormalsAvailable" return false this method will return an
     *          invalid normal and generate an assert in debug mode
     */
    const CT_Normal& getNormal(const size_t& objectIndex,
                               const size_t& vertexIndex) const;

    /**
     * @brief Set the new normal to all objects
     */
    void setNormal(const CT_Normal& newNormal);

    /**
     * @brief Set the new normal to the specified object
     */
    void setNormal(const size_t& objectIndex,
                           const CT_Normal& newNormal);

    /**
     * @brief Set the new normal to the specified vertex of the specified object
     */
    void setNormal(const size_t& objectIndex,
                           const size_t& vertexIndex,
                           const CT_Normal& newNormal);

    /**
     * @brief Returns true if informations (visible, selected) is available, false otherwise
     */
    bool isInfosAvailable() const;

    /**
     * @brief Returns if the specified object is visible or not
     * @warning If method "isInfosAvailable" return false this method will return an
     *          invalid value and generate an assert in debug mode
     */
    bool isVisible(const size_t& objectIndex) const;

    /**
     * @brief Set if all objects is visible or not
     */
    void setVisible(bool visible);

    /**
     * @brief Set if the specified object is visible or not
     */
    void setVisible(const size_t& objectIndex,
                            bool visible);
    /**
     * @brief Returns if the specified object is selected or not
     * @warning If method "isInfosAvailable" return false this method will return an
     *          invalid value and generate an assert in debug mode
     */
    bool isSelected(const size_t& objectIndex) const;

    /**
     * @brief Set if all objects is selected or not
     */
    void setSelected(bool selected);

    /**
     * @brief Set if the specified object is selected or not
     */
    void setSelected(const size_t& objectIndex,
                             bool selected);

    /**
     * @brief Returns if the flag of the specified object is on or not
     * @warning If method "isInfosAvailable" return false this method will return an
     *          invalid value and generate an assert in debug mode
     */
    bool isFlagOn(const size_t& objectIndex,
                  const GraphicsObjectFlag& flag) const;

    /**
     * @brief Returns the complete flags value of the specified object
     */
    GraphicsObjectFlags getFlagsValue(const size_t& objectIndex) const;


    /**
     * @brief Set one or multiple flags on/off in the same time for all objects.
     */
    void setFlags(const GraphicsObjectFlags& flags,
                  bool on);

    /**
     * @brief Set one or multiple flags on/off in the same time for the specified object.
     */
    void setFlags(const size_t& objectIndex,
                  const GraphicsObjectFlags& flags,
                  bool on);
    /**
     * @brief Toggle one or multiple flags on/off in the same time for all objects.
     */
    void toggleFlags(const GraphicsObjectFlags& flags);

    /**
     * @brief Toggle one or multiple flags on/off in the same time for the specified object.
     */
    void toggleFlags(const size_t& objectIndex,
                     const GraphicsObjectFlags& flags);

private:
    IChunk*                     m_chunk;
    Basic::AbstractColorCloud*  m_colorcloud;
    Basic::AbstractNormalCloud* m_normalcloud;
    Basic::AbstractInfoCloud*   m_infocloud;
    size_t                      m_from;
    size_t                      m_count;
};

#endif // DM_OBJECTSMODIFIER_H
