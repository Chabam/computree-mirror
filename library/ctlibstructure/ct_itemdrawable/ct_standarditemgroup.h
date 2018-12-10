#ifndef CT_STANDARDITEMGROUP_H
#define CT_STANDARDITEMGROUP_H

#include "ct_itemdrawable/tools/drawmanager/ct_standardstandarditemgroupdrawmanager.h"
#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"
#include "ct_result/abstract/ct_abstractresult.h"
#include "ct_tools/ct_containert.h"

#include "ct_model/outModel/abstract/def_ct_abstractgroupmodelout.h"
#include "ct_model/outModel/abstract/def_ct_abstractitemdrawablemodelout.h"
#include "ct_model/inModel/abstract/def_ct_abstractgroupmodelin.h"
#include "ct_model/inModel/abstract/def_ct_abstractitemdrawablemodelin.h"

#include "ct_iterator/ct_multicollectioniteratorstdstyle.h"
#include "ct_tools/ct_handleiteratort.h"

/**
 * @brief Represent a group that can contains other groups or other singular item (leaf). Its a node in a tree structure of items.
 *
 * The advantage is that you can process groups in multithread. Per example if you want to convert all CT_PointCluster contained in groups to
 * a CT_Circle you can process each group with a different thread.
 */
class CTLIBSTRUCTURE_EXPORT CT_StandardItemGroup : public CT_AbstractItemDrawable, public IGroupForModel
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_StandardItemGroup, CT_AbstractItemDrawable, Group)

    using SuperClass = CT_AbstractItemDrawable;

protected:
    /**
     * @brief Container for a singular item that manage the life in memory of one singular item
     *        and can set if it must be removed later or not from this group.
     */
    struct SingularItemContainer
    {
    public:
        struct InternalIterator : public std::iterator<std::input_iterator_tag, const CT_AbstractSingularItemDrawable*> {
        public:
            using self_type = InternalIterator;

            InternalIterator() : m_currentValue(NULL) {}
            InternalIterator(const InternalIterator& other) = default;
            InternalIterator(const CT_AbstractSingularItemDrawable* item) : m_currentValue(const_cast<CT_AbstractSingularItemDrawable*>(item)) {}

            self_type& operator++() {
                m_currentValue = NULL;
                return *this;
            }

            reference operator*() { return m_currentValue; }
            pointer operator->() { return &m_currentValue; }
            bool operator==(const self_type& rhs) { return m_currentValue == rhs.m_currentValue; }
            bool operator!=(const self_type& rhs) { return m_currentValue != rhs.m_currentValue; }

        private:
            const CT_AbstractSingularItemDrawable* m_currentValue;
        };

        using const_iterator = InternalIterator;

        SingularItemContainer(CT_AbstractSingularItemDrawable* item) : m_item(item), m_willBeRemovedLater(false), m_autoDelete(true) {}
        ~SingularItemContainer() { if(m_autoDelete) { delete m_item; } }

        inline void setWillBeRemovedLater() { m_willBeRemovedLater = true; }
        inline void undoWillBeRemovedLater() { m_willBeRemovedLater = false; }
        inline bool willBeRemovedLater() const { return m_willBeRemovedLater; }

        inline CT_AbstractSingularItemDrawable* item() const { return m_item; }

        const_iterator constBegin() const { return const_iterator(m_item); }
        const_iterator constEnd() const { return const_iterator(NULL); }

        SingularItemContainer* copy()
        {
            SingularItemContainer* ct = new SingularItemContainer(m_item);
            ct->m_autoDelete = false;

            return ct;
        }

    private:
        CT_AbstractSingularItemDrawable *m_item;
        bool                            m_willBeRemovedLater;
        bool                            m_autoDelete;
    };

    using ItemContainerType = SingularItemContainer;

