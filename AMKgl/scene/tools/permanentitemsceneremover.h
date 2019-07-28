#ifndef PERMANENTITEMSCENEREMOVER_H
#define PERMANENTITEMSCENEREMOVER_H

#include "scene/permanentitemscene.h"
#include "scene/tools/dispatchinformation.h"

/**
 * @brief Use this class to remove effectively items from a scene (without blocking the user interface).
 *
 * When you add an item to this class it will first be hidden. While the scene is not drawn elements will be removed
 * one by one from the scene and all PermanentItemInformation will be updated. When the scene is draw the remove will be
 * temporary paused and will restart when draw is finished. If the user move fast the camera of the view, the draw method
 * of the scene was called frequently and this object has no time to remove elements. To prevent this the algorithm pause
 * the deletion only if at least X ms was past between two draw calls.
 */
class PermanentItemSceneRemover : public QObject
{
    Q_OBJECT

public:
    PermanentItemSceneRemover(PermanentItemScene& scene);
    virtual ~PermanentItemSceneRemover();

    /**
     * @brief Add an item that must be removed from the scene.
     * @param item : item to remove
     */
    void addItemToRemove(const Item *item);

    /**
     * @brief Add an item that must be updated.
     * @param item : item to update
     */
    void addItemToUpdate(const Item *item, GraphicsView& graphicsView, const QColor &color);

    /**
     * @brief Remove all elements added
     */
    void compute();

    /**
     * @brief Remove all items from the scene
     */
    void clearItems();

    /**
     * @brief Returns true if it has some items to remove or update
     */
    bool mustBeLaunched() const;

private:

    class MergedItemInformation {

    public:
        MergedItemInformation() { m_lastChunkUsed = nullptr; m_lastVectorUsed = nullptr; }
        ~MergedItemInformation() { clear(); }

        struct MergedObjectInformation {
            MergedObjectInformation() : mbegin(0), mend(0) {}
            MergedObjectInformation(const DispatchInformation& obj) : mbegin(obj.begin()), mend(obj.end()) {}

            /**
             * @brief Begin index in the chunk of the first object
             */
            size_t  mbegin;

            /**
             * @brief Last object + 1 or Next object begin
             */
            size_t  mend;

            inline bool operator< ( const MergedObjectInformation& s ) const {
                return begin() < s.begin();
            }

            inline size_t begin() const { return mbegin; }
            inline size_t end() const { return mend; }
            inline size_t count() const { return (mend-mbegin)+1; }

            inline void mergeWith(const MergedObjectInformation& newV) {
                mend = qMax(this->end(), newV.end());
                mbegin = qMin(newV.begin(), this->begin());
            }
        };

        struct VectorOfMergedObjectInformation {
            typedef std::vector<MergedObjectInformation> Collection;
            typedef Collection::iterator iterator;

            VectorOfMergedObjectInformation() : totalCount(0) {}

            size_t      totalCount;
            Collection  collection;
        };

        VectorOfMergedObjectInformation* createOrGetVectorForChunk(const IChunk* chunk);

        void mergeChunkWith(const IChunk* chunk, const MergedObjectInformation& toAdd, const quint32& index);

        void clear();

        typedef QHash<IChunk*, VectorOfMergedObjectInformation*>            ObjectCollection;
        typedef QHashIterator<IChunk*, VectorOfMergedObjectInformation*>    ObjectCollectionIterator;

        ObjectCollection                    m_objects;
        IChunk*                             m_lastChunkUsed;
        VectorOfMergedObjectInformation*    m_lastVectorUsed;
    };

struct Backup {
        Item*                           item;
        PermanentItemInformation*       info;
        bool                            deleteIt;
        GraphicsView*                   graphicsView;
        QColor                          color;
    };

    PermanentItemScene&                 m_scene;
    MergedItemInformation               m_infoMerged;
    QMap<quint32, Backup>               m_itemsBackup;

protected:
    /**
     * @brief Return the MergedItemInformation attribute
     */
    MergedItemInformation& getMergedItemInformation();

    /**
     * @brief Merge the item information with one of the list or add it to the list
     */
    void mergeItemInformation(const quint32 &index, PermanentItemInformation* infoToMerge);

    /**
     * @brief shift the item information
     */
    void shifItemInformation(PermanentItemInformation *infoToShift, const quint32 &offsetForIndex);
};

#endif // PERMANENTITEMSCENEREMOVER_H
