#ifndef CT_INMODELSTRUCTUREMANAGER_H
#define CT_INMODELSTRUCTUREMANAGER_H

#include "ct_model/inModel/handle/ct_handleinzeroormoregroup.h"
#include "ct_model/inModel/abstract/ct_inabstractresultmodel.h"
#include "tools/sfinae.h"

#define NO_INPUT_STATIC_ASSERT_MESSAGE static_assert(false, "It seems that one of the specified handle does not contains an INPUT model. Please verify that you pass INPUT handles !")

/**
 * @brief Class that you must use to create the structure (tree) of input models and assign each
 *        created model to an handle
 */
class CTLIBMODELS_EXPORT CT_InModelStructureManager
{
public:
    virtual ~CT_InModelStructureManager();

    /**
     * @brief Set that you doesn't need input result. This will clear all result added
     *        previously to this manager and add a special result model.
     */
    void setNotNeedInputResult();

    /**
     * @brief Returns true if this manager has at least one result model and the developper has not
     *        call the method "setNotNeedInputResult".
     */
    bool needInputs() const;

    /**
     * @brief Returns true if at least one model (recursive) use the specified type
     */
    bool doesAtLeastOneInputModelsUseType(const QString& type) const;

    /**
     * @brief Add a new input result model to the structure (result are always root)
     * @param handleResult : the handle to use to create the result model and access it later
     */
    template<class HandleInResult>
    void addResult(HandleInResult& handleResult,
                   const QString& displayableName = QString{"In Result"},
                   const QString& shortDescription = QString(),
                   bool recursive = false) {

        checkNotNeedInputResultIsNotPresent();

        internalAddResult(handleResult,
                          std::integral_constant<bool, IsAnInputModel<typename HandleInResult::ModelType>::value>(),
                          displayableName,
                          shortDescription,
                          recursive);
    }

    /**
     * @brief Set a zero or more root group to the input result model
     * @param handleResult : the handle of an input result model to use to set the zero or more root group
     * @return Return the handle to use to access to the root group of the input result model
     */
    template<class GroupType, class HandleInResult>
    CT_HandleInZeroOrMoreGroup<GroupType> setZeroOrMoreRootGroup(const HandleInResult& handleResult) {
        CT_HandleInZeroOrMoreGroup<GroupType> handleRootGroup;

        checkNotNeedInputResultIsNotPresent();

        internalSetZeroOrMoreRootGroup<HandleInResult>(handleResult,
                                                       handleRootGroup,
                                                       std::integral_constant<bool, IsAnInputModel<typename HandleInResult::ModelType>::value>());

        return handleRootGroup;
    }

    /**
     * @brief Set a zero or more root group to the input result model
     * @param handleResult : the handle of an input result model to use to set the zero or more root group
     * @return Return the handle to use to access to the root group of the input result model
     */
    template<class HandleInResult, class HandleZeroOrMore>
    void setZeroOrMoreRootGroup(const HandleInResult& handleResult,
                                HandleZeroOrMore& handleRootGroup) {

        checkNotNeedInputResultIsNotPresent();

        internalSetZeroOrMoreRootGroup<HandleInResult>(handleResult,
                                                       handleRootGroup,
                                                       std::integral_constant<bool, IsAnInputModel<typename HandleInResult::ModelType>::value>());
    }

    /**
     * @brief Set a specific root group to the input result model
     * @param handleResult : the handle of an input result model to use to set the zero or more root group
     * @param rootGroupHandle : the handle of the input group model to use to create the new group model and access it later
     * @param displayableName : the displayable that must be set to the new group model
     * @param shortDescription : the short description that must be set to the new group model
     * @param detailledDescription : the detailled description that must be set to the new group model
     */
    template<class HandleInResult, class HandleInGroup>
    void setRootGroup(const HandleInResult& handleResult,
                      HandleInGroup& rootGroupHandle,
                      const QString& displayableName = QString("Group"),
                      const QString& shortDescription = QString(),
                      const QString& detailledDescription = QString()) {

        checkNotNeedInputResultIsNotPresent();

        internalSetRootGroup(handleResult,
                             rootGroupHandle,
                             std::integral_constant<bool, SFINAE_And_<IsAnInputModel<typename HandleInResult::ModelType>, IsAnInputModel<typename HandleInGroup::ModelType>>::value>(),
                             displayableName.isEmpty() ? HandleInGroup::GroupType::nameFromType(HandleInGroup::GroupType::staticType()) : displayableName,
                             shortDescription,
                             detailledDescription);
    }

