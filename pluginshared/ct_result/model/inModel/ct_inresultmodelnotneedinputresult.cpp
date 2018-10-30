#include "ct_inresultmodelnotneedinputresult.h"

#include "interfaces.h"

CT_InResultModelNotNeedInputResult::CT_InResultModelNotNeedInputResult() : CT_InAbstractResultModel("CT_InResultModelNotNeedInputResult",
                                                                                                    tr("Not input result"),
                                                                                                    tr("No input result"),
                                                                                                    false)
{
    setMinimumNumberOfPossibilityThatMustBeSelectedForOneTurn(0);
}

void CT_InResultModelNotNeedInputResult::saveSettings(SettingsWriterInterface &writer) const
{
    SuperClass::saveSettings(writer);

    writer.addParameter(this, "Type", staticMetaObject.className());
}

bool CT_InResultModelNotNeedInputResult::restoreSettings(SettingsReaderInterface &reader)
{
    if(!SuperClass::restoreSettings(reader))
        return false;

    QVariant value;

    if(!reader.parameter(this, "Type", value) || (value.toString() != staticMetaObject.className()))
        return false;

    return true;
}

CT_InAbstractModel* CT_InResultModelNotNeedInputResult::copy(bool withPossibilities) const
{
    Q_UNUSED(withPossibilities)

    CT_InResultModelNotNeedInputResult *cpy = new CT_InResultModelNotNeedInputResult();
    cpy->setStep(step());
    cpy->setOriginalModel(this);

    return cpy;
}
