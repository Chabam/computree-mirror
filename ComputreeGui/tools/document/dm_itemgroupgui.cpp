#include "dm_itemgroupgui.h"

#include "ct_global/ct_context.h"
#include "ct_pointcloudindex/ct_pointcloudindexlessmemory.h"
#include "ct_cloudindex/registered/ct_standardnotmodifiablecloudindexregisteredt.h"

CT_TYPE_IMPL_INIT_MACRO(DM_ItemGroupGui)

DM_ItemGroupGui::DM_ItemGroupGui() : SuperClass(),
    mCacheList(nullptr),
    mGraphicsView(nullptr),
    mPointsColorModel(nullptr),
    mPointsNormalModel(nullptr),
    mEdgesColorModel(nullptr),
    mEdgesNormalModel(nullptr),
    mFacesColorModel(nullptr),
    mFacesNormalModel(nullptr),
    mSelectedPointsModel(nullptr),
    mPointAttributesColor(nullptr),
    mPointAttributesNormal(nullptr),
    mEdgeAttributesColor(nullptr),
    mEdgeAttributesNormal(nullptr),
    mFaceAttributesColor(nullptr),
    mFaceAttributesNormal(nullptr),
    mSelectedPointsScene(nullptr)
{
}

DM_ItemGroupGui::DM_ItemGroupGui(const DM_ItemGroupGui& other) : SuperClass(other)
{
    mCacheList = nullptr;
    mGraphicsView = nullptr;

    mPointsColorModel = nullptr;
    mPointsNormalModel = nullptr;
    mEdgesColorModel = nullptr;
    mEdgesNormalModel = nullptr;
    mFacesColorModel = nullptr;
    mFacesNormalModel = nullptr;

    mSelectedPointsModel = nullptr;

    mPointAttributesColor = nullptr;
    mPointAttributesNormal = nullptr;
    mEdgeAttributesColor = nullptr;
    mEdgeAttributesNormal = nullptr;
    mFaceAttributesColor = nullptr;
    mFaceAttributesNormal = nullptr;

    mSelectedPointsScene = nullptr;
}

DM_ItemGroupGui::~DM_ItemGroupGui()
{
    qDeleteAll(mListForType.begin(), mListForType.end());
    qDeleteAll(mAttributesListForType.begin(), mAttributesListForType.end());

    delete mPointAttributesColor;
    delete mPointAttributesNormal;
    delete mEdgeAttributesColor;
    delete mEdgeAttributesNormal;
    delete mFaceAttributesColor;
    delete mFaceAttributesNormal;

    delete mSelectedPointsScene;
}

void DM_ItemGroupGui::addItemDrawable(CT_AbstractItemDrawable& item)
{
    const QString itemType = item.type();
    QList<CT_AbstractItemDrawable*>* list = nullptr;

    if(itemType == mCacheType)
    {
        list = mCacheList;
    }
    else
    {
        list = mListForType.value(itemType, nullptr);

        if(list == nullptr)
        {
            auto itemModelOriginal = dynamic_cast<DEF_CT_AbstractItemDrawableModelOut*>(item.model());

            if(itemModelOriginal != nullptr)
            {
                auto itemModel = static_cast<DEF_CT_AbstractItemDrawableModelOut*>(itemModelOriginal->copy());

                ModelType* myModel = modelStaticT<ModelType>();

                myModel->addItem(itemModel);

                list = new QList<CT_AbstractItemDrawable*>();

                mListForType.insert(itemType, list);

                mCacheList = list;
                mCacheType = itemType;
            }
        }
        else
        {
            mCacheList = list;
            mCacheType = itemType;
        }
    }

    if(list != nullptr)
        list->append(&item);
}

void DM_ItemGroupGui::removeItemDrawable(CT_AbstractItemDrawable& item)
{
    const QString itemType = item.type();

    QList<CT_AbstractItemDrawable*>* list = nullptr;

    if(itemType == mCacheType)
    {
        list = mCacheList;
    }
    else
    {
        list = mListForType.value(itemType, nullptr);

        if(list == nullptr)
            return;

        mCacheList = list;
        mCacheType = itemType;
    }

    list->removeOne(&item);

    if(list->isEmpty())
    {
        mListForType.remove(itemType);
        delete list;

        ModelType* myModel = modelStaticT<ModelType>();
        CT_OutAbstractSingularItemModel* itemModel = nullptr;

        if(!myModel->visitItems([&itemType, &itemModel](const CT_OutAbstractSingularItemModel* model) -> bool {
            if(itemType == model->itemDrawableStaticCastT<>()->type()) {
                itemModel = const_cast<CT_OutAbstractSingularItemModel*>(model);
                return false;
            }

            return true;
        }))
        {
            myModel->removeItem(itemModel);
        }

        if(list == mCacheList)
        {
            mCacheList = nullptr;
            mCacheType.clear();
        }
    }
}