public:
    struct LockAccessTool {

        LockAccessTool() {
            m_mutexAccessGroup = new QMutex(QMutex::Recursive);
            m_mutexAccessItem = new QMutex(QMutex::Recursive);
        }

        ~LockAccessTool() {
            delete m_mutexAccessGroup;
            delete m_mutexAccessItem;
        }

        void lockAccess() { m_mutexAccessGroup->lock(); }
        void unlockAccess() { m_mutexAccessGroup->unlock(); }

        /**
         * @brief Mutex when acessing groups (add, remove, etc...)
         */
        QMutex*             m_mutexAccessGroup;

        /**
         * @brief Mutex when acessing items (add, remove, etc...)
         */
        QMutex*             m_mutexAccessItem;
    };

    enum RemoveLaterBit
    {
        Nothing = 0,
        RemoveLater = 1,
        ChildRemoveLater = 2
    };
    Q_DECLARE_FLAGS(RemoveLaterBits, RemoveLaterBit)

    using SingularItemVisitor = std::function<bool(const CT_AbstractSingularItemDrawable*)>;
    using GroupVisitor = std::function<bool(const CT_StandardItemGroup*)>;
    using GroupContainerType = CT_ContainerT<CT_StandardItemGroup, LockAccessTool>;

    using MultiGroupCollectionIterator = CT_MultiCollectionIteratorStdStyle<typename GroupContainerType::const_iterator, const CT_StandardItemGroup>;
    using GroupIterator = CT_HandleIteratorT<MultiGroupCollectionIterator>;

    using MultiItemCollectionIterator = CT_MultiCollectionIteratorStdStyle<typename ItemContainerType::const_iterator, const CT_AbstractSingularItemDrawable>;
    using SingularItemIterator = CT_HandleIteratorT<MultiItemCollectionIterator>;

    CT_StandardItemGroup();

    /**
     * @brief Copy constructor.
     *
     *        What is copied :
     *          - Pointer of the result and model of the original item.
     *          - ID
     *          - Pointer of base and alternative draw manager
     *          - Pointer of context
     *          - Removed later flags
     *
     *        What is initialized differently :
     *          - Parent is set to NULL
     *          - isSelected and isDisplayed is set to false
     *          - Document list is not copied
     *          - Parent container is set to NULL
     *          - Childrens was not copied
     */
    CT_StandardItemGroup(const CT_StandardItemGroup& other);

    ~CT_StandardItemGroup();

    /**
     * @brief Visit childrens but only for tree view. By default call the method "visitChildrens"
     * @return Returns true if no childrens has been visited otherwise returns the result of the visitor.
     */
    bool visitChildrensForTreeView(const ChildrensVisitor& visitor) const override;

    /**
     * @brief Visit childrens
     * @return Returns true if no childrens has been visited otherwise returns the result of the visitor.
     */
    bool visitChildrens(const ChildrensVisitor& visitor) const override;

    /**
     * @brief Returns true if this group doesn't contain any group or item that will not be removed later.
     * @warning Be conscious that if this group contains group or item that will be removed later it will not be considered by this method.
     */
    bool isEmpty() const;

    /**
     * @brief Returns the x center coordinate of this item
     */
    double centerX() const override;

    /**
     * @brief Returns the y center coordinate of this item
     */
    double centerY() const override;

    /**
     * @brief Returns the z center coordinate of this item
     */
    double centerZ() const override;

    /**
     * @brief Returns the number of groups and items to keep
     */
    int nChildrens() const override;

    /**
     * @brief Visit childrens of type CT_AbstractItemDrawable. Redefined to iterate over groups and items.
     * @return Returns true if no childrens has been visited otherwise returns the result of the visitor.
     */
    bool visitChildrensOfTypeItem(const ItemVisitor& visitor) const override;

    /**
     * @brief Returns the tool for model
     */
    IPrototypeToolForModel* prototypeToolForModel() const override;

    /**
     * @brief Returns the tool for model
     */
    IItemDrawableToolForModel* itemToolForModel() const override;

    /****** SINGULAR ITEM METHODS ********/

    /**
     * @brief Add an item to the group.
     * @param outItemHandle : the handle of the item
     * @param item : the item to add (life in memory of this item will be managed by this group)
     * @warning There is no verification made to check that this item can be added or not to this group. Be sure what you do.
     */
    template<typename OutHandleType>
    void addSingularItemWithOutHandle(const OutHandleType& outItemHandle, CT_AbstractSingularItemDrawable* item) {
        Q_ASSERT(model() != NULL);

        QMutexLocker locker(m_lockAccessTool.m_mutexAccessItem);

        // the handle can have multiple models if it was created with a result copy so we must get the model
        // that his parent match with the model of this group
        const DEF_CT_AbstractItemDrawableModelOut* outModelToUse = outItemHandle.findModelWithParent(model());

        // now we can add the item with the right model
        addSingularItemWithOutModel(outModelToUse, item);
    }

    /**
     * @brief Add an item to the group.
     * @param model : the model to set to the item and to use to add it in the collection of items
     * @param item : the item to add (life in memory of this item will be managed by this group)
     * @warning There is no verification made to check that this item can be added or not to this group. Be sure what you do.
     */
    void addSingularItemWithOutModel(const DEF_CT_AbstractItemDrawableModelOut* outModel, CT_AbstractSingularItemDrawable* item);

    /**
     * @brief Remove an item from the group. The item is not really removed but a flag is set to know that on the
     *        end of your compute (of your step) the item must be removed. This is a choice to help users which use multithreaded
     *        compute to not worry about problems of access to items removed from memory.
     *
     *        All methods that return the number of item in this group will return the number of item that has not be
     *        set to be removed later.
     * @param outItemHandle : the handle to use to get the model of the item to remove.
     * @return True if it was found and removed
     */
    template<typename OutHandleType>
    bool removeSingularItemWithOutHandle(const OutHandleType& outItemHandle) {
        Q_ASSERT(model() != NULL);

        QMutexLocker locker(m_lockAccessTool.m_mutexAccessItem);

        // the handle can have multiple models if it was created with a result copy so we must get the model
        // that his parent match with the model of this group
        const DEF_CT_AbstractItemDrawableModelOut* outModelToUse = outItemHandle.findModelWithParent(model());

        Q_ASSERT(outModelToUse != NULL);

        // now we can remove the item with the right model
        removeSingularItemWithOutModel(outModelToUse);
    }

    /**
     * @brief Remove an item from the group. The item is not really removed but a flag is set to know that on the
     *        end of your compute (of your step) the item must be removed. This is a choice to help users which use multithreaded
     *        compute to not worry about problems of access to items removed from memory.
     *
     *        All methods that return the number of item in this group will return the number of item that has not be
     *        set to be removed later.
     * @param outModel : the model of the item to remove. The model of the item will be used to remove it from the collection.
     * @return True if it was found and removed
     */
    bool removeSingularItemWithOutModel(const DEF_CT_AbstractItemDrawableModelOut* outModel);

    /**
     * @brief Remove an item from the group. The item is not really removed but a flag is set to know that on the
     *        end of your compute (of your step) the item must be removed. This is a choice to help users which use multithreaded
     *        compute to not worry about problems of access to items removed from memory.
     *
     *        All methods that return the number of item in this group will return the number of item that has not be
     *        set to be removed later.
     * @param item : the item to remove. The model of the item will be used to remove it from the collection.
     * @return True if it was found and removed
     */
    bool removeSingularItem(const CT_AbstractSingularItemDrawable* item);

    /**
     * @brief Returns true if this group contains an item that use at least one of the output model in the selected possibilities of the input model.
     */
    bool containsSingularItemInSelectedPossibilitiesOfInModel(const DEF_CT_AbstractItemDrawableModelIn* inModel) const;

    /**
     * @brief Returns true if the group contains an item that use the specified output model
     */
    bool containsSingularItemWithOutModel(const DEF_CT_AbstractItemDrawableModelOut* outModel) const;

    /**
     * @brief Returns the number of items in this group that must not be removed later.
     */
    int nSingularItem() const;

    /**
     * @brief Returns the number of items in this group that must be removed later.
     */
    int nSingularItemToBeRemovedLater() const;

    /**
     * @brief Returns the number of items in this group to keep or to be removed later.
     */
    int nSingularItemInTotal() const;

    /**
     * @brief Visit all singular items that will not be removed
     * @param visitor : the visitor to use
     * @return Returns true if no items has been visited otherwise returns the result of the visitor.
     */
    bool visitSingularItems(const SingularItemVisitor& visitor) const;

    /**
     * @brief Returns the collection of singular item. You can use the begin and end method to iterate
     *        over item inside the collection
     * @return An object that you can use to iterate over items
     */
    SingularItemIterator singularItems() const;

    /**
     * @brief Visit ony new singular items that will not be removed
     * @param visitor : the visitor to use
     * @return Returns true if no items has been visited otherwise returns the result of the visitor.
     */
    bool visitNewSingularItems(const SingularItemVisitor& visitor) const;

    /**
     * @brief Returns the collection of new singular item. You can use the begin and end method to iterate
     *        over item inside the collection
     * @return An object that you can use to iterate over items
     */
    SingularItemIterator newSingularItems() const;

    /**
     * @brief Visit only backup singular items that will not be removed
     * @param visitor : the visitor to use
     * @return Returns true if no items has been visited otherwise returns the result of the visitor.
     */
    bool visitBackupSingularItems(const SingularItemVisitor& visitor) const;

    /**
     * @brief Returns the collection of backup singular item. You can use the begin and end method to iterate
     *        over item inside the collection
     * @return An object that you can use to iterate over items
     */
    SingularItemIterator backupSingularItems() const;

    /**
     * @brief Returns the singular item that use the specified output model
     * @param outModel : the model of the item to find. The model of the item will be used to find it in the collection.
     * @return NULL if no item that use this model has been found or if it must be removed later
     */
    CT_AbstractSingularItemDrawable* singularItemWithOutModel(const DEF_CT_AbstractItemDrawableModelOut* outModel) const;

    /**
     * @brief Visit all singular items that will not be removed and that use output model of selected possibilities of the specified input model
     * @param inModel : the input model to use to visit selected possibilities to get output models
     * @param visitor : the visitor to use
     * @return Returns true if no items has been visited otherwise returns the result of the visitor.
     */
    bool visitSingularItemsInSelectedPossibilitiesOfInModel(const DEF_CT_AbstractItemDrawableModelIn* inModel, const SingularItemVisitor &visitor) const;

    /****** GROUPS METHODS ********/

    /**
     * @brief Add a group to the group.
     * @param outGroupHandle : the handle of the group
     * @param group : the group to add (life in memory of this group will be managed by this group)
     * @warning There is no verification made to check that this group can be added or not to this group. Be sure what you do.
     */
    template<typename OutHandleType>
    void addGroupWithOutHandle(const OutHandleType& outGroupHandle, CT_StandardItemGroup* group) {
        QMutexLocker locker(m_lockAccessTool.m_mutexAccessGroup);

        Q_ASSERT(model() != NULL);

        // the handle can have multiple models if it was created with a result copy so we must get the model
        // that his parent match with the model of this group
        const DEF_CT_AbstractGroupModelOut* outModelToUse = outGroupHandle.findModelWithParent(model());

        // now we can add the group with the right model
        addGroupWithOutModel(outModelToUse, group);
    }

    /**
     * @brief Add an group to the group.
     * @param outModel : the model to set to the group and to use to add it in the collection of groups
     * @param group : the group to add (life in memory of this group will be managed by this group)
     * @warning There is no verification made to check that this group can be added or not to this group. Be sure what you do.
     */
    void addGroupWithOutModel(const DEF_CT_AbstractGroupModelOut* outModel, CT_StandardItemGroup* group);

    /**
     * @brief Remove a group from the group. The group is not really removed but a flag is set to know that on the
     *        end of your compute (of your step) the group must be removed. This is a choice to help users which use multithreaded
     *        compute to not worry about problems of access to groups removed from memory.
     *
     *        All methods that return the number of group in this group will return the number of group that has not be
     *        set to be removed later.
     * @param outGroupHandle : the handle to use to get the model of the group to remove.
     * @return True if it was found and removed
     */
    template<typename OutHandleType>
    bool removeGroupWithOutHandle(const OutHandleType& outGroupHandle) {
        Q_ASSERT(model() != NULL);

        QMutexLocker locker(m_lockAccessTool.m_mutexAccessGroup);

        // the handle can have multiple models if it was created with a result copy so we must get the model
        // that his parent match with the model of this group
        const DEF_CT_AbstractGroupModelOut* outModelToUse = outGroupHandle.findModelWithParent(model());

        Q_ASSERT(outModelToUse != NULL);

        // now we can remove the group with the right model
        removeGroupsWithOutModel(outModelToUse);
    }

    /**
     * @brief Remove all groups from the group. Groups was not really removed but a flag is set to know that on the
     *        end of your compute (of your step) groups must be removed. This is a choice to help users which use multithreaded
     *        compute to not worry about problems of access to groups removed from memory.
     *
     *        All methods that return the number of group in this group will return the number of group that has not be
     *        set to be removed later.
     * @param outModel : the model of groups to remove. The model of groups will be used to remove it from the collection.
     * @return True if it was found and removed
     */
    bool removeGroupsWithOutModel(const DEF_CT_AbstractGroupModelOut* outModel);

    /**
     * @brief Remove a group from the group. The group is not really removed but a flag is set to know that on the
     *        end of your compute (of your step) the group must be removed. This is a choice to help users which use multithreaded
     *        compute to not worry about problems of access to groups removed from memory.
     *
     *        All methods that return the number of group in this group will return the number of group that has not be
     *        set to be removed later.
     * @param group : the group to remove. The model of the group will be used to remove it from the collection.
     * @return True if it was found and removed
     */
    bool removeGroup(CT_StandardItemGroup* group);

    /**
     * @brief Visit all groups that will not be removed
     * @param visitor : the visitor to use
     * @return Returns true if no items has been visited otherwise returns the result of the visitor.
     */
    bool visitGroups(const GroupVisitor& visitor) const;

    /**
     * @brief Returns the collection of groups. You can use the begin and end method to iterate
     *        over group inside the collection
     * @return An object that you can use to iterate over groups
     */
    GroupIterator groups() const;

    /**
     * @brief Visit only new groups that will not be removed
     * @param visitor : the visitor to use
     * @return Returns true if no items has been visited otherwise returns the result of the visitor.
     */
    bool visitNewGroups(const GroupVisitor& visitor) const;

    /**
     * @brief Returns the collection of new groups. You can use the begin and end method to iterate
     *        over group inside the collection
     * @return An object that you can use to iterate over groups
     */
    GroupIterator newGroups() const;

    /**
     * @brief Visit only backup groups that will not be removed
     * @param visitor : the visitor to use
     * @return Returns true if no items has been visited otherwise returns the result of the visitor.
     */
    bool visitBackupGroups(const GroupVisitor& visitor) const;

    /**
     * @brief Returns the collection of backup groups. You can use the begin and end method to iterate
     *        over group inside the collection
     * @return An object that you can use to iterate over groups
     */
    GroupIterator backupGroups() const;

    /**
     * @brief Returns the number of groups in this group that must not be removed later.
     */
    int nGroup() const;

    /**
     * @brief Returns the number of groups in this group that must be removed later.
     */
    int nGroupToBeRemovedLater() const;

    /**
     * @brief Returns the number of groups in this group to keep or to be removed later.
     */
    int nGroupInTotal() const;

    /**
     * @brief Returns true if the group contains a group that use the specified output model
     */
    bool containsGroupWithOutModel(const DEF_CT_AbstractGroupModelOut* outModel) const;

    /**
     * @brief Returns the parentGroup of this group
     * @return NULL if the group is a root group (the parent group is the result)
     */
    CT_StandardItemGroup* parentGroup() const;

    /**** OTHERS ****/

    /**
     * @brief Returns true if the group will be removed at the end of the compute of your step
     */
    bool willBeRemovedLater() const;

    /**
     * @brief Returns true if at least one child must be removed later
     */
    bool atLeastOneChildMustBeRemovedLater() const;

    /**
     * @brief Set the parent group
     * @warning Not intended for direct use by plugin developper
     */
    void setParentGroup(const CT_StandardItemGroup* pGroup);

    /**
     * @brief Set the container which contains this group
     * @warning Not intended for direct use by plugin developper
     */
    void setParentContainer(const GroupContainerType* container);

    /**
     * @brief Define a object to store in the group like a context for multithread processing. You must
     *        manage the life of this object in memory !
     */
    void setContext(const void* context);

    /**
     * @brief Returns the object stored in this group
     */
    void* getContext() const;

    /**
     * @brief Called from result to inform that group that it must be removed later. Inform parent that this
     *        group must be removed later.
     */
    void setBeRemovedLater();

    /**
     * @brief Called from children to inform parent that at least one child must be removed later
     */
    void setAtLeastOneChildMustBeRemovedLater();

    /**
     * @brief Called from result to inform that group must undo to be removed later
     */
    void undoWillBeRemovedLater();

    /**
     * @brief Called from parent or result to delete all groups and items that must be removed later (recursively)
     * @warning Not intended for direct use by plugin developper
     */
    virtual void recursiveSetComplete();

    /**
     * @brief Called from result or parent group to inform that this item will be deleted from the result passed in parameter
     */
    void willBeRemovedFromResult(const CT_AbstractResult* res) override;

    /**
     * @brief Returns a copy of this group that will use the new model and the new result. If some child groups
     *        or items was not present in collection of children of the new model they will not be copied
     * @param model : the new model to use
     * @param result : the new result to set to the copy
     * @return A copy of this group but if some child groups or items was not present in collection of children
     *         of the new model they will not be copied
     */
    CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel* model, const CT_AbstractResult* result) const override;

