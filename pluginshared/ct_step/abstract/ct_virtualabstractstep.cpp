/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Michal KREBS (ARTS/ENSAM)

 This file is part of PluginShared library 2.0.

 PluginShared is free library: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 PluginShared is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with PluginShared.  If not, see <http://www.gnu.org/licenses/lgpl.html>.

*****************************************************************************/

#include "qglobal.h"

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtConcurrentMap>
#else
#include <QtConcurrent/QtConcurrentMap>
#endif

#include "ct_global/ct_context.h"

#include "ct_step/abstract/ct_virtualabstractstep.h"
#include "ct_step/ct_stepinitializedata.h"

#include "ct_result/abstract/ct_abstractresult.h"
#include "ct_itemdrawable/abstract/ct_abstractitemdrawable.h"
#include "ct_abstractstepplugin.h"

#include "ct_view/ct_stepconfigurabledialog.h"

#include "ct_result/ct_resultgroup.h"

#include "ct_result/tools/ct_inmanager.h"
#include "ct_result/tools/ct_outmanager.h"

#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/inModel/tools/ct_inresultmodelmanager.h"
#include "ct_result/model/inModel/tools/ct_inresultsbymodel.h"
#include "ct_result/model/outModel/ct_outresultmodelgroupcopy.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"

#include "ct_tools/model/abstract/ct_abstractoutmodelcopyaction.h"
#include "ct_result/model/inModel/ct_inresultmodelnotneedinputresult.h"
#include "ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibilities.h"

#include "ct_tools/model/ct_generateoutresultmodelname.h"

#include "ct_model/inModel/tools/ct_instdmodelpossibility.h"

#include "ct_model/tools/ct_modelsearchhelper.h"

#include "ct_view/tools/ct_configurablewidgettodialog.h"

#include <QMutex>
#include <QWaitCondition>
#include <QDialog>
#include <QMessageBox>

#include <assert.h>

int CT_VirtualAbstractStep::CURRENT_ID = 1;

CT_VirtualAbstractStep::CT_VirtualAbstractStep(CT_StepInitializeData &dataInit)
{
    if (dataInit.getMenuStepFlag())
    {
        _uniqueId = 0;
    } else {
        _uniqueId = CURRENT_ID;
        ++CURRENT_ID;
    }

    m_isAPrototype = false;

    m_guiContext = NULL;
    _preConfigDialog = NULL;
    _postConfigDialog = NULL;

    _data = &dataInit;

    _defaultOutModelCreating = false;

    _running = false;
    _stop = false;
    _paused = false;
    _error_code = 0;
    _elapsed = 0;
    _progress = 0;
    _progressClearMem = 0;
    _isSettingsModified = true;
    _debuggable = false;
    _debugMode = false;
    _ackOfDebugMode = false;
    m_manual = false;
    m_modifiable = false;
    m_firstCallToManualMode = true;
    m_mustQuitManualMode = true;
    _result_absolute_path_saved_dir = "";
    _result_saved_dir = "";
    _n_result_saved = 0;

    _jump_n_step = 1;
    _step_by_step_size = 0;
    _index_step_step_to_go = 0;

    _inManager = new CT_InManager();
    _outManager = new CT_OutManager(*this);

    connect(_outManager->getResultManager(), SIGNAL(resultAdded(const CT_AbstractResult*)), this, SIGNAL(resultAdded(const CT_AbstractResult*)), Qt::DirectConnection);
}

CT_VirtualAbstractStep::~CT_VirtualAbstractStep()
{
    qDeleteAll(_stepChildList.begin(), _stepChildList.end());

    clearOutResult();

    delete _inManager;
    delete _preConfigDialog;
    delete _postConfigDialog;
    delete _outManager;
    delete _data;
}

int CT_VirtualAbstractStep::uniqueID() const
{
    return _uniqueId;
}

void CT_VirtualAbstractStep::init()
{
}

bool CT_VirtualAbstractStep::mustCreateInputResultModels() const
{
    return _inManager->getResultModelManager()->isEmpty();
}

bool CT_VirtualAbstractStep::canCreateInputResultModels() const
{
    return _stepChildList.isEmpty() && mustCreateInputResultModels();
}

bool CT_VirtualAbstractStep::createInputResultModels()
{
    if(canCreateInputResultModels())
    {
        createInResultModelListProtected();

        if(parentStep() == NULL)
            return (isAPrototype() || !needInputResults()); // we return true if we are a prototype otherwise false because the GUI don't set the parent step

        // this method must always returns true and only returns false if the parent step is NULL(happen when it was the prototype) or
        // if the collection of input result model is not empty
        return _inManager->getResultModelManager()->findFromThisStepAllMatchingOutputModelsAndSavePossibilities(parentStep());
    }

    return false;
}

QList<CT_InAbstractResultModel*> CT_VirtualAbstractStep::getInResultsModel() const
{
    return _inManager->getResultModelManager()->models();
}

QList<CT_OutAbstractResultModel*> CT_VirtualAbstractStep::getOutResultsModel() const
{
    return _outManager->getResultModelManager()->models();
}

