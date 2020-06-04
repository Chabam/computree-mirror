#ifndef GPOINTSATTRIBUTESMANAGER_H
#define GPOINTSATTRIBUTESMANAGER_H

#include <QDialog>
#include <QStandardItemModel>
#include <QThread>

#include "tools/attributes/worker/dm_attributesmanager.h"
#include "tools/attributes/worker/abstract/dm_abstractattributescolor.h"
#include "tools/attributes/worker/abstract/dm_abstractattributesnormal.h"
#include "tools/attributes/worker/abstract/dm_abstractattributesscalar.h"

#include "view/Tools/gradient/gradientarrow.h"

#include "ct_itemdrawable/abstract/ct_abstractpointsattributes.h"
#include "ct_itemdrawable/abstract/ct_abstractedgeattributes.h"
#include "ct_itemdrawable/abstract/ct_abstractfaceattributes.h"

namespace Ui {
class GPointsAttributesManager;
}

class GPointsAttributesManager : public QDialog
{
    Q_OBJECT

public:
    explicit GPointsAttributesManager(QWidget *parent = 0);
    ~GPointsAttributesManager();

    void setManager(const DM_AttributesManager *man);
    void setDocument(const GDocumentViewForGraphics *doc);

protected:
    void closeEvent(QCloseEvent *e);

private:
    Ui::GPointsAttributesManager    *ui;
    DM_AttributesManager            *m_manager;
    GDocumentViewForGraphics        *m_doc;
    QStandardItemModel              m_model;

    QStandardItem                   *m_itemPointRoot;
    QStandardItem                   *m_itemFaceRoot;
    QStandardItem                   *m_itemEdgeRoot;

    QStandardItem                   *m_itemPointRootColor;
    QStandardItem                   *m_itemPointRootScalar;
    QStandardItem                   *m_itemPointRootNormal;
    QStandardItem                   *m_itemFaceRootColor;
    QStandardItem                   *m_itemFaceRootScalar;
    QStandardItem                   *m_itemFaceRootNormal;
    QStandardItem                   *m_itemEdgeRootColor;
    QStandardItem                   *m_itemEdgeRootScalar;
    QStandardItem                   *m_itemEdgeRootNormal;

    bool                            m_internalSetColor;

    void initTreeView();

    template<typename IAttributesType, typename AttributesScalarType, typename AttributesColorType, typename AttributesNormalType>
    void buildTreeViewTForStep(CT_VirtualAbstractStep *step);

    void constructHeader();

    void checkAndSave(DM_AbstractAttributesScalar *pas);

    QList<QStandardItem*> createAttributesScalarForModel(DM_AbstractAttributesScalar *pa);
    QList<QStandardItem*> createAttributesColorForModel(DM_AbstractAttributesColor *pa);
    QList<QStandardItem*> createAttributesNormalForModel(DM_AbstractAttributesNormal *pa);

    template<typename IAttributesType>
    QStandardItem* rootItemForType() const { return nullptr; }

    template<typename IAttributesType>
    QStandardItem* getOrCreateScalarRootItemForType(QStandardItem*) { return nullptr; }

    template<typename IAttributesType>
    QStandardItem* getOrCreateColorRootItemForType(QStandardItem*) { return nullptr; }

    template<typename IAttributesType>
    QStandardItem* getOrCreateNormalRootItemForType(QStandardItem*) { return nullptr; }

    void createScalarRootItemIfNull(QStandardItem*& root, QStandardItem* parent) const;
    void createColorRootItemIfNull(QStandardItem*& root, QStandardItem* parent) const;
    void createNormalRootItemIfNull(QStandardItem*& root, QStandardItem* parent) const;

    QStandardItem* findOrCreateChildForAttributeModel(const CT_OutAbstractModel* attModel, std::function<QVariant()> data, QStandardItem* parent) const;

    void createWidgetForItems(const QList<QStandardItem*> &items) const;

    void editAttributesScalar(DM_AbstractAttributesScalar *pas);
    void clearEditGradient();
    void saveCurrentGradientTo(DM_AbstractAttributesScalar *pas);

    void editAttributesNormal(DM_AbstractAttributesNormal *pan);
    void clearEditNormal();
    void applyAndSaveNormal();

    DM_AbstractAttributes *attributesSelected() const;

private slots:
    void on_pushButtonAddColor_clicked();
    void on_pushButtonDeleteColor_clicked();
    void on_pushButtonSave_clicked();
    void on_pushButtonGradientColorPicker_colorChanged(const QColor &color);
    void on_colorGradientView_newFocusColor(const QColor &color, int arrowIndex);
    void on_colorGradientView_arrowMove(qreal lastPos, const GradientArrow &arrow);
    void on_checkBoxShowNormals_stateChanged(int state);

    void on_doubleSpinBoxNormalsLength_valueChanged(double v);
    void on_pushButtonNormalsColorPicker_colorChanged(QColor c);
    void on_pushButtonApplyEditNormals_clicked();

    void on_doubleSpinBoxGradientArrowValue_editingFinished();

    void pushButtonApplyClicked();
    void pushButtonConfigureClicked();
    void itemChanged(QStandardItem *item);
    void treeView_currentRowChanged(const QModelIndex & current, const QModelIndex & previous);
    void updateArrowValue(qreal val, GradientArrow arr);
};

template<>
QStandardItem* GPointsAttributesManager::rootItemForType<CT_AbstractPointsAttributes>() const;
template<>
QStandardItem* GPointsAttributesManager::rootItemForType<CT_AbstractFaceAttributes>() const;
template<>
QStandardItem* GPointsAttributesManager::rootItemForType<CT_AbstractEdgeAttributes>() const;

template<>
QStandardItem* GPointsAttributesManager::getOrCreateScalarRootItemForType<CT_AbstractPointsAttributes>(QStandardItem* parent);
template<>
QStandardItem* GPointsAttributesManager::getOrCreateScalarRootItemForType<CT_AbstractFaceAttributes>(QStandardItem* parent);
template<>
QStandardItem* GPointsAttributesManager::getOrCreateScalarRootItemForType<CT_AbstractEdgeAttributes>(QStandardItem* parent);

template<>
QStandardItem* GPointsAttributesManager::getOrCreateColorRootItemForType<CT_AbstractPointsAttributes>(QStandardItem* parent);
template<>
QStandardItem* GPointsAttributesManager::getOrCreateColorRootItemForType<CT_AbstractFaceAttributes>(QStandardItem* parent);
template<>
QStandardItem* GPointsAttributesManager::getOrCreateColorRootItemForType<CT_AbstractEdgeAttributes>(QStandardItem* parent);

template<>
QStandardItem* GPointsAttributesManager::getOrCreateNormalRootItemForType<CT_AbstractPointsAttributes>(QStandardItem* parent);
template<>
QStandardItem* GPointsAttributesManager::getOrCreateNormalRootItemForType<CT_AbstractFaceAttributes>(QStandardItem* parent);
template<>
QStandardItem* GPointsAttributesManager::getOrCreateNormalRootItemForType<CT_AbstractEdgeAttributes>(QStandardItem* parent);

#endif // GPOINTSATTRIBUTESMANAGER_H