protected:
    using OutModelKeyType = CT_OutAbstractModel*;

    using GroupsCollection = QHash<OutModelKeyType, GroupContainerType*>;
    using ItemsCollection = QHash<OutModelKeyType, ItemContainerType*>;

    using GroupContainerCollectionIterator = QHashIterator<OutModelKeyType, GroupContainerType*>;
    using ItemContainerCollectionIterator = QHashIterator<OutModelKeyType, ItemContainerType*>;

    /**
     * @brief Convert the specified out model to a key for use with a GroupsCollection
     */
    template<class OutModel>
    static inline OutModelKeyType outModelKey(const OutModel* model) {
        return const_cast<OutModel*>(model);
    }

    /**
     * @brief Convert the specified out model copy to a key for use with a GroupsCollection
     */
    template<class OutModel>
    static inline OutModelKeyType outModelCopyKey(const OutModel* modelCopy) {
        return static_cast<OutModel*>(modelCopy->originalModel());
    }

    /**
     * @brief Return the container which contains this group
     */
    GroupContainerType* parentContainer() const;

    /**
     * @brief Copy all childrens (groups and items) to the specified group if they are still present in the model of the specified group
     * @param itemGroup : the group where childrens must be copied
     */
    void copyChildrensToGroupIfTheyAreStillPresentInModel(CT_StandardItemGroup* itemGroup) const;

    /**
     * @brief Copy all children groups from a source to the backup collection if their are still present in the children of the parent group
     * @param result : the result to set to new group copied
     * @param parentGroup : the reference to the parent group that will be set to new groups (copy) and used to get the parent group model and the backup collection
     * @param mapSource : the source that contains real childrens to use to make a copy if the child are still present in the parent group
     */
    static void staticCopyGroupsFromXToBackupIfChildrenGroupsAreStillPresent(const CT_AbstractResult* result,
                                                                             CT_StandardItemGroup& parentGroup,
                                                                             const GroupsCollection& mapSource);

    /**
     * @brief Copy all children items from a source to the backup collection if their are still present in the children of the parent group
     * @param parentGroup : the reference to the parent group that will be used to get the parent group model and the backup collection
     * @param mapSource : the source that contains real childrens to use to make a copy if the child are still present in the parent group
     */
    static void staticCopyItemsFromXToBackupIfChildrenItemsAreStillPresent(const CT_AbstractResult* result,
                                                                           CT_StandardItemGroup &parentGroup,
                                                                           const ItemsCollection& mapSource);

    /**
     * @brief Create and return a qt style iterator to iterate over childrens (groups or items or etc...) that use the specified out model
     * @param outModel : the out model of childrens
     * @return A new qt style iterator to iterate over childrens (groups or items or etc...) that use the specified out model
     */
    IChildrensIteratorQtStyleSharedPtr createQtStyleIteratorForChildrensThatUseOutModel(const CT_OutAbstractModel* outModel) const override;