bool CT_VirtualAbstractStep::finalizeConfiguration()
{
    PS_MODELS->clearCache();

    // finalize configuration only if there is no child step after this step ! otherwise do nothing more
    if(!_stepChildList.isEmpty())
        return true;

    // now
    //     * settings of step has been restored
    //  or * showPostConfigurationDialog has been called
    // so we can finalize the configuration by creating all output models
    return _outManager->createOutputModels();
}

void CT_VirtualAbstractStep::savePreSettings(SettingsWriterInterface &writer) const
{
    if(_preConfigDialog != NULL)
        _preConfigDialog->saveSettings(writer);
}

void CT_VirtualAbstractStep::saveInputSettings(SettingsWriterInterface &writer) const
{
    _inManager->getResultModelConfigurationManager()->saveSettings(writer);
}

void CT_VirtualAbstractStep::savePostSettings(SettingsWriterInterface &writer) const
{
    if(_postConfigDialog != NULL)
        _postConfigDialog->saveSettings(writer);
}

void CT_VirtualAbstractStep::saveOthersSettings(SettingsWriterInterface &writer) const
{
    if(!_customName.isEmpty())
        writer.addParameter(this, "CustomName", _customName);
}

bool CT_VirtualAbstractStep::restorePreSettings(SettingsReaderInterface &reader)
{
    //cree la fenetre de pre-configuration
    createPreConfigurationDialog();

    // pre-configuration
    if(_preConfigDialog != NULL)
        _preConfigDialog->restoreSettings(reader);

    return true;
}

bool CT_VirtualAbstractStep::restoreInputSettings(SettingsReaderInterface &reader)
{
    // crée les résultats modèles d'entrée après avoir initialisé les paramètres des étapes
    // puisque certaines etapes modifie leur inResultModel après la pré-configuration !
    if(!createInputResultModels())
        return false;

    const bool ok = _inManager->getResultModelConfigurationManager()->restoreSettings(reader);

    if(ok)
        _inManager->getResultModelManager()->createSearchModelCollection();

    return ok;
}

bool CT_VirtualAbstractStep::restorePostSettings(SettingsReaderInterface &reader)
{
    //cree la fenetre de post-configuration
    createPostConfigurationDialog();

    // pre-configuration
    if(_postConfigDialog != NULL)
        return _postConfigDialog->restoreSettings(reader);

    return true;
}

bool CT_VirtualAbstractStep::restoreOthersSettings(SettingsReaderInterface &reader)
{
    QVariant customName;

    if(reader.parameter(this, "CustomName", customName))
        _customName = customName.toString();

    return true;
}

bool CT_VirtualAbstractStep::hasChoiceBetweenMultipleInputResults()
{
    return _inManager->getResultModelConfigurationManager()->checkIfMustCreateOrShowConfigurationDialog();
}

CT_AbstractStepPlugin* CT_VirtualAbstractStep::getPlugin() const
{
    return _data->getPlugin();
}

bool CT_VirtualAbstractStep::appendStep(CT_VirtualAbstractStep *step)
{
    return insertStep(_stepChildList.size(), step);
}

bool CT_VirtualAbstractStep::insertStep(int n, CT_VirtualAbstractStep *step)
{
    if(step->acceptAddAfterThisStep(this))
    {
        if(n >= 0)
        {
            step->setParentStep(this);
            _stepChildList.insert(n, step);
            step->setParentStep(this);
        }
        else
        {
            bool ok = true;

            QListIterator<CT_VirtualAbstractStep*> it(_stepChildList);

            while(ok
                  && it.hasNext())
                ok = it.next()->acceptAddAfterThisStep(step);

            if(!ok)
                return false;

            it.toFront();

            while(it.hasNext())
                step->appendStep(it.next());

            _stepChildList.clear();
            _stepChildList.append(step);
        }

        return true;
    }

    return false;
}

bool CT_VirtualAbstractStep::removeStep(CT_VirtualAbstractStep *step)
{
    if(_stepChildList.removeOne(step))
    {
        delete step;
        return true;
    }

    return false;
}

QList<CT_VirtualAbstractStep *> CT_VirtualAbstractStep::getStepChildList() const
{
    return _stepChildList;
}

CT_VirtualAbstractStep* CT_VirtualAbstractStep::parentStep() const
{
    return _data->getParentStep();
}

CT_VirtualAbstractStep* CT_VirtualAbstractStep::rootStep() const
{
    CT_VirtualAbstractStep *parent = const_cast<CT_VirtualAbstractStep*>(this);

    while(parent->parentStep() != NULL)
        parent = parent->parentStep();

    return parent;
}

bool CT_VirtualAbstractStep::acceptAddAfterThisStep(const CT_VirtualAbstractStep *step)
{
    if((step == NULL) && needInputResults())
        return false;

    return _inManager->getResultModelManager()->checkFromThisStepIfAllInputModelsFindMatchingOutputModels(step);
}

bool CT_VirtualAbstractStep::needInputResults() const
{
    return _inManager->getResultModelManager()->needInputResults();
}

bool CT_VirtualAbstractStep::isRunning() const
{
    return _running;
}

bool CT_VirtualAbstractStep::isStopped() const
{
    return _stop;
}

bool CT_VirtualAbstractStep::isPaused() const
{
    return _paused;
}

bool CT_VirtualAbstractStep::isInManualMode() const
{
    return !m_mustQuitManualMode;
}

