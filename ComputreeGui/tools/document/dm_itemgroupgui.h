#ifndef DM_ITEMGROUPGUI_H
#define DM_ITEMGROUPGUI_H

#include "graphicsviewinterface.h"

#include "ct_itemdrawable/ct_standarditemgroup.h"
#include "ct_itemdrawable/abstract/ct_abstractattributes.h"
#include "ct_itemdrawable/ct_pointsattributescolor.h"
#include "ct_itemdrawable/ct_pointsattributesnormal.h"
#include "ct_itemdrawable/ct_edgeattributescolor.h"
#include "ct_itemdrawable/ct_edgeattributesnormal.h"
#include "ct_itemdrawable/ct_faceattributescolor.h"
#include "ct_itemdrawable/ct_faceattributesnormal.h"

#include "ct_colorcloud/registered/ct_standardcolorcloudregistered.h"
#include "ct_normalcloud/registered/ct_standardnormalcloudregistered.h"

#include "ct_model/outModel/ct_outstdgroupmodel.h"
#include "ct_model/outModel/ct_outstdsingularitemmodel.h"

/**
 * @brief
 * When you add an item it will check if
 *        the type already exist in it and if not it will use a copy of the model of the item
 *        and att it to the model of the root group.
 */
class DM_ItemGroupGui : public CT_StandardItemGroup
{
    Q_OBJECT
    using SuperClass = CT_StandardItemGroup;
    CT_TYPE_IMPL_MACRO(DM_ItemGroupGui, CT_StandardItemGroup, Group GUI)

public:
    using ModelType = CT_OutStdGroupModel<DM_ItemGroupGui>;

    DM_ItemGroupGui();
    DM_ItemGroupGui(const DM_ItemGroupGui& other);
    ~DM_ItemGroupGui() override;

    void addItemDrawable(CT_AbstractItemDrawable &item);

    void removeItemDrawable(CT_AbstractItemDrawable &item);

    void setAttributes(const QList<CT_AbstractAttributes*>& attributes);

    void setGraphicsView(const GraphicsViewInterface* gv);

    void finalizeBeforeUseIt();

    IChildrensIteratorQtStylePtr createQtStyleIteratorForChildrensThatUseOutModel(const CT_OutAbstractModel* outModel) const override;

    CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel* model, const CT_AbstractResult* result) const override;

private:
    using ItemsQtIterator = CollectionQtIterator<QList<CT_AbstractItemDrawable*>>;
    using PointsColorModel = CT_OutStdSingularItemModel<CT_PointsAttributesColor>;
    using PointsNormalModel = CT_OutStdSingularItemModel<CT_PointsAttributesNormal>;
    using EdgesColorModel = CT_OutStdSingularItemModel<CT_EdgeAttributesColor>;
    using EdgesNormalModel = CT_OutStdSingularItemModel<CT_EdgeAttributesNormal>;
    using FacesColorModel = CT_OutStdSingularItemModel<CT_FaceAttributesColor>;
    using FacesNormalModel = CT_OutStdSingularItemModel<CT_FaceAttributesNormal>;

    QHash<QString, QList<CT_AbstractItemDrawable*>*>    mListForType;
    QString                                             mCacheType;
    QList<CT_AbstractItemDrawable*>*                    mCacheList;

    QHash<QString, QList<CT_AbstractItemDrawable*>*>    mAttributesListForType;
    QList<DEF_CT_AbstractItemDrawableModelOut*>         mAttributesModels;

    const GraphicsViewInterface*                        mGraphicsView;

    PointsColorModel*                                   mPointsColorModel;
    PointsNormalModel*                                  mPointsNormalModel;

    EdgesColorModel*                                    mEdgesColorModel;
    EdgesNormalModel*                                   mEdgesNormalModel;

    FacesColorModel*                                    mFacesColorModel;
    FacesNormalModel*                                   mFacesNormalModel;

    CT_NMPCIR                                           mPointsCloudIndex;
    CT_PointsAttributesColor*                           mPointAttributesColor;
    CT_PointsAttributesNormal*                          mPointAttributesNormal;

    CT_NMECIR                                           mEdgesCloudIndex;
    CT_EdgeAttributesColor*                             mEdgeAttributesColor;
    CT_EdgeAttributesNormal*                            mEdgeAttributesNormal;

    CT_NMFCIR                                           mFacesCloudIndex;
    CT_FaceAttributesColor*                             mFaceAttributesColor;
    CT_FaceAttributesNormal*                            mFaceAttributesNormal;

    template<typename AttributeType, typename CloudIndexType>
    IChildrensIteratorQtStylePtr createIteratorForColorAttribute(AttributeType*& attribute, CloudIndexType cloudIndex, GraphicsViewInterface::ColorCloudType cloudType) const {
        if(attribute == nullptr)
        {
            attribute = new AttributeType(cloudIndex,
                                          mGraphicsView->colorCloudOf(cloudType)->abstractColorCloud(),
                                          false);
        }

        return new ItemQtIterator(attribute);
    }

    template<typename AttributeType, typename CloudIndexType>
    IChildrensIteratorQtStylePtr createIteratorForNormalAttribute(AttributeType*& attribute, CloudIndexType cloudIndex, GraphicsViewInterface::NormalCloudType cloudType) const {
        if(attribute == nullptr)
        {
            attribute = new AttributeType(cloudIndex,
                                          mGraphicsView->normalCloudOf(cloudType)->abstractNormalCloud(),
                                          false);
        }

        return new ItemQtIterator(attribute);
    }
};

#endif // DM_ITEMGROUPGUI_H