private:

    class GroupQtIterator : public IChildrensIteratorQtStyle {
    public:
        GroupQtIterator(GroupContainerType::iterator begin, GroupContainerType::iterator end) : m_begin(begin), m_end(end) {}

        bool hasNext() const override { return m_begin != m_end; }

        CT_AbstractItem* next() override {
            CT_AbstractItem* c = (*m_begin);
            m_begin++;
            return c;
        }

    private:
        GroupContainerType::iterator m_begin;
        GroupContainerType::iterator m_end;
    };

    class ItemQtIterator : public IChildrensIteratorQtStyle {
    public:
        ItemQtIterator(const CT_AbstractItemDrawable* item) : m_current(const_cast<CT_AbstractItemDrawable*>(item)) {}

        bool hasNext() const override { return m_current != NULL; }

        CT_AbstractItem* next() override {
            CT_AbstractItem* c = m_current;
            m_current = NULL;
            return c;
        }

    private:
        CT_AbstractItemDrawable* m_current;
    };

    const static CT_StandardStandardItemGroupDrawManager SIG_DRAW_MANAGER;

    LockAccessTool      m_lockAccessTool;

    /**
     * @brief Collection of new groups added to this group
     */
    GroupsCollection    m_newGroupsAdded;

    /**
     * @brief If this group is a copy this collection will contains all child groups copied from the original group
     */
    GroupsCollection    m_groupsCopied;

    /**
     * @brief Collection of new items added to this group
     */
    ItemsCollection     m_newItemsAdded;

    /**
     * @brief If this group is a copy this collection will contains all child items copied from the original group
     */
    ItemsCollection     m_itemsCopied;

    /**
     * @brief Flags
     */
    RemoveLaterBits     m_removedLaterFlags;  // quint8 : first bit is set if the group must be removed later, second bit is set if at least one child must be removed later

    /**
     * @brief The container which this group has been added
     */
    GroupContainerType* m_parentContainer;

    /**
     * @brief A context for multithread processing if necessary
     */
    void*               m_context;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(CT_StandardItemGroup::RemoveLaterBits)

#endif // CT_STANDARDITEMGROUP_H