bool CT_VirtualAbstractStep::mustBeRestarted() const
{
    return (isSettingsModified()
            || (nResult() == 0)
            || ((nResult() > 0)
                && (getResult(0)->isClearedFromMemory())));
}

bool CT_VirtualAbstractStep::mustRecheckTree() const
{
    return false;
}

void CT_VirtualAbstractStep::stop()
{
    _stop = true;

    emit stopped();
}

void CT_VirtualAbstractStep::quitManualMode()
{
    PS_LOG->addMessage(LogInterface::info, LogInterface::step, getStepCustomName() + tr(" quit manual mode"));

    if(getGuiContext() != NULL)
        useManualMode(true);

    m_mustQuitManualMode = true;
}

void CT_VirtualAbstractStep::unPause()
{
    _paused = false;
}

void CT_VirtualAbstractStep::aboutToBeDeleted()
{
    delete _preConfigDialog;
    _preConfigDialog = NULL;

    delete _postConfigDialog;
    _postConfigDialog = NULL;

    _inManager->aboutToBeDeleted();
}

void CT_VirtualAbstractStep::setDebugModeOn(bool enable)
{
    if(!isRunning())
        _debugMode = enable;
    else if(!enable)
        _index_step_step_to_go = -1;
}

bool CT_VirtualAbstractStep::isDebuggable() const
{
    return _debuggable;
}

bool CT_VirtualAbstractStep::isManual() const
{
    return m_manual;
}

bool CT_VirtualAbstractStep::isModifiable() const
{
    return (m_manual && m_modifiable && _inManager->getResultModelManager()->checkIfAllInputModelsWasCorrectlyConfigured());
}

int CT_VirtualAbstractStep::getExecuteTime() const
{
    return _elapsed;
}

int CT_VirtualAbstractStep::getProgress() const
{
    return _progress;
}

int CT_VirtualAbstractStep::getErrorCode() const
{
    return _error_code;
}

QString CT_VirtualAbstractStep::getErrorMessage(int error_code) const
{
    if(error_code != 0)
    {
        QString errMess = _errorMessageMap.value(error_code, "");

        if(!errMess.isEmpty())
            return errMess;

        return tr("Erreur %1").arg(error_code);
    }

    return tr("Aucune erreur detecte");
}

QString CT_VirtualAbstractStep::getToolTip() const
{
    // TODO
    /*QVariantList settings = getAllSettings();
    QListIterator<QVariant> it(settings);
    QListIterator<QList<Result*> > itRes(_inResultManager->getAllInputResults());

    QString str = getStepName();

    if(itRes.hasNext())
    {
        QListIterator<Result*> itRes2(itRes.next());

        if (itRes2.hasNext())
        {
            str += QString("\nInRes= %1").arg(itRes2.next()->getId());

            while(itRes2.hasNext())
            {
                str += QString(",%1").arg(itRes2.next()->getId());
            }

        }

        while(itRes.hasNext())
        {
            QListIterator<Result*> itRes2(itRes.next());

            if (itRes2.hasNext())
            {
                str += QString(" ; %1").arg(itRes2.next()->getId());

                while(itRes2.hasNext())
                {
                    str += QString(",%1").arg(itRes2.next()->getId());
                }

            }
        }
    }

    if(it.hasNext())
    {
        str += "\nParam: " + it.next().toString();

        while(it.hasNext())
        {
            str += "/" + it.next().toString();
        }
    }

    return str;*/

    return getStepDescription();
}

QString CT_VirtualAbstractStep::getStepName() const
{
    return metaObject()->className();
}

QString CT_VirtualAbstractStep::getStepExtendedName() const
{
    return getStepName() + " (" + QString().setNum(uniqueID()) + ")";
}

QString CT_VirtualAbstractStep::getStepDisplayableName() const
{
    return getStepName();
}

QString CT_VirtualAbstractStep::getStepExtendedDisplayableName() const
{
    return getStepDisplayableName() + " (" + QString().setNum(uniqueID()) + ")";
}

QString CT_VirtualAbstractStep::getStepCustomName() const
{
    if(_customName.isEmpty())
        return getStepExtendedDisplayableName();

    return _customName;
}

void CT_VirtualAbstractStep::setStepCustomName(QString customName)
{
    if(_customName != customName)
    {
        _customName = customName;
        emit nameChanged();
    }
}

QString CT_VirtualAbstractStep::getStepDescription() const
{
    return tr("No description for this step");
}

QString CT_VirtualAbstractStep::getStepDetailledDescription() const
{
    return tr("No detailled description for this step");
}

QString CT_VirtualAbstractStep::getStepURL() const
{
    return getPlugin()->getPluginURL();
}

int CT_VirtualAbstractStep::nResult() const
{
    return _outManager->getResultManager()->nResults();
}

CT_ResultGroup* CT_VirtualAbstractStep::getResult(int n) const
{
    return (CT_ResultGroup*)_outManager->getResultManager()->getResult(n);
}

void CT_VirtualAbstractStep::clearOutResult()
{
    setClearMemoryProgress(0);

    const int size = nResult();

    for(int i=0; i<size; ++i) {
        removeResult(0);
        setClearMemoryProgress((i*100)/size);
    }

    setClearMemoryProgress(100);
}