void DM_ItemGroupGui::setAttributes(const QList<CT_AbstractAttributes*>& attributes)
{
    mAttributesListForType.clear();
    mCacheType.clear();
    mCacheList = nullptr;

    ModelType* myModel = modelStaticT<ModelType>();

    for(DEF_CT_AbstractItemDrawableModelOut* attributeModel : mAttributesModels)
    {
        myModel->removeItem(attributeModel);
    }

    mAttributesModels.clear();

    for(CT_AbstractAttributes* attribute : attributes)
    {
        CT_AbstractAttributes& item = *attribute;
        const QString itemType = item.type();
        QList<CT_AbstractItemDrawable*>* list = nullptr;

        if(itemType == mCacheType)
        {
            list = mCacheList;
        }
        else
        {
            list = mAttributesListForType.value(itemType, nullptr);

            if(list == nullptr)
            {
                auto itemModel = dynamic_cast<DEF_CT_AbstractItemDrawableModelOut*>(item.model()->copy());

                myModel->addItem(itemModel);
                mAttributesModels.append(itemModel);

                list = new QList<CT_AbstractItemDrawable*>();

                mAttributesListForType.insert(itemType, list);
            }

            mCacheList = list;
            mCacheType = itemType;
        }

        list->append(&item);
    }

    mCacheType.clear();
    mCacheList = nullptr;
}

void DM_ItemGroupGui::setGraphicsView(const GraphicsViewInterface* gv)
{
    // TODO : somewhere add a model for selected points, selected edges and selected faces

    if((mPointsColorModel == nullptr) && (gv != nullptr))
    {
        mPointsColorModel = new PointsColorModel(tr("Couleurs des points (GUI)"));
        mPointsNormalModel = new PointsNormalModel(tr("Normales des points (GUI)"));
        mEdgesColorModel = new EdgesColorModel(tr("Couleurs des arêtes (GUI)"));
        mEdgesNormalModel = new EdgesNormalModel(tr("Normales des arêtes (GUI)"));
        mFacesColorModel = new FacesColorModel(tr("Couleurs des faces (GUI)"));
        mFacesNormalModel = new FacesNormalModel(tr("Normales des faces (GUI)"));

        mSelectedPointsModel = new SelectedPointsModel(tr("Points sélectionnés (GUI)"));

        ModelType* myModel = modelStaticT<ModelType>();
        myModel->addItem(mPointsColorModel);
        myModel->addItem(mPointsNormalModel);
        myModel->addItem(mEdgesColorModel);
        myModel->addItem(mEdgesNormalModel);
        myModel->addItem(mFacesColorModel);
        myModel->addItem(mFacesNormalModel);
        myModel->addItem(mSelectedPointsModel);
    }
    else if((mPointsColorModel != nullptr) && (gv == nullptr))
    {
        ModelType* myModel = modelStaticT<ModelType>();
        myModel->removeItem(mPointsColorModel);
        myModel->removeItem(mPointsNormalModel);
        myModel->removeItem(mEdgesColorModel);
        myModel->removeItem(mEdgesNormalModel);
        myModel->removeItem(mFacesColorModel);
        myModel->removeItem(mFacesNormalModel);
        myModel->removeItem(mSelectedPointsModel);

        mPointsColorModel = nullptr;
        mPointsNormalModel = nullptr;
        mEdgesColorModel = nullptr;
        mEdgesNormalModel = nullptr;
        mFacesColorModel = nullptr;
        mFacesNormalModel = nullptr;
        mSelectedPointsModel = nullptr;
    }

    mGraphicsView = gv;
}