    /**
     * @brief Add a group to another group model
     * @param parentGroup : the handle of the input group model to use to add the new group
     * @param groupHandle : the handle of the input group model to use to create the new group model and access it later
     * @param displayableName : the displayable that must be set to the new group model
     * @param shortDescription : the short description that must be set to the new group model
     * @param detailledDescription : the detailled description that must be set to the new group model
     */
    template<class HandleInGroupParent, class HandleInGroup>
    void addGroup(const HandleInGroupParent& parentGroup,
                  HandleInGroup& groupHandle,
                  const QString& displayableName = QString("Group"),
                  const QString& shortDescription = QString(),
                  const QString& detailledDescription = QString()) {

        checkNotNeedInputResultIsNotPresent();

        internalAddGroup(parentGroup,
                         groupHandle,
                         std::integral_constant<bool, SFINAE_And_<IsAnInputModel<typename HandleInGroupParent::ModelType>, IsAnInputModel<typename HandleInGroup::ModelType>>::value>(),
                         displayableName.isEmpty() ? HandleInGroup::GroupType::nameFromType(HandleInGroup::GroupType::staticType()) : displayableName,
                         shortDescription,
                         detailledDescription);
    }

    /**
     * @brief Add an item to a group model
     * @param parentGroup : the handle of the input group model to use to add the new group
     * @param itemHandle : the handle of the input item model to use to create the new item model and access it later
     * @param displayableName : the displayable that must be set to the new item model
     * @param shortDescription : the short description that must be set to the new item model
     * @param detailledDescription : the detailled description that must be set to the new item model
     */
    template<class HandleInGroupParent, class HandleInItem>
    void addItem(const HandleInGroupParent& parentGroup,
                 HandleInItem& itemHandle,
                 const QString& displayableName = QString(),
                 const QString& shortDescription = QString(),
                 const QString& detailledDescription = QString()) {

        checkNotNeedInputResultIsNotPresent();

        internalAddItem(parentGroup,
                        itemHandle,
                        std::integral_constant<bool, SFINAE_And_<IsAnInputModel<typename HandleInGroupParent::ModelType>, IsAnInputModel<typename HandleInItem::ModelType>>::value>(),
                        displayableName.isEmpty() ? HandleInItem::ItemType::nameFromType(HandleInItem::ItemType::staticType()) : displayableName,
                        shortDescription,
                        detailledDescription);
    }

    /**
     * @brief Add an item to a group model
     * @param parentItem : the handle of the input item model to use to add the new group
     * @param itemAttributeHandle : the handle of the input item attribute model to use to create the new item attribute model and access it later
     * @param categories : one category or a list of category. Per example : CT_AbstractCategory::DATA_VALUE or QStringList{CT_AbstractCategory::DATA_SIZE, CT_AbstractCategory::DATA_RADIUS}
     * @param displayableName : the displayable that must be set to the new item attribute model
     * @param shortDescription : the short description that must be set to the new item attribute model
     * @param detailledDescription : the detailled description that must be set to the new item attribute model
     */
    template<class HandleInItemParent, class HandleInItemAttribute, typename Categories>
    void addItemAttribute(const HandleInItemParent& parentItem,
                          HandleInItemAttribute& itemAttributeHandle,
                          const Categories& categories,
                          const QString& displayableName = QString{"In Item Attribute"},
                          const QString& shortDescription = QString(),
                          const QString& detailledDescription = QString()) {

        checkNotNeedInputResultIsNotPresent();

        internalAddItemAttribute(parentItem,
                                 itemAttributeHandle,
                                 std::integral_constant<bool, SFINAE_And_<IsAnInputModel<typename HandleInItemParent::ModelType>, IsAnInputModel<typename HandleInItemAttribute::ModelType>>::value>(),
                                 categories,
                                 displayableName,
                                 shortDescription,
                                 detailledDescription);
    }