void CT_VirtualAbstractStep::clearOutResultFromMemory()
{
    setClearMemoryProgress(0);

    const int size = nResult();

    for(int i=0; i<size; ++i)
    {
        CT_ResultGroup* res = getResult(i);

        emit resultToBeClearedFromMemory(res);

        res->setBusy(true);
        res->clearFromMemory();
        res->setBusy(false);

        setClearMemoryProgress(((i+1)*100)/size);
    }

    setClearMemoryProgress(100);
}

void CT_VirtualAbstractStep::recursiveClearOutResult()
{
    recursiveClearChildOutResult();
    clearOutResult();
}

void CT_VirtualAbstractStep::recursiveClearChildOutResult()
{
    QListIterator<CT_VirtualAbstractStep*> it(_stepChildList);

    while(it.hasNext())
        it.next()->recursiveClearOutResult();
}

void CT_VirtualAbstractStep::recursiveClearOutResultFromMemory()
{
    QListIterator<CT_VirtualAbstractStep*> it(_stepChildList);

    while(it.hasNext())
        it.next()->recursiveClearOutResultFromMemory();

    clearOutResultFromMemory();
}

bool CT_VirtualAbstractStep::isSettingsModified() const
{
    return _isSettingsModified;
}

bool CT_VirtualAbstractStep::isDebugModeOn() const
{
    return _debugMode;
}

void CT_VirtualAbstractStep::execute()
{
    runProcessing();
}

void CT_VirtualAbstractStep::executeModify()
{
    if(isModifiable())
        runProcessing(true);
}

bool CT_VirtualAbstractStep::showInputResultConfigurationDialog()
{
    return configureInputResult();
}

bool CT_VirtualAbstractStep::showPreConfigurationDialog()
{
    //cree la fenetre de pre-configuration
    createPreConfigurationDialog();

    return preConfigure();
}

bool CT_VirtualAbstractStep::showPostConfigurationDialog()
{
    // cree la fenetre de post-configuration
    createPostConfigurationDialog();

    return postConfigure();
}

void CT_VirtualAbstractStep::ackDebugMode(int jump_n_step)
{
    _jump_n_step = jump_n_step; // store the value, for it to be accessible in the step (see CT_StepEndLoop for use exemple)
    _index_step_step_to_go += jump_n_step;
    _ackOfDebugMode = true;
}

void CT_VirtualAbstractStep::setGuiContext(const GuiContextInterface *context)
{
    m_guiContext = (GuiContextInterface*)context;
}

void CT_VirtualAbstractStep::ackManualMode()
{
    if(m_firstCallToManualMode)
        PS_LOG->addMessage(LogInterface::info, LogInterface::step, getStepCustomName() + tr(" enter manual mode"));

    if(m_firstCallToManualMode
            && getGuiContext() != NULL)
    {
        initManualMode();
        m_firstCallToManualMode = false;
    }

    if(getGuiContext() != NULL)
        useManualMode();
}

void CT_VirtualAbstractStep::setIsAPrototype()
{
    m_isAPrototype = true;
}

////////// PROTECTED ////////

// RESULTAT ET ITEMDRAWABLE EN ENTREE //

bool CT_VirtualAbstractStep::addInResultModel(CT_InAbstractResultModel *model)
{
    if(!_inManager->getResultModelManager()->addResultModel(model))
    {
        qFatal(QString("Erreur lors de l'ajout d'un CT_InAbstractResultModel dans l'étape %1").arg(getStepCustomName()).toLatin1());
        return false;
    }

    model->setStep(this);

    return true;
}

CT_InResultModelGroup* CT_VirtualAbstractStep::createNewInResultModel(const QString &uniqueName,
                                                                      const QString &displayableName,
                                                                      const QString &description,
                                                                      bool recursive)
{
    if(uniqueName.isEmpty())
        return NULL;

    CT_InResultModelGroup *model = new CT_InResultModelGroup(uniqueName,
                                                             NULL,
                                                             displayableName,
                                                             description,
                                                             recursive);

    if(addInResultModel(model))
        return model;

    delete model;
    return NULL;
}

CT_InResultModelGroupToCopy *CT_VirtualAbstractStep::createNewInResultModelForCopy(const QString &uniqueName,
                                                                                   const QString &displayableName,
                                                                                   const QString &description,
                                                                                   bool recursive)
{
    if(uniqueName.isEmpty())
        return NULL;

    CT_InResultModelGroupToCopy *model = new CT_InResultModelGroupToCopy(uniqueName,
                                                                         NULL,
                                                                         displayableName,
                                                                         description,
                                                                         recursive);

    if(addInResultModel(model))
        return model;

    delete model;
    return NULL;
}

bool CT_VirtualAbstractStep::setNotNeedInputResult()
{
    CT_InResultModelNotNeedInputResult *model = new CT_InResultModelNotNeedInputResult();

    if(addInResultModel(model))
        return true;

    delete model;
    return false;
}

CT_InAbstractResultModel* CT_VirtualAbstractStep::getInResultModel(const QString &uniqueName) const
{
    return _inManager->getResultModelManager()->resultModelByUniqueName(uniqueName);
}

CT_InAbstractModel* CT_VirtualAbstractStep::getInModelForResearch(const CT_OutAbstractResultModel *inResultModel,
                                                                  const QString &uniqueName) const
{
    Q_ASSERT(inResultModel != NULL);

    return _inManager->getResultModelManager()->modelByUniqueName(*inResultModel, uniqueName);
}