void DM_ItemGroupGui::finalizeBeforeUseIt()
{
    const size_t pSize = PS_REPOSITORY->globalPointCloud()->size();

    if(mPointsCloudIndex.isNull() || (mPointsCloudIndex->size() != pSize))
        mPointsCloudIndex = CT_NMPCIR(new CT_StandardNotModifiableCloudIndexRegisteredT<CT_PointData>(new CT_PointCloudIndexLessMemory(0, pSize)));

    const size_t eSize = PS_REPOSITORY->globalEdgeCloud()->size();

    if(mEdgesCloudIndex.isNull() || (mEdgesCloudIndex->size() != eSize))
        mEdgesCloudIndex = CT_NMECIR(new CT_StandardNotModifiableCloudIndexRegisteredT<CT_Edge>(new CT_EdgeCloudIndexLessMemory(0, eSize)));

    const size_t fSize = PS_REPOSITORY->globalFaceCloud()->size();

    if(mFacesCloudIndex.isNull() || (mFacesCloudIndex->size() != fSize))
        mFacesCloudIndex = CT_NMFCIR(new CT_StandardNotModifiableCloudIndexRegisteredT<CT_Face>(new CT_FaceCloudIndexLessMemory(0, fSize)));

    delete mPointAttributesColor;
    delete mPointAttributesNormal;
    delete mEdgeAttributesColor;
    delete mEdgeAttributesNormal;
    delete mFaceAttributesColor;
    delete mFaceAttributesNormal;

    delete mSelectedPointsScene;

    mPointAttributesColor = nullptr;
    mPointAttributesNormal = nullptr;
    mEdgeAttributesColor = nullptr;
    mEdgeAttributesNormal = nullptr;
    mFaceAttributesColor = nullptr;
    mFaceAttributesNormal = nullptr;

    mSelectedPointsScene = nullptr;
}

CT_AbstractItem::IChildrensIteratorQtStylePtr DM_ItemGroupGui::createQtStyleIteratorForChildrensThatUseOutModel(const CT_OutAbstractModel* outModel) const
{
    if(outModel == mPointsColorModel)
        return createIteratorForColorAttribute(const_cast<DM_ItemGroupGui*>(this)->mPointAttributesColor, mPointsCloudIndex, const_cast<DM_ItemGroupGui*>(this)->mPointsColorAttributeManager, GraphicsViewInterface::CPointCloud);

    if(outModel == mPointsNormalModel)
        return createIteratorForNormalAttribute(const_cast<DM_ItemGroupGui*>(this)->mPointAttributesNormal, mPointsCloudIndex, const_cast<DM_ItemGroupGui*>(this)->mPointsNormalAttributeManager, GraphicsViewInterface::NPointCloud);

    if(outModel == mEdgesColorModel)
        return createIteratorForColorAttribute(const_cast<DM_ItemGroupGui*>(this)->mEdgeAttributesColor, mEdgesCloudIndex, const_cast<DM_ItemGroupGui*>(this)->mEdgesColorAttributeManager, GraphicsViewInterface::CEdgeCloud);

    if(outModel == mEdgesNormalModel)
        return createIteratorForNormalAttribute(const_cast<DM_ItemGroupGui*>(this)->mEdgeAttributesNormal, mEdgesCloudIndex, const_cast<DM_ItemGroupGui*>(this)->mEdgesNormalAttributeManager, GraphicsViewInterface::NEdgeCloud);

    if(outModel == mFacesColorModel)
        return createIteratorForColorAttribute(const_cast<DM_ItemGroupGui*>(this)->mFaceAttributesColor, mFacesCloudIndex, const_cast<DM_ItemGroupGui*>(this)->mFacesColorAttributeManager, GraphicsViewInterface::CFaceCloud);

    if(outModel == mFacesNormalModel)
        return createIteratorForNormalAttribute(const_cast<DM_ItemGroupGui*>(this)->mFaceAttributesNormal, mFacesCloudIndex, const_cast<DM_ItemGroupGui*>(this)->mFacesNormalAttributeManager, GraphicsViewInterface::NFaceCloud);

    if(outModel == mSelectedPointsModel)
    {
        if(mSelectedPointsScene == nullptr)
            const_cast<DM_ItemGroupGui*>(this)->mSelectedPointsScene = new CT_Scene(qSharedPointerDynamicCast<CT_AbstractCloudIndexRegisteredT<CT_PointData>>(mGraphicsView->getSelectedPoints()));

        return new ItemQtIterator(mSelectedPointsScene);
    }

    const CT_OutAbstractItemModel* itemModel = dynamic_cast<const CT_OutAbstractItemModel*>(outModel);

    if(itemModel == nullptr)
        return nullptr;

    const QString itemType = itemModel->itemDrawableStaticCastT<>()->type();

    if(!itemType.isEmpty())
    {
        if(dynamic_cast<CT_AbstractAttributes*>(itemModel->itemDrawable()) != nullptr)
        {
            QList<CT_AbstractItemDrawable*>* list = mAttributesListForType.value(itemType, nullptr);

            if(list != nullptr)
                return new ItemsQtIterator(list->begin(), list->end());
        }
        else
        {
            QList<CT_AbstractItemDrawable*>* list = mListForType.value(itemType, nullptr);

            if(list != nullptr)
                return new ItemsQtIterator(list->begin(), list->end());
        }
    }

    return nullptr;
}

CT_AbstractItemDrawable* DM_ItemGroupGui::copy(const CT_OutAbstractItemModel*, const CT_AbstractResult*) const
{
    return nullptr;
}
