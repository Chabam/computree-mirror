#ifndef CT_READEROUTMODELSTRUCTUREMANAGER_H
#define CT_READEROUTMODELSTRUCTUREMANAGER_H

#include "ctlibreader_global.h"

#include "ct_model/outModel/manager/ct_outmodelstructuremanager.h"
#include "ct_model/inModel/ct_instdgroupmodel.h"
#include "ct_model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_handle/ct_handleoutstdgroup.h"

#include "tools/sfinae.h"

class CTLIBREADER_EXPORT CT_ReaderOutModelStructureManager
{
public:
    using ToolToModifyResultModelCopiesType = CT_InResultModelGroupToCopy::ToolToModifyResultModelCopiesType;

    CT_ReaderOutModelStructureManager(CT_OutModelStructureManager& manager, CT_HandleOutStdGroup& hOutGroup) :
        m_manager(manager),
        m_hOutGroup(&hOutGroup),
        m_inParentModel(nullptr),
        m_inTool(nullptr)
    {
    }

    CT_ReaderOutModelStructureManager(CT_OutModelStructureManager& manager,
                                      const CT_InStdGroupModel* inParentModel,
                                      const ToolToModifyResultModelCopiesType* inTool) :
        m_manager(manager),
        m_hOutGroup(nullptr),
        m_inParentModel(const_cast<CT_InStdGroupModel*>(inParentModel)),
        m_inTool(const_cast<ToolToModifyResultModelCopiesType*>(inTool))
    {
    }

    template<class HandleGroup>
    static CT_ReaderOutModelStructureManager createFromHandle(CT_OutModelStructureManager& manager,
                                                              HandleGroup& hParentGroup)
    {
        return internalCreateFromHandle(manager,
                                        hParentGroup,
                                        std::integral_constant<bool, IsAnOutputModel<HandleGroup::ModelType>::Is>());
    }

    template<class HandleInGroup>
    static CT_ReaderOutModelStructureManager createFromInHandle(CT_OutModelStructureManager& manager, HandleInGroup& hInGroup)
    {
        using InResultModelCopyType = typename HandleInGroup::InResultModelCopyType;

        CT_InStdGroupModel* inParentModel = hInGroup.model();

        Q_ASSERT(inParentModel != nullptr);

        InResultModelCopyType* inResultModelCopy = dynamic_cast<InResultModelCopyType*>(inParentModel->rootModel());

        Q_ASSERT(inResultModelCopy != nullptr);

        ToolToModifyResultModelCopiesType* inTool = inResultModelCopy->toolToModifyResultModelCopies();

        Q_ASSERT(inTool != nullptr);

        return CT_ReaderOutModelStructureManager(manager, inParentModel, inTool);
    }

    /**
     * @brief Add a group to the root group
     * @param groupHandle : the handle of the output group model to use to create the new group model and access it later
     * @param displayableName : the displayable that must be set to the new group model
     * @param shortDescription : the short description that must be set to the new group model
     * @param detailledDescription : the detailled description that must be set to the new group model
     */
    template<class HandleOutGroup>
    void addGroup(HandleOutGroup& groupHandle,
                  const QString& displayableName = QString{"Out Group"},
                  const QString& shortDescription = QString{""},
                  const QString& detailledDescription = QString{""},
                  typename HandleOutGroup::GroupType* prototype = nullptr) {

        if(m_hOutGroup != nullptr) {
            this->addGroup(*m_hOutGroup,
                           groupHandle,
                           displayableName,
                           shortDescription,
                           detailledDescription,
                           prototype);
            return;
        }

        m_manager.addGroupWithInputTool(m_inParentModel,
                                        m_inTool,
                                        groupHandle,
                                        displayableName,
                                        shortDescription,
                                        detailledDescription,
                                        prototype);
    }

    /**
     * @brief Add a group to another group model
     * @param parentGroup : the handle of an output group model to use to add the new group
     * @param groupHandle : the handle of the output group model to use to create the new group model and access it later
     * @param displayableName : the displayable that must be set to the new group model
     * @param shortDescription : the short description that must be set to the new group model
     * @param detailledDescription : the detailled description that must be set to the new group model
     */
    template<class HandleGroupParent, class HandleOutGroup>
    void addGroup(const HandleGroupParent& parentGroup,
                  HandleOutGroup& groupHandle,
                  const QString& displayableName = QString{"Out Group"},
                  const QString& shortDescription = QString{""},
                  const QString& detailledDescription = QString{""},
                  typename HandleOutGroup::GroupType* prototype = nullptr) {
        m_manager.addGroup(parentGroup,
                           groupHandle,
                           displayableName,
                           shortDescription,
                           detailledDescription,
                           prototype);
    }

