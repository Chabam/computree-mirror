#ifndef PERMANENTITEMINFORMATION_H
#define PERMANENTITEMINFORMATION_H

#include <QList>
#include <QHash>

#include "scene/permanentsceneelementtype.h"

class PermanentItemScene;
class PermanentScene;
class IChunk;
class DispatchInformation;

/**
 * @brief Keep information of an Item to remove it after from a scene
 */
class PermanentItemInformation
{
public:

    typedef QHash<IChunk*, DispatchInformation*>                ObjectCollection;
    typedef QHashIterator<IChunk*, DispatchInformation*>        ObjectCollectionIterator;

    /**
     * @brief Create the information
     * @param index : index of this object in your list (unique index)
     * @param scene : scene to use
     */
    PermanentItemInformation(const quint32& index);
    ~PermanentItemInformation();

    /**
     * @brief Change index
     */
    void setIndex(const quint32& index);

    /**
     * @brief Returns the unique index of this object
     */
    const quint32& getIndex() const;

    /**
     * @brief Add an object
     * @param type : type of the object
     * @param size : number of object added
     * @param chunk : the chunk that contains this object
     * @param calledAfterInserted : set to true if you have already inserted the object in the scene before call this method, false otherwise
     */
    void addObject(Scene::ObjectType type,
                   const size_t& size,
                   const IChunk* chunk,
                   bool calledAfterInserted = true);

    /**
     * @brief Clear this object
     */
    void clear();

    /**
     * @brief Reset this object
     */
    void reset();

    /**
     * @brief Create if not exist or get the object information for a specific chunk
     * @param chunk : the chunk
     * @return The object information to use for the specified chunk
     */
    DispatchInformation& createOrGetObjectForChunk(const IChunk* chunk);

    /**
     * @brief Returns the object information for a specific chunk
     * @param chunk : the chunk
     * @return The object information for the specified chunk if founded, otherwise returs nullptr
     */
    DispatchInformation* getObjectInformationForChunk(const IChunk* chunk) const;

    /**
     * @brief Remove all shapes and points of this object from the scene
     */
    void removeFromScene();

    /**
     * @brief Show/Hide all shapes and points of this object in the scene
     * @param visible : true if you want to set visible, false to hide it
     */
    void setVisible(bool visible);

    /**
     * @brief Check if at least one object is visible in the scene
     */
    bool isVisible();

    /**
     * @brief Returns collection of objects
     */
    const ObjectCollection& getObjectsCollection() const;

    /**
     * @brief Returns collection of objects that you can modify
     */
    ObjectCollection& getObjectsCollection();

private:

    quint32             m_index;
    ObjectCollection    m_objects;
};

#endif // PERMANENTITEMINFORMATION_H