CT_InAbstractModel* CT_VirtualAbstractStep::getInModelForResearch(const CT_ResultGroup *inResult,
                                                                  const QString &uniqueName) const
{
    Q_ASSERT(inResult != NULL);

    return getInModelForResearch(inResult->model(), uniqueName);
}

CT_InAbstractModel* CT_VirtualAbstractStep::getInModelForResearch(const QString &inResultUniqueName,
                                                                  const int &possibilitySelectedIndex,
                                                                  const QString &uniqueName) const
{
    CT_InAbstractResultModel* inResModel = getInResultModel(inResultUniqueName);

    Q_ASSERT(inResModel != NULL);

    QList<CT_InStdModelPossibility*> p = inResModel->getPossibilitiesSavedSelected();

    Q_ASSERT((possibilitySelectedIndex > 0) && (possibilitySelectedIndex < p.size()));

    return getInModelForResearch((CT_OutAbstractResultModel*)p.at(possibilitySelectedIndex)->outModel(), uniqueName);
}

QList<CT_ResultGroup*> CT_VirtualAbstractStep::getInputResults() const
{
    QList<CT_ResultGroup*> ret;
    QList<CT_AbstractResult*> list = _inManager->getResultManager()->getResults();

    while(!list.isEmpty())
        ret.append(dynamic_cast<CT_ResultGroup*>(list.takeFirst()));

    return ret;
}

QList<CT_InResultsByModel*> CT_VirtualAbstractStep::getInputResultsSortedByModels() const
{
    return _inManager->getResultManager()->getResultsSortedByModels();
}

QList<CT_ResultGroup*> CT_VirtualAbstractStep::getInputResultsForModel(const CT_InAbstractResultModel *resultModel) const
{
    QList<CT_ResultGroup*> outList;

    QList<CT_InResultsByModel*> inResultByModels = getInputResultsSortedByModels();
    QListIterator<CT_InResultsByModel*> it(inResultByModels);

    while(it.hasNext())
    {
        CT_InResultsByModel *resM = it.next();

        // si ce modèle correspond à celui à exporter
        if(resM->getInModel() == resultModel)
        {
            outList.append((const QList<CT_ResultGroup*>&)resM->getResults());
            return outList;
        }
    }

    return outList;
}

QList<CT_ResultGroup*> CT_VirtualAbstractStep::getInputResultsForModel(const QString &inResultUniqueName) const
{
    return getInputResultsForModel(getInResultModel(inResultUniqueName));
}

bool CT_VirtualAbstractStep::isCreateDefaultOutModelActive() const
{
    return _defaultOutModelCreating;
}

// RESULTAT ET ITEMDRAWABLE EN SORTIE //

bool CT_VirtualAbstractStep::addOutResultModel(CT_OutAbstractResultModel *model)
{
    if(!_outManager->getResultModelManager()->addResultModel(model))
    {
        qFatal(QString("Erreur lors de l'ajout d'un CT_OutResultModelGroup dans l'étape %1").arg(getStepCustomName()).toLatin1());
        return false;
    }

    model->setStep(this);

    return true;
}

CT_OutResultModelGroup* CT_VirtualAbstractStep::createNewOutResultModel(const QString &uniqueName,
                                                                        const QString &resultName,
                                                                        const QString &displayableName,
                                                                        const QString &description)
{
    QString mn = uniqueName;

    if(mn.isEmpty())
        mn = CT_GenerateOutResultModelName().getNewResultModelNameThatDontExistIn(*_outManager->getResultModelManager());

    CT_OutResultModelGroup *model = new CT_OutResultModelGroup(mn,
                                                               NULL,
                                                               resultName,
                                                               displayableName,
                                                               description);

    if(addOutResultModel(model))
        return model;

    delete model;
    return NULL;
}

CT_OutResultModelGroupToCopyPossibilities* CT_VirtualAbstractStep::createNewOutResultModelToCopy(const QString &inUniqueName)
{
    if(inUniqueName.isEmpty())
        return NULL;

    CT_InResultModelGroupToCopy *inModel = dynamic_cast<CT_InResultModelGroupToCopy*>(getInResultModel(inUniqueName));

    if(inModel == NULL)
        return NULL;

    CT_GenerateOutResultModelName gen;

    CT_OutResultModelGroupToCopyPossibilities *outModel = new CT_OutResultModelGroupToCopyPossibilities();

    // on récupère toutes les possibilités que l'utilisateur à défini
    QList<CT_OutResultModelGroupToCopyPossibility*> copyList = inModel->getOutResultModelGroupsSelectedToCopy();
    QListIterator<CT_OutResultModelGroupToCopyPossibility*> it(copyList);

    if(it.hasNext()) {
        // pour chaque possibilité à copier
        while(it.hasNext())
        {
            CT_OutResultModelGroupToCopyPossibility *outResModelToCopy = it.next();

            QString newResultModelName = gen.getNewResultModelNameThatDontExistIn(*_outManager->getResultModelManager());
            QList<QString> generated;
            generated.append(newResultModelName);

            while(outResModelToCopy->outModelForModification()->existInTree(newResultModelName)) {
                newResultModelName = gen.getNewResultModelNameThatDontExistIn(*_outManager->getResultModelManager(), generated);
                generated.append(newResultModelName);
            }

            CT_OutResultModelGroupCopy *rModel = new CT_OutResultModelGroupCopy(newResultModelName,
                                                                                outResModelToCopy);


            // on crée un nouveau résultat de sortie contenant le résultat modèle modifié
            if(!addOutResultModel(rModel))
            {
                delete rModel;

                delete outModel;
                return NULL;
            }

            rModel->setInResultCopyModel(inModel);

            outModel->addResulModel(rModel);
        }
    }
    else
    {
        delete outModel;
        return NULL;
    }

    // TODO : memory leak !!!!
    return outModel;
}

