#ifndef DM_GLOBALPOINTSMODIFIER_H
#define DM_GLOBALPOINTSMODIFIER_H

#include "graphicsviewinterface.h"

#include "scene/permanentitemscenebymodel.h"
#include "defineglobalcloud.h"

#include <Eigen/Dense>

/**
 * @brief Use this class to modify global points in a scene or points from a specific item in a scene
 */
class DM_GlobalPointsModifier : public ObjectsModifierInterface
{
public:
    /**
     * @brief Construct it
     * @param scene : the scene to modify
     * @param itemWithPC : item to get point cloud index (can be null if you want to apply modification to all points of the scene)
     */
    DM_GlobalPointsModifier(const PermanentItemSceneByModel* scene,
                            const CT_AbstractItemDrawable *itemWithPC,
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
     * @brief Transformation of points. Do nothing !
     */
    void transform(const Eigen::Matrix4f& trMatrix) { Q_UNUSED(trMatrix); }
    void translate(const Eigen::Vector3f& translation) { Q_UNUSED(translation); }
    void rotate(const Eigen::AngleAxis<float>& rotation) { Q_UNUSED(rotation); }
    void rotate(const Eigen::Quaternion<float>& rotation) { Q_UNUSED(rotation); }
    void scale(const Eigen::Vector3f& scaling) { Q_UNUSED(scaling); }

    void transform(const size_t& objectIndex,
                   const Eigen::Matrix4f& trMatrix) { Q_UNUSED(objectIndex) Q_UNUSED(trMatrix); }
    void translate(const size_t& objectIndex,
                   const Eigen::Vector3f& translation) { Q_UNUSED(objectIndex) Q_UNUSED(translation); }
    void rotate(const size_t& objectIndex,
                const Eigen::AngleAxis<float>& rotation) { Q_UNUSED(objectIndex) Q_UNUSED(rotation); }
    void rotate(const size_t& objectIndex,
                const Eigen::Quaternion<float>& rotation) { Q_UNUSED(objectIndex) Q_UNUSED(rotation); }
    void scale(const size_t& objectIndex,
               const Eigen::Vector3f& scaling) { Q_UNUSED(objectIndex) Q_UNUSED(scaling); }

    /**
     * @brief Returns true if colors is available, false otherwise
     */
    bool isColorsAvailable() const;

    /**
     * @brief Returns the color of the specified vertex of the specified object
     * @warning If method "isColorsAvailable" return false this method will return an
     *          invalid value and generate an assert in debug mode
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
     *          invalid value and generate an assert in debug mode
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
    AMKgl::GlobalColorCloud*    m_colorcloud;
    AMKgl::GlobalNormalCloud*   m_normalcloud;
    AMKgl::GlobalInfoCloud*     m_infocloud;
    CT_AbstractCloudIndex*      m_cloudIndex;
};

#endif // DM_GLOBALPOINTSMODIFIER_H
