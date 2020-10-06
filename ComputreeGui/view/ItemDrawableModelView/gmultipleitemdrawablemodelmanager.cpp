#include "gmultipleitemdrawablemodelmanager.h"
#include "ui_gmultipleitemdrawablemodelmanager.h"

#include "gitemdrawablemanageroptionscolor.h"

#include <QLineEdit>

#include "ct_step/abstract/ct_virtualabstractstep.h"
#include "ct_result/abstract/ct_abstractresult.h"

GMultipleItemDrawableModelManager::GMultipleItemDrawableModelManager(QWidget *parent) :
    QWidget(parent), DM_MultipleItemDrawableModelManager(),
    ui(new Ui::GMultipleItemDrawableModelManager)
{
    ui->setupUi(this);

    _options.load();

    ui->widgetModelManager->setColorOptions(_options);

    connect(ui->gradientManagerView, SIGNAL(newGradientSelected(QLinearGradient)), ui->widgetModelManager->contextMenuColouristAdder(), SLOT(setLinearGradientToUseForColorization(QLinearGradient)));
    ui->widgetModelManager->contextMenuColouristAdder()->setGradientToUseForColorization(ui->gradientManagerView->gradientSelected());

    setEmptyComboBoxText();
}

GMultipleItemDrawableModelManager::~GMultipleItemDrawableModelManager()
{
    delete ui;
}

void GMultipleItemDrawableModelManager::setDocumentManagerView(const GDocumentManagerView *docManagerView)
{
    ui->widgetModelManager->setDocumentManagerView(docManagerView);
}

bool GMultipleItemDrawableModelManager::containsResult(const CT_AbstractResult* res) const
{
    return (indexOfResultInComboBox(res) != -1);
}

void GMultipleItemDrawableModelManager::addResult(const CT_AbstractResult *res)
{
    int indexOf = indexOfResultInComboBox(res);

    if(indexOf == -1)
    {
        if(res->model() == nullptr)
            return;

        if(ui->comboBoxResult->itemData(0).value<void*>() == nullptr)
        {
            ui->comboBoxResult->clear();
            ui->comboBoxResult->setEditable(false);
        }

        connect(res->model(), SIGNAL(destroyed(QObject*)), this, SLOT(resultModelDestroyedDirect(QObject*)), Qt::DirectConnection);
        connect(this, SIGNAL(internalRemoveResult(const CT_AbstractResult*)), this, SLOT(removeResult(const CT_AbstractResult*)), Qt::QueuedConnection);
        connect((CT_AbstractResult*)res, SIGNAL(destroyed(QObject*)), this, SLOT(resultDestroyedDirect(QObject*)), Qt::DirectConnection);
        connect((CT_AbstractResult*)res, SIGNAL(destroyed(QObject*)), this, SLOT(resultDestroyedQueued(QObject*)), Qt::QueuedConnection);

        _results.append((CT_AbstractResult*)res);
        ui->comboBoxResult->addItem(res->displayableName() + QString(" (%1)").arg(static_cast<CT_VirtualAbstractStep*>(res->parentStep())->uniqueID()), QVariant::fromValue((void*)res));
        ui->comboBoxResult->setCurrentIndex(ui->comboBoxResult->count()-1);
    }
    else
    {
        ui->comboBoxResult->setCurrentIndex(indexOf);
    }
}

bool GMultipleItemDrawableModelManager::removeResult(const CT_AbstractResult *res)
{
    int index = indexOfResultInComboBox(res);

    if(index != -1)
    {
        if(res->model() != nullptr)
            disconnect(res->model(), nullptr, this, nullptr);

        disconnect((CT_AbstractResult*)res, nullptr, this, nullptr);

        _results.removeOne((CT_AbstractResult*)res);
        ui->comboBoxResult->removeItem(index);
    }

    if(ui->comboBoxResult->count() == 0)
        setEmptyComboBoxText();

    return (index != -1);
}

CT_AbstractResult* GMultipleItemDrawableModelManager::currentResult() const
{
    return ui->widgetModelManager->result();
}

void GMultipleItemDrawableModelManager::reconstruct()
{
    ui->widgetModelManager->reconstruct();
}

int GMultipleItemDrawableModelManager::indexOfResultInComboBox(const CT_AbstractResult *res) const
{
    int count = ui->comboBoxResult->count();

    for(int i=0; i<count; ++i)
    {
        if(ui->comboBoxResult->itemData(i).value<void*>() == res)
            return i;
    }

    return -1;
}

void GMultipleItemDrawableModelManager::setEmptyComboBoxText()
{
    ui->comboBoxResult->clear();
    ui->comboBoxResult->addItem(tr("Aucun élément"), QVariant::fromValue(0));
    ui->comboBoxResult->setEditable(true);
    ui->comboBoxResult->lineEdit()->setAlignment(Qt::AlignCenter);
    ui->comboBoxResult->lineEdit()->setReadOnly(true);
}

void GMultipleItemDrawableModelManager::on_comboBoxResult_currentIndexChanged(int index)
{
    ui->widgetModelManager->setResult(nullptr);
    int indexOf = _results.indexOf((CT_AbstractResult*)ui->comboBoxResult->itemData(index).value<void*>());

    if((index >= 0)
            && (index < ui->comboBoxResult->count())
            && (indexOf != -1))
        ui->widgetModelManager->setResult((CT_AbstractResult*)_results.at(indexOf));
}

void GMultipleItemDrawableModelManager::on_pushButtonChooseColor_clicked()
{
    GItemDrawableManagerOptionsColor dialog(this);

    dialog.setOptions(_options);

    if(dialog.exec())
    {
        _options.setColorList(dialog.getColorList());

        ui->widgetModelManager->setColorOptions(_options);
    }
}

void GMultipleItemDrawableModelManager::resultModelDestroyedDirect(QObject* o)
{
    CT_AbstractResult* r = static_cast<CT_OutAbstractResultModel*>(o)->resultStaticCastT<CT_AbstractResult>();

    _results.removeOne(r);

    emit internalRemoveResult(r);
}

void GMultipleItemDrawableModelManager::resultDestroyedDirect(QObject *o)
{
    _results.removeOne((CT_AbstractResult*)o);
}

void GMultipleItemDrawableModelManager::resultDestroyedQueued(QObject *o)
{
    int index = indexOfResultInComboBox((CT_AbstractResult*)o);

    if(index != -1)
        ui->comboBoxResult->removeItem(index);

    if(ui->comboBoxResult->count() == 0)
        setEmptyComboBoxText();
}