    /**
     * @brief Add a point attribute to a group model
     * @param parentGroup : the handle of the input group model to use to add the new group
     * @param itemHandle : the handle of the input item model to use to create the new item model and access it later
     * @param displayableName : the displayable that must be set to the new item model
     * @param shortDescription : the short description that must be set to the new item model
     * @param detailledDescription : the detailled description that must be set to the new item model
     */
    template<class HandleInGroupParent, class HandleInPointAttribute>
    void addPointAttribute(const HandleInGroupParent& parentGroup,
                           HandleInPointAttribute& itemHandle,
                           const QString& displayableName = QString(),
                           const QString& shortDescription = QString(),
                           const QString& detailledDescription = QString()) {


        checkNotNeedInputResultIsNotPresent();

        internalAddItem(parentGroup,
                        itemHandle,
                        std::integral_constant<bool, SFINAE_And_<IsAnInputModel<typename HandleInGroupParent::ModelType>, IsAnInputModel<typename HandleInPointAttribute::ModelType>, HasApplicableToPoint<HandleInPointAttribute>>::value>(),
                        displayableName.isEmpty() ? HandleInPointAttribute::ItemType::nameFromType(HandleInPointAttribute::ItemType::staticType()) : displayableName,
                        shortDescription,
                        detailledDescription);
    }

    /**
     * @brief Add an edge attribute to a group model
     * @param parentGroup : the handle of the input group model to use to add the new group
     * @param itemHandle : the handle of the input item model to use to create the new item model and access it later
     * @param displayableName : the displayable that must be set to the new item model
     * @param shortDescription : the short description that must be set to the new item model
     * @param detailledDescription : the detailled description that must be set to the new item model
     */
    template<class HandleInGroupParent, class HandleInEdgeAttribute>
    void addEdgeAttribute(const HandleInGroupParent& parentGroup,
                          HandleInEdgeAttribute& itemHandle,
                          const QString& displayableName = QString(),
                          const QString& shortDescription = QString(),
                          const QString& detailledDescription = QString()) {


        checkNotNeedInputResultIsNotPresent();

        internalAddItem(parentGroup,
                        itemHandle,
                        std::integral_constant<bool, SFINAE_And_<IsAnInputModel<typename HandleInGroupParent::ModelType>, IsAnInputModel<typename HandleInEdgeAttribute::ModelType>, HasApplicableToEdge<HandleInEdgeAttribute>>::value>(),
                        displayableName.isEmpty() ? HandleInEdgeAttribute::ItemType::nameFromType(HandleInEdgeAttribute::ItemType::staticType()) : displayableName,
                        shortDescription,
                        detailledDescription);
    }

    /**
     * @brief Add a face attribute to a group model
     * @param parentGroup : the handle of the input group model to use to add the new group
     * @param itemHandle : the handle of the input item model to use to create the new item model and access it later
     * @param displayableName : the displayable that must be set to the new item model
     * @param shortDescription : the short description that must be set to the new item model
     * @param detailledDescription : the detailled description that must be set to the new item model
     */
    template<class HandleInGroupParent, class HandleInFaceAttribute>
    void addFaceAttribute(const HandleInGroupParent& parentGroup,
                          HandleInFaceAttribute& itemHandle,
                          const QString& displayableName = QString(),
                          const QString& shortDescription = QString(),
                          const QString& detailledDescription = QString()) {


        checkNotNeedInputResultIsNotPresent();

        internalAddItem(parentGroup,
                        itemHandle,
                        std::integral_constant<bool, SFINAE_And_<IsAnInputModel<typename HandleInGroupParent::ModelType>, IsAnInputModel<typename HandleInFaceAttribute::ModelType>, HasApplicableToFace<HandleInFaceAttribute>>::value>(),
                        displayableName.isEmpty() ? HandleInFaceAttribute::ItemType::nameFromType(HandleInFaceAttribute::ItemType::staticType()) : displayableName,
                        shortDescription,
                        detailledDescription);
    }

    /**
     * @brief Visit all input result model added to this manager
     * @param visitor : a lambda expression per example that return a bool (true if must continue the visit) and receive a const pointer to a CT_InAbstractResultModel
     * @return True if no results has been visited, otherwise the returned value of the visitor
     */
    template<typename Visitor>
    bool visitResults(const Visitor& visitor) const {
        for(const CT_InAbstractResultModel* resultModel : m_results) {
            if(!visitor(resultModel))
                return false;
        }

        return true;
    }

    /**
     * @brief Returns the number of results
     */
    int nResults() const;