bool CT_VirtualAbstractStep::addOutResultModelCopy(CT_InResultModelGroupToCopy *inModel,
                                                   const QList<CT_AbstractOutModelCopyAction*> &actions)
{
    if(inModel == NULL)
        return false;

    CT_GenerateOutResultModelName gen;

    // on récupère toutes les possibilités que l'utilisateur à défini
    QList<CT_OutResultModelGroupToCopyPossibility*> copyList = inModel->getOutResultModelGroupsSelectedToCopy();
    QListIterator<CT_OutResultModelGroupToCopyPossibility*> it(copyList);

    // pour chaque possibilité à copier
    while(it.hasNext())
    {
        CT_OutResultModelGroupToCopyPossibility *outResModelToCopy = it.next();

        QListIterator<CT_AbstractOutModelCopyAction*> itA(actions);

        while(itA.hasNext())
        {
            if(!itA.next()->execute(this, outResModelToCopy))
                return false;
        }

        QString newResultModelName = gen.getNewResultModelNameThatDontExistIn(*_outManager->getResultModelManager());

        // on crée un nouveau résultat de sortie contenant le résultat modèle modifié
        if(!addOutResultModel(new CT_OutResultModelGroupCopy(newResultModelName,
                                                             outResModelToCopy)))
            return false;
    }

    return true;
}

CT_OutAbstractResultModel* CT_VirtualAbstractStep::getOutResultModel(const QString &uniqueName) const
{
    return _outManager->getResultModelManager()->resultModelByUniqueName(uniqueName);
}

CT_OutAbstractModel* CT_VirtualAbstractStep::getOutModelForCreation(const CT_ResultGroup *outResult, const QString &uniqueName) const
{
    Q_ASSERT((outResult != NULL) && (outResult->model() != NULL));

    return _outManager->getResultModelManager()->modelByUniqueName(*outResult->model(), uniqueName);
}

CT_OutAbstractSingularItemModel *CT_VirtualAbstractStep::getOutSingularItemModelForCreation(const CT_ResultGroup *outResult, const QString &uniqueName) const
{
    return (CT_OutAbstractSingularItemModel*)getOutModelForCreation(outResult, uniqueName);
}

CT_OutAbstractGroupModel *CT_VirtualAbstractStep::getOutGroupModelForCreation(const CT_ResultGroup *outResult, const QString &uniqueName) const
{
    return (CT_OutAbstractGroupModel*)getOutModelForCreation(outResult, uniqueName);
}

CT_OutAbstractItemAttributeModel *CT_VirtualAbstractStep::getOutItemAttributeModelForCreation(const CT_ResultGroup *outResult, const QString &uniqueName) const
{
    return (CT_OutAbstractItemAttributeModel*)getOutModelForCreation(outResult, uniqueName);
}

QList<CT_ResultGroup*> CT_VirtualAbstractStep::getOutResultList() const
{
    const QList<CT_AbstractResult*> list = _outManager->getResultManager()->getResults();

    QList<CT_ResultGroup*> ret;
    ret.reserve(list.size());

    for(CT_AbstractResult* res : list) {
        ret.append(static_cast<CT_ResultGroup*>(res));
    }

    return ret;
}

CT_ResultGroup* CT_VirtualAbstractStep::getOutputResultForModel(const QString &outResultUniqueName) const
{
    return getOutputResultForModel(getOutResultModel(outResultUniqueName));
}

CT_ResultGroup* CT_VirtualAbstractStep::getOutputResultForModel(const CT_OutAbstractResultModel *outResultModel) const
{
    if(outResultModel == NULL)
        return NULL;

    QList<CT_AbstractResult*> results = _outManager->getResultManager()->getResults();
    QListIterator<CT_AbstractResult*> it(results);

    while(it.hasNext())
    {
        CT_AbstractResult *res = it.next();

        // si ce modèle correspond à celui passé en paramètre
        if(res->model() == outResultModel)
            return dynamic_cast<CT_ResultGroup*>(res);
    }

    return NULL;
}

// CONFIGURATION //

CT_StepConfigurableDialog* CT_VirtualAbstractStep::newStandardPreConfigurationDialog()
{
    delete _preConfigDialog;
    _preConfigDialog = new CT_StepConfigurableDialog();
    _preConfigDialog->setWindowTitle(_preConfigDialog->windowTitle() + QString(" (%1)").arg(_customName.isEmpty() ? getStepCustomName() : _customName));
    return _preConfigDialog;
}