    /**
     * @brief Add an item to the root group
     * @param parentGroup : the handle of an output group model to use to add the new group
     * @param itemHandle : the handle of the output item model to use to create the new item model and access it later
     * @param displayableName : the displayable that must be set to the new item model
     * @param shortDescription : the short description that must be set to the new item model
     * @param detailledDescription : the detailled description that must be set to the new item model
     */
    template<class HandleOutItem>
    void addItem(HandleOutItem& itemHandle,
                 const QString& displayableName = QString{"Out Item"},
                 const QString& shortDescription = QString{""},
                 const QString& detailledDescription = QString{""},
                 typename HandleOutItem::ItemType* prototype = nullptr) {
        if(m_hOutGroup != nullptr) {
            this->addItem(*m_hOutGroup,
                          itemHandle,
                          displayableName,
                          shortDescription,
                          detailledDescription,
                          prototype);
            return;
        }

        m_manager.addItemWithInputTool(m_inParentModel,
                                       m_inTool,
                                       itemHandle,
                                       displayableName,
                                       shortDescription,
                                       detailledDescription,
                                       prototype);
    }

    /**
     * @brief Add an item to a group model
     * @param parentGroup : the handle of an output group model to use to add the new group
     * @param itemHandle : the handle of the output item model to use to create the new item model and access it later
     * @param displayableName : the displayable that must be set to the new item model
     * @param shortDescription : the short description that must be set to the new item model
     * @param detailledDescription : the detailled description that must be set to the new item model
     */
    template<class HandleGroupParent, class HandleOutItem>
    void addItem(const HandleGroupParent& parentGroup,
                 HandleOutItem& itemHandle,
                 const QString& displayableName = QString{"Out Item"},
                 const QString& shortDescription = QString{""},
                 const QString& detailledDescription = QString{""},
                 typename HandleOutItem::ItemType* prototype = nullptr) {
        m_manager.addItem(parentGroup,
                          itemHandle,
                          displayableName,
                          shortDescription,
                          detailledDescription,
                          prototype);
    }

    /**
     * @brief Add an item attribute to an item model
     * @param parentItem : the handle of an output item model to use to add the new item attribute
     * @param itemAttributeHandle : the handle of the output item attribute model to use to create the new item attribute model and access it later
     * @param category : a category, per example PS_CATEGORY_MANAGER->findByUniqueName(CT_AbstractCategory::DATA_VALUE)
     * @param displayableName : the displayable that must be set to the new item attribute model
     * @param shortDescription : the short description that must be set to the new item attribute model
     * @param detailledDescription : the detailled description that must be set to the new item attribute model
     */
    template<class HandleItemParent, class HandleOutItemAttribute, typename Category>
    void addItemAttribute(const HandleItemParent& parentItem,
                          HandleOutItemAttribute& itemAttributeHandle,
                          const Category& category,
                          const QString& displayableName = QString{"Out Item Attribute"},
                          const QString& shortDescription = QString{""},
                          const QString& detailledDescription = QString{""},
                          typename HandleOutItemAttribute::ItemAttributeType* prototype = nullptr) {

        m_manager.addItemAttribute(parentItem,
                                   itemAttributeHandle,
                                   category,
                                   displayableName,
                                   shortDescription,
                                   detailledDescription,
                                   prototype);
    }

    /**
     * @brief Add an item attribute to an item model
     * @param parentItem : the handle of an output item model to use to add the new item attribute
     * @param itemAttributeHandle : the handle of the output item attribute model to use to create the new item attribute model and access it later
     * @param category : a category string from category manager, per example CT_AbstractCategory::DATA_VALUE
     * @param displayableName : the displayable that must be set to the new item attribute model
     * @param shortDescription : the short description that must be set to the new item attribute model
     * @param detailledDescription : the detailled description that must be set to the new item attribute model
     */
    template<class HandleItemParent, class HandleOutItemAttribute>
    void addItemAttributeAndFindCategory(const HandleItemParent& parentItem,
                                         HandleOutItemAttribute& itemAttributeHandle,
                                         const QString& category,
                                         const QString& displayableName = QString{"Out Item Attribute"},
                                         const QString& shortDescription = QString{""},
                                         const QString& detailledDescription = QString{""},
                                         typename HandleOutItemAttribute::ItemAttributeType* prototype = nullptr) {

        m_manager.addItemAttribute(parentItem,
                                   itemAttributeHandle,
                                   PS_CATEGORY_MANAGER->findByUniqueName(category),
                                   displayableName,
                                   shortDescription,
                                   detailledDescription,
                                   prototype);
    }

private:
    CT_OutModelStructureManager&            m_manager;
    CT_HandleOutStdGroup*                   m_hOutGroup;
    CT_InStdGroupModel*                     m_inParentModel;
    ToolToModifyResultModelCopiesType*      m_inTool;

    template<class HandleGroup>
    static CT_ReaderOutModelStructureManager internalCreateFromHandle(CT_OutModelStructureManager& manager,
                                                                      HandleGroup& hParentGroup,
                                                                      std::true_type)
    {
        return CT_ReaderOutModelStructureManager(manager, hParentGroup);
    }

    template<class HandleGroup>
    static CT_ReaderOutModelStructureManager internalCreateFromHandle(CT_OutModelStructureManager& manager,
                                                                      HandleGroup& hParentGroup,
                                                                      std::false_type)
    {
        return createFromInHandle(manager, hParentGroup);
    }
};

#endif // CT_READEROUTMODELSTRUCTUREMANAGER_H