    /**
     * @brief Clear input result model from this manager and from the memory
     * @param deleteFromMemory : set it to true if you want to remove from collection and delete from memory input result models. If
     *                           you set it to false it will only be removed from the collection.
     */
    void clearResults(bool deleteFromMemory = true);

private:
    QList<CT_InAbstractResultModel*>    m_results;

    /**
     * @brief Check that the not need input result model is not present, otherwise
     *        assert !
     */
    void checkNotNeedInputResultIsNotPresent() const;

    template<class Parent, class Child>
    static constexpr void checkStructureValidity(std::true_type) {
        static_assert((Parent::MinValue > 0)
                      || ((Parent::MinValue == 0) && (Child::MinValue == 0)), "Structure not allowed ! Parent is optionnal (min == 0) but child is set to be obligatory (min > 0).");
    }

    template<class Parent, class Child>
    static constexpr void checkStructureValidity(std::false_type) {
        // parent is abstract so we can not check at compilation time
    }

    template<class Parent, class Child>
    static void checkStructureValidityAtRunTime(const Parent& parent, const Child& child) {
        #define QT_FORCE_ASSERTS
        Q_ASSERT_X((parent.minimum() > 0)
                      || ((parent.minimum() == 0) && (child.minimum() == 0)), "checkStructureValidityAtRunTime", "Structure not allowed ! Parent is optionnal (min == 0) but child is set to be obligatory (min > 0).");
        #undef QT_FORCE_ASSERTS

        Q_UNUSED(parent);
        Q_UNUSED(child);
    }

    /*****************************************************/

    template<class HandleInResult, typename... ConstructorArgs>
    void internalAddResult(HandleInResult& handleResult,
                           std::true_type,
                           ConstructorArgs&& ...constructorArgs) {
        MODELS_ASSERT(!handleResult.isValid());

        auto resultModel = new HandleInResult::ModelType(std::forward<ConstructorArgs>(constructorArgs)...,
                                                         HandleInResult::MinValue,
                                                         HandleInResult::MaxValue);
        handleResult.setModel(resultModel);

        m_results.append(resultModel);
    }

    template<class HandleInResult, typename... ConstructorArgs>
    void internalAddResult(HandleInResult&,
                           std::false_type,
                           ConstructorArgs&& ...) {

        NO_INPUT_STATIC_ASSERT_MESSAGE;
    }

    /*****************************************************/

    template<class HandleInResult, class HandleZeroOrMore>
    void internalSetZeroOrMoreRootGroup(const HandleInResult& handleResult,
                                        HandleZeroOrMore& handleRootGroup,
                                        std::true_type) {
        MODELS_ASSERT((handleResult.model()->rootGroup() == nullptr) && !handleRootGroup.isValid());

        auto rootGroupModel = HandleZeroOrMore::ModelType::create<HandleZeroOrMore::GroupType>();

        handleResult.model()->setRootGroup(rootGroupModel);

        handleRootGroup.setModel(rootGroupModel);
    }

    template<class HandleInResult, class HandleZeroOrMore>
    void internalSetZeroOrMoreRootGroup(const HandleInResult& handleResult,
                                        HandleZeroOrMore& handleRootGroup,
                                        std::false_type) {

        NO_INPUT_STATIC_ASSERT_MESSAGE;
    }

    /*****************************************************/

    template<class HandleInResult, class HandleInGroup, typename... ConstructorArgs>
    void internalSetRootGroup(const HandleInResult& handleResult,
                              HandleInGroup& rootGroupHandle,
                              std::true_type,
                              ConstructorArgs&& ...constructorArgs) {
        MODELS_ASSERT((handleResult.model()->rootGroup() == nullptr) && !rootGroupHandle.isValid());

        auto rootGroupModel = HandleInGroup::ModelType::create<HandleInGroup::GroupType>(std::forward<ConstructorArgs>(constructorArgs)...,
                                                                                         HandleInGroup::MinValue,
                                                                                         HandleInGroup::MaxValue);

        handleResult.model()->setRootGroup(rootGroupModel);
        rootGroupHandle.setModel(rootGroupModel);
    }

    template<class HandleInResult, class HandleInGroup, typename... ConstructorArgs>
    void internalSetRootGroup(const HandleInResult&,
                              HandleInGroup&,
                              std::false_type,
                              ConstructorArgs&& ...) {

        NO_INPUT_STATIC_ASSERT_MESSAGE;
    }