bool CT_VirtualAbstractStep::preConfigure()
{
    if(_preConfigDialog != NULL)
    {
        if(CT_ConfigurableWidgetToDialog::exec(_preConfigDialog) == QDialog::Accepted)
        {
            setSettingsModified(_preConfigDialog->isSettingsModified());

            return true;
        }

        return false;
    }

    return true;
}

bool CT_VirtualAbstractStep::configureInputResult(bool forceReadOnly)
{
    const bool firstTime = mustCreateInputResultModels();

    if(firstTime) {
        if(!createInputResultModels())
            return false;
    }

    const CT_InResultModelConfigurationManager::CreateDialogReturn crReturn = _inManager->getResultModelConfigurationManager()->createInResultModelConfigurationDialog();

    // If it was an error (does not happen)
    if(crReturn == CT_InResultModelConfigurationManager::CreateError) {
        qFatal("Erreur lors de la creation de la fenetre de configuration des resultats d'entree");
        return false;
    }

    if(!_stepChildList.isEmpty()
            || (!firstTime && crReturn == CT_InResultModelConfigurationManager::CreateNotNecessary)
            || forceReadOnly) {

        _inManager->getResultModelConfigurationManager()->showReadOnlyInResultModel();
        return true;
    }

    CT_InResultModelConfigurationManager::ConfigureReturn cReturn = _inManager->getResultModelConfigurationManager()->configureInResultModel();

    if(cReturn == CT_InResultModelConfigurationManager::ConfigureSuccess) {     // If modification has been made by the user (user click the "Ok" button)

        _inManager->getResultModelManager()->createSearchModelCollection();

        setSettingsModified(true);
        return true;
    }

    if(cReturn == CT_InResultModelConfigurationManager::ConfigureNotChanged)    // If nothing was modified (user click the "Cancel" button)
        return true;
    else if(cReturn == CT_InResultModelConfigurationManager::ConfigureError)    // If it was an error (does not happen)
        qFatal("Erreur lors de la définition des modèles d'entrées");

    return false;
}

CT_StepConfigurableDialog* CT_VirtualAbstractStep::newStandardPostConfigurationDialog()
{
    delete _postConfigDialog;
    _postConfigDialog = new CT_StepConfigurableDialog();
    _postConfigDialog->setWindowTitle(_postConfigDialog->windowTitle() + QString(" (%1)").arg(_customName.isEmpty() ? getStepCustomName() : _customName));

    return _postConfigDialog;
}

bool CT_VirtualAbstractStep::postConfigure()
{
    if(_postConfigDialog != NULL)
    {
        if(CT_ConfigurableWidgetToDialog::exec(_postConfigDialog) == QDialog::Accepted)
        {
            setSettingsModified(_postConfigDialog->isSettingsModified());

            return true;
        }

        return false;
    }

    return true;
}

void CT_VirtualAbstractStep::setSettingsModified(bool modified)
{
    if(_isSettingsModified != modified)
    {
        _isSettingsModified = modified;
    }

    emit settingsModified();
}

// TRAITEMENT ET MODE DEBUG //

void CT_VirtualAbstractStep::setProgress(float progress)
{
    if(((int)progress) != _progress)
    {
        _progress = (int)progress;
        _elapsed = _execute_time.elapsed();

        emit inProgress(_progress);
        emit elapsed(_elapsed);
    }
}

void CT_VirtualAbstractStep::setErrorCode(int errorCode)
{
    _error_code = errorCode;
}

void CT_VirtualAbstractStep::setErrorMessage(int errorCode, const QString &errorMessage)
{
    _errorMessageMap.insert(errorCode, errorMessage);
}

void CT_VirtualAbstractStep::clearErrorMessage()
{
    _errorMessageMap.clear();
}

void CT_VirtualAbstractStep::pauseProcessing()
{
    if(!_paused)
    {
        _paused = true;

        emit paused();

        QMutex mutex;

        while((!_paused)
                && (!_stop))
        {
            mutex.lock();
            QWaitCondition().wait(&mutex, 100);
            mutex.unlock();
        }
    }
}

void CT_VirtualAbstractStep::setDebuggable(bool debuggable)
{
    _debuggable = debuggable;
}

void CT_VirtualAbstractStep::waitForAckIfInDebugMode()
{
    if(_debugMode)
    {
        if(_step_by_step_size == _index_step_step_to_go)
        {
            _inManager->getResultManager()->setResultsBusy(false);

            // preWaitForAckIfInDebugMode() et postWaitForAckIfInDebugMode() sont appelé
            // par le GUI

            _ackOfDebugMode = false;
            emit waitForAckInDebugMode();

            QMutex mutex;

            while((_index_step_step_to_go != -1)
                    && (!_ackOfDebugMode)
                    && (!_stop))
            {
                mutex.lock();
                QWaitCondition().wait(&mutex, 100);
                mutex.unlock();
            }

            _inManager->getResultManager()->setResultsBusy(true);
        }

        ++_step_by_step_size;
    }
}

int CT_VirtualAbstractStep::currentDebugBreakPointNumber() const
{
    return _step_by_step_size;
}

void CT_VirtualAbstractStep::setModifiable(bool modifiableOn)
{
    m_modifiable = modifiableOn;
}

void CT_VirtualAbstractStep::setManual(bool manualModeOn)
{
    m_manual = manualModeOn;
}

