#ifndef PB_STEPEXPORTATTRIBUTESASASCII_H
#define PB_STEPEXPORTATTRIBUTESASASCII_H

#include <QMap>

#include "ct_step/abstract/ct_abstractstep.h"
#include "ct_itemdrawable/ct_loopcounter.h"

class PB_StepExportAttributesAsASCII: public CT_AbstractStep
{
    Q_OBJECT
    using SuperClass = CT_AbstractStep;

public:

    PB_StepExportAttributesAsASCII();

    QString description() const final;
    QString detailledDescription() const final;
    QString inputDescription() const final;
    QString outputDescription() const final;
    QString detailsDescription() const final;

    CT_VirtualAbstractStep* createNewInstance() const final;

protected:

    void declareInputModels(CT_StepInModelStructureManager& manager) final;

    void fillPostInputConfigurationDialog(CT_StepConfigurableDialog* postInputConfigDialog) final;

    void declareOutputModels(CT_StepOutModelStructureManager& manager) final;

    void compute() final;

private:

    CT_HandleInResultGroup<>                                        mInResult;
    CT_HandleInStdZeroOrMoreGroup                                   mInRootGroup;
    CT_HandleInStdGroup<>                                           mInGroupMain;
    CT_HandleInSingularItem<CT_AbstractSingularItemDrawable, 1, -1> mInItemWithAttribute; // multiple
    CT_HandleInStdItemAttribute<CT_AbstractCategory::ANY, 1, -1>    mInItemAttribute; // multiple

    CT_HandleInResultGroup<0,1>                                     mInResultCounter;
    CT_HandleInStdGroup<>                                           mInGroupCounter;
    CT_HandleInSingularItem<CT_LoopCounter>                         mInLoopCounter;

    QList<QString>          _modelsKeys;
    QList<QString>          _modelsKeysWithoutXOrYAttribute;
    QMap<QString, QString>  _names;
    QMap<QString, QString>  _shortNames;
    QMap<QString, QString>  _itemNames;

    bool                    _inLoop;

    // Step parameters
    bool        _replaceSpecialCharacters;
    bool        _shortenNames;
    QStringList _outASCIIFileName;

    QString createExportBaseName(bool& first, bool &inLoop) const;

    template<typename InHandleT>
    void computeModelsKeysAndNames(InHandleT& handle)
    {
        auto it = handle.template iterateSelectedOutputModels<CT_OutAbstractItemAttributeModel>(mInResult);
        auto begin = it.begin();
        auto end = it.end();

        while(begin != end)
        {
            if(isStopped())
                return;

            const CT_OutAbstractItemAttributeModel* attModel = (*begin);
            const CT_OutAbstractModel* itemModel = static_cast<const CT_OutAbstractModel*>(attModel->parentModel());

            computeModelsKeysAndNamesForModels(itemModel, attModel);

            ++begin;
        }
    }

    void computeModelsKeysAndNamesForModels(const CT_OutAbstractModel* itemModel, const CT_OutAbstractItemAttributeModel* attModel);
    QString computeKeyForModels(const CT_OutAbstractModel* itemModel, const CT_OutAbstractModel* attModel) const;

    bool exportInAscii(QScopedPointer<QFile>& fileASCII, QScopedPointer<QTextStream>& streamASCII, const bool firstTurnFromCounter);

    void addToIndexedAttributesCollection(const CT_AbstractSingularItemDrawable* item, const CT_AbstractItemAttribute* attribute, QMap<QString, QPair<const CT_AbstractSingularItemDrawable*, const CT_AbstractItemAttribute*> >& indexedAttributes) const;

    void replaceSpecialCharacters(QMap<QString, QString> &names) const;

};

#endif // PB_STEPEXPORTATTRIBUTESASASCII_H