    /*****************************************************/

    template<class HandleInGroupParent, class HandleInGroup, typename... ConstructorArgs>
    void internalAddGroup(const HandleInGroupParent& parentGroup,
                          HandleInGroup& groupHandle,
                          std::true_type,
                          ConstructorArgs&& ...constructorArgs) {

        checkStructureValidity<HandleInGroupParent, HandleInGroup>(std::integral_constant<bool, IsAnInputAbstractHandle<HandleInGroupParent>::value>());
        checkStructureValidityAtRunTime<HandleInGroupParent, HandleInGroup>(parentGroup, groupHandle);

        MODELS_ASSERT(parentGroup.isValid() && !groupHandle.isValid());

        auto groupModel = HandleInGroup::ModelType::create<HandleInGroup::GroupType>(std::forward<ConstructorArgs>(constructorArgs)...,
                                                                                     HandleInGroup::MinValue,
                                                                                     HandleInGroup::MaxValue);

        parentGroup.model()->addGroup(groupModel);
        groupHandle.setModel(groupModel);
    }

    template<class HandleInGroupParent, class HandleInGroup, typename... ConstructorArgs>
    void internalAddGroup(const HandleInGroupParent&,
                          HandleInGroup&,
                          std::false_type,
                          ConstructorArgs&& ...) {

        NO_INPUT_STATIC_ASSERT_MESSAGE;
    }

    /*****************************************************/

    template<class HandleInGroupParent, class HandleInItem, typename... ConstructorArgs>
    void internalAddItem(const HandleInGroupParent& parentGroup,
                         HandleInItem& itemHandle,
                         std::true_type,
                         ConstructorArgs&& ...constructorArgs) {

        checkStructureValidity<HandleInGroupParent, HandleInItem>(std::integral_constant<bool, IsAnInputAbstractHandle<HandleInGroupParent>::value>());
        checkStructureValidityAtRunTime<HandleInGroupParent, HandleInItem>(parentGroup, itemHandle);

        MODELS_ASSERT(parentGroup.isValid() && !itemHandle.isValid());

        auto itemModel = HandleInItem::ModelType::create<HandleInItem::ItemType>(std::forward<ConstructorArgs>(constructorArgs)...,
                                                                                 HandleInItem::MinValue,
                                                                                 HandleInItem::MaxValue);

        parentGroup.model()->addItem(itemModel);
        itemHandle.setModel(itemModel);
    }

    template<class HandleInGroupParent, class HandleInItem, typename... ConstructorArgs>
    void internalAddItem(const HandleInGroupParent&,
                         HandleInItem&,
                         std::false_type,
                         ConstructorArgs&& ...) {

        NO_INPUT_STATIC_ASSERT_MESSAGE;
    }

    /*****************************************************/

    template<class HandleInItemParent, class HandleInItemAttribute, typename... ConstructorArgs>
    void internalAddItemAttribute(const HandleInItemParent& parentItem,
                                  HandleInItemAttribute& itemAttributeHandle,
                                  std::true_type,
                                  ConstructorArgs&& ...constructorArgs) {

        checkStructureValidity<HandleInItemParent, HandleInItemAttribute>(std::integral_constant<bool, IsAnInputAbstractHandle<HandleInItemParent>::value>());
        checkStructureValidityAtRunTime<HandleInItemParent, HandleInItemAttribute>(parentItem, itemAttributeHandle);

        MODELS_ASSERT(parentItem.isValid() && !itemAttributeHandle.isValid());

        auto itemAttributeModel = HandleInItemAttribute::ModelType::create(HandleInItemAttribute::ValueType,
                                                                           std::forward<ConstructorArgs>(constructorArgs)...,
                                                                           HandleInItemAttribute::MinValue,
                                                                           HandleInItemAttribute::MaxValue);

        parentItem.model()->addItemAttribute(itemAttributeModel);
        itemAttributeHandle.setModel(itemAttributeModel);
    }

    template<class HandleInItemParent, class HandleInItemAttribute, typename... ConstructorArgs>
    void internalAddItemAttribute(const HandleInItemParent&,
                                  HandleInItemAttribute&,
                                  std::false_type,
                                  ConstructorArgs&& ...) {

        NO_INPUT_STATIC_ASSERT_MESSAGE;
    }
};

#endif // CT_INMODELSTRUCTUREMANAGER_H