void CT_VirtualAbstractStep::requestManualMode()
{
    if(m_manual)
    {
        m_mustQuitManualMode = false;
        _inManager->getResultManager()->setResultsBusy(false);

        emit manualModeRequired();

        QMutex mutex;

        while(!m_mustQuitManualMode
                && (!isStopped()))
        {
            mutex.lock();
            QWaitCondition().wait(&mutex, 100);
            mutex.unlock();
        }

        _inManager->getResultManager()->setResultsBusy(true);

        emit manualModeCompleted();
    }
}

void CT_VirtualAbstractStep::initManualMode()
{
}

void CT_VirtualAbstractStep::useManualMode(bool quit)
{
    Q_UNUSED(quit)
}

GuiContextInterface* CT_VirtualAbstractStep::getGuiContext() const
{
    return m_guiContext;
}

bool CT_VirtualAbstractStep::isAPrototype() const
{
    return m_isAPrototype;
}

void CT_VirtualAbstractStep::setClearMemoryProgress(float progress)
{
    if(((int)progress) != _progressClearMem)
    {
        _progressClearMem = (int)progress;

        emit cleanMemoryInProgress(_progressClearMem);
    }
}

////////// PRIVATE ////////

void CT_VirtualAbstractStep::setParentStep(CT_VirtualAbstractStep *parent)
{
    _data->setParentStep(parent);
}

void CT_VirtualAbstractStep::setLaunchTime(QDateTime launchTime)
{
    _launch_time = launchTime;
}

void CT_VirtualAbstractStep::runProcessing(bool modificationMode)
{
    PS_LOG->addMessage(LogInterface::trace, LogInterface::step,  getStepCustomName() + tr(" - Start computing at \t%1").arg(QTime::currentTime().toString("hh:mm:ss")));

    _running = true;
    _stop = false;

    if(modificationMode) // supprime tous les anciens resultats des etapes enfants
        recursiveClearChildOutResult();
    else // supprime tous les anciens resultats ainsi que ceux des etapes enfants
        recursiveClearOutResult();

    // en fonction des resultats modeles configurer on cree les resultats à utiliser
    bool canCompute = _inManager->getResultManager()->retrieveResults(parentStep());

    // debut du lancement
    setLaunchTime(QDateTime::currentDateTime());
    _step_by_step_size = 0;
    _index_step_step_to_go = 0;
    setErrorCode(-2);

    _execute_time.start();
    setProgress(0);

    m_firstCallToManualMode = true;

    PS_MODELS->clearCache();

    // en fonction des resultats modeles de sortie on cree les resultats de sortie que l'etape
    // pourra utiliser dans sa methode compute.
    if(!modificationMode)
        canCompute = _outManager->getResultManager()->createResults();

    if(canCompute)
    {
        // crée la liste de recherche des modèles d'entrée pour le tour courant
        _inManager->getResultModelManager()->createSearchModelCollection();

        // crée la liste de recherche des modèles de sortie pour le tour courant
        _outManager->getResultModelManager()->createSearchModelCollection();

        if(_inManager->getResultModelManager()->needInputResults())
            canCompute = _inManager->getResultManager()->existResults();

        if(canCompute)
        {
            setErrorCode(0);

            _inManager->getResultManager()->setResultsBusy(true);

            // le traitement lui-meme
            if(modificationMode)
                modify();
            else
                compute();

            _inManager->getResultManager()->setResultsBusy(false);

            // si l'utilisateur a stopper le traitement
            if(isStopped())
            {
                // on supprime tous les resultats genere
                setProgress(0);
                clearOutResult();
            }
            else if(getErrorCode() == 0)
            {
                // on peut dire au ResultManager que les résultats du tour courant sont OK
                if(!modificationMode)
                    _outManager->getResultManager()->finalizeResults();

                setSettingsModified(false);
                setProgress(100);
            }
        }
    }

    if(!canCompute)
        setErrorCode(-3);

    _running = false;

    int elaps = _execute_time.elapsed();
    int hour = elaps/3600000;
    elaps -= hour*3600000;
    int min = elaps/60000;
    elaps -= min*60000;
    int sec = elaps/1000;
    elaps -= sec*1000;
    int ms = elaps;

    PS_LOG->addMessage(LogInterface::trace, LogInterface::step,  getStepCustomName() + tr(" - Computing completed, elapsed time:\t\t%1h:%2m:%3s:%4ms").arg(hour).arg(min).arg(sec).arg(ms));

    emit isCompleted();
}

void CT_VirtualAbstractStep::removeResult(int n)
{
    CT_ResultGroup* res = (CT_ResultGroup*)_outManager->getResultManager()->takeResult(n);

    emit resultToBeClearedFromMemory(res);

    res->setBusy(true);
    res->clearFromMemory();
    res->setBusy(false);

    emit resultToBeRemoved(res);

    delete res;
}

QList<CT_OutAbstractResultModel*> CT_VirtualAbstractStep::getOutResultModels() const
{
    return _outManager->getResultModelManager()->models();
}

void CT_VirtualAbstractStep::setCreateDefaultOutModelActive(bool active)
{
    _defaultOutModelCreating = active;
}

CT_OutManager* CT_VirtualAbstractStep::getOutManager() const
{
    return _outManager;
}
