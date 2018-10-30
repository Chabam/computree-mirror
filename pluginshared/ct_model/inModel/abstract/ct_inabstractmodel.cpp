#include "ct_inabstractmodel.h"

#include "ct_model/inModel/tools/ct_instdmodelpossibility.h"
#include "ct_model/inModel/tools/ct_instdmodelpossibilitygroup.h"
#include "ct_model/outModel/abstract/ct_outabstractmodel.h"
#include <QDebug>

//#define DEBUG_MODELS 2

#ifdef DEBUG_MODELS

QString DEBUG_STR_FOR_MODELS;

#define DEBUG_APPEND QString("\t")
#define BEGIN_RECURSIVE_DEBUG DEBUG_STR_FOR_MODELS += DEBUG_APPEND;
#define END_RECURSIVE_DEBUG DEBUG_STR_FOR_MODELS.remove(DEBUG_STR_FOR_MODELS.size()-DEBUG_APPEND.size(), DEBUG_APPEND.size());

#define DEBUG_MODEL_SEARCH(strToPrint) if(savePossibilities) { qDebug() << DEBUG_STR_FOR_MODELS << uniqueName() << " => " << strToPrint; }

    #if DEBUG_MODELS > 1
        #define DEBUG_MODEL_SEARCH_PRECISE_1(strToPrint) DEBUG_MODEL_SEARCH(strToPrint);
    #else
        #define DEBUG_MODEL_SEARCH_PRECISE_1(strToPrint) ;
    #endif

#else
    #define BEGIN_RECURSIVE_DEBUG ;
    #define END_RECURSIVE_DEBUG ;
    #define DEBUG_MODEL_SEARCH(strToPrint) ;
    #define DEBUG_MODEL_SEARCH_PRECISE_1(strToPrint) ;
#endif

CT_InAbstractModel::CT_InAbstractModel(const QString &uniqueName,
                                       const QString &description,
                                       const QString &displayableName) : CT_AbstractModel(uniqueName,
                                                                                          description,
                                                                                          displayableName)
{
    m_choiceMode = CT_InAbstractModel::C_DontChoose;
    m_finderMode = CT_InAbstractModel::F_IsObligatory;

    m_possibilitiesGroup = new CT_InStdModelPossibilityGroup();
    m_nPossibilitiesWithoutSaving = 0;
}

CT_InAbstractModel::~CT_InAbstractModel()
{
    clearPossibilitiesSaved();

    delete m_possibilitiesGroup;
}

CT_InAbstractModel::ChoiceMode CT_InAbstractModel::choiceMode() const
{
    return m_choiceMode;
}

CT_InAbstractModel::FinderMode CT_InAbstractModel::finderMode() const
{
    return m_finderMode;
}

void CT_InAbstractModel::startSaveCycle()
{
    m_saveCycles.append(CT_InModelSaveCycle());
}

void CT_InAbstractModel::finishSaveCycle()
{
}

int CT_InAbstractModel::nSaveCycle() const
{
    return m_saveCycles.size();
}

bool CT_InAbstractModel::needOutputModel() const
{
    return true;
}

void CT_InAbstractModel::recursiveVisitPossibilities(std::function<void (CT_InAbstractModel*, CT_InStdModelPossibility*)>& visitor)
{
    visitPossibilities(visitor);

    QList<CT_AbstractModel*> c = childrens();

    for(CT_AbstractModel* child : c) {
        static_cast<CT_InAbstractModel*>(child)->recursiveVisitPossibilities(visitor);
    }
}

void CT_InAbstractModel::visitPossibilities(std::function<void (CT_InAbstractModel*, CT_InStdModelPossibility*)>& visitor)
{
    const QList<CT_InStdModelPossibility*>& possibilities = m_possibilitiesGroup->getPossibilities();

    for(CT_InStdModelPossibility* possibility : possibilities) {
        visitor(this, possibility);
    }
}

bool CT_InAbstractModel::recursiveFindOnePossibilityInModel(const CT_OutAbstractModel &model,
                                                            bool savePossibilities)
{
    m_nPossibilitiesWithoutSaving = 0;

    DEBUG_MODEL_SEARCH("---------- recursiveFindOnePossibilityInModel ------------");
    return recursiveFindPossibilitiesInModel(model, savePossibilities, false) != 0;
}

bool CT_InAbstractModel::recursiveFindAllPossibilitiesInModel(const CT_OutAbstractModel &model,
                                                              bool savePossibilities)
{
    m_nPossibilitiesWithoutSaving = 0;

    DEBUG_MODEL_SEARCH("---------- recursiveFindAllPossibilitiesInModel ------------");
    return recursiveFindPossibilitiesInModel(model, savePossibilities, true) != 0;
}

bool CT_InAbstractModel::isAtLeastOnePossibilitySelected() const
{
    // if we have no possibilities : we return false
    if(nPossibilitiesSaved() == 0)
        return false;

    bool ok = false;

    QListIterator<CT_InStdModelPossibility*> itP(m_possibilitiesGroup->getPossibilities());

    while(itP.hasNext())
    {
        CT_InStdModelPossibility *possibility = itP.next();

        if(possibility->isSelected())
            ok = true;
    }

    // if no possibilities of this model was selected : we return false
    if(!ok)
        return false;

    return true;
}

bool CT_InAbstractModel::recursiveIsAtLeastOnePossibilitySelected() const
{
    if(!isAtLeastOnePossibilitySelected())
        return false;

    QList<CT_AbstractModel*> l = childrensOfPossibilities();
    QListIterator<CT_AbstractModel*> it(l);

    while(it.hasNext())
    {
        // if no possibilities of this children (and recursively) is selected : we return false
        if(!((CT_InAbstractModel*)it.next())->recursiveIsAtLeastOnePossibilitySelected())
            return false;
    }

    // all it's ok
    return true;
}

bool CT_InAbstractModel::isAtLeastOnePossibilitySelectedIfItDoes(QStringList* errors) const
{
    // if this model not need possiblities
    if(!needOutputModel())
        return true;

    // if this model is optionnal : we return true
    if(finderMode() == CT_InAbstractModel::F_IsOptional)
        return true;

    bool ok = false;

    QListIterator<CT_InStdModelPossibility*> itP(m_possibilitiesGroup->getPossibilities());

    while(itP.hasNext())
    {
        CT_InStdModelPossibility *possibility = itP.next();

        if(possibility->isSelected())
            ok = true;
    }

    if(!ok && (errors != NULL))
        errors->append(tr("Le modèle %1 (%2) a %3 possibilité(s) sauvegardée(s) mais aucune de sélectionné").arg(displayableName()).arg(uniqueName()).arg(m_possibilitiesGroup->getPossibilities().size()));

    // if no possibilities of this model was selected : we return false
    return ok;
}

bool CT_InAbstractModel::recursiveIsAtLeastOnePossibilitySelectedIfItDoes(QStringList* errors) const
{
    // if this model not need possiblities
    if(!needOutputModel())
        return true;

    if(!isAtLeastOnePossibilitySelectedIfItDoes(errors))
        return false;

    QList<CT_AbstractModel*> l = childrensOfPossibilities();
    QListIterator<CT_AbstractModel*> it(l);

    while(it.hasNext())
    {
        CT_InAbstractModel *model = (CT_InAbstractModel*)it.next();

        // if no possibilities of this children (and recursively) is selected : we return false
        if(!model->recursiveIsAtLeastOnePossibilitySelectedIfItDoes(errors))
            return false;
    }

    // all it's ok
    return true;
}

int CT_InAbstractModel::nPossibilitiesSaved() const
{
    return m_possibilitiesGroup->nPossibilities();
}

QList<CT_InStdModelPossibility *> CT_InAbstractModel::getPossibilitiesSaved() const
{
    return m_possibilitiesGroup->getPossibilities();
}

QList<CT_InStdModelPossibility *> CT_InAbstractModel::getPossibilitiesSavedSelected() const
{
    QList<CT_InStdModelPossibility*> l;

    QListIterator<CT_InStdModelPossibility*> it(m_possibilitiesGroup->getPossibilities());

    while(it.hasNext())
    {
        CT_InStdModelPossibility *p = it.next();

        if(p->isSelected())
            l.append(p);
    }

    return l;
}

void CT_InAbstractModel::clearPossibilitiesSaved()
{
    while(deleteLastSaveCycle());
}

void CT_InAbstractModel::recursiveClearPossibilitiesSaved()
{
    clearPossibilitiesSaved();

    QList<CT_AbstractModel*> child = childrensOfPossibilities();
    QListIterator<CT_AbstractModel*> it(child);

    while(it.hasNext())
        ((CT_InAbstractModel*)it.next())->recursiveClearPossibilitiesSaved();
}

void CT_InAbstractModel::saveSettings(SettingsWriterInterface &writer) const
{
    writer.addParameter(this, "ModelName", uniqueName());
    writer.addParameter(this, "ModelType", metaObject()->className());
    writer.addParameter(this, "ChoiceMode", (int)choiceMode());
    writer.addParameter(this, "FinderMode", (int)finderMode());

    QListIterator<CT_InStdModelPossibility*> it(possibilitiesGroup()->getPossibilities());

    while(it.hasNext())
    {
        CT_InStdModelPossibility *possibility = it.next();

        const int pID = writer.addParameter(this, "Possibility", "");
        writer.addParameterInfo(this, pID, "IsSelected", possibility->isSelected() ? true : false);
        writer.addParameterInfo(this, pID, "OutModelName", possibility->outModel()->uniqueName());
        writer.addParameterInfo(this, pID, "OutModelType", possibility->outModel()->metaObject()->className());
    }

    writer.beginWriteChildrens(this);

    const QList<CT_AbstractModel*> childrens = childrensOfPossibilities();
    QListIterator<CT_AbstractModel*> itC(childrens);

    while(itC.hasNext())
        ((CT_InAbstractModel*)itC.next())->saveSettings(writer);

    writer.endWriteChildrens(this);
}

bool CT_InAbstractModel::restoreSettings(SettingsReaderInterface &reader)
{
    QVariant value;

    if(!reader.parameter(this, "ModelName", value) || value.toString() != uniqueName())
        return false;

    if(!reader.parameter(this, "ChoiceMode", value) || value.isNull() || value.toInt() != int(choiceMode()))
        return false;

    if(reader.parameterCount(this, "Possibility") != nPossibilitiesSaved())
        return false;

    QListIterator<CT_InStdModelPossibility*> it(possibilitiesGroup()->getPossibilities());

    while(it.hasNext())
    {
        CT_InStdModelPossibility *possibility = it.next();

        const int pID = reader.parameter(this, "Possibility", value);

        if(!reader.parameterInfo(this, pID, "OutModelName", value) || value.toString() != possibility->outModel()->uniqueName())
            return false;

        if(!reader.parameterInfo(this, pID, "IsSelected", value) || value.isNull())
            return false;

        possibility->setSelected(value.toBool());
    }

    reader.beginReadChildrens(this);

    QList<CT_AbstractModel*> c = childrensOfPossibilities();
    QListIterator<CT_AbstractModel*> itC(c);

    while(itC.hasNext())
    {
        if(!((CT_InAbstractModel*)itC.next())->restoreSettings(reader))
            return false;
    }

    reader.endReadChildrens(this);

    return true;
}

bool CT_InAbstractModel::deleteLastSaveCycle()
{
    if(!m_saveCycles.isEmpty())
    {
        CT_InModelSaveCycle cycle = m_saveCycles.takeLast();

        while(!cycle.isEmpty())
        {
            CT_InStdModelPossibility *p = cycle.takeLast();
            m_possibilitiesGroup->removePossibility(p);
            delete p;
        }

        return true;
    }

    return false;
}

void CT_InAbstractModel::addToPossibility(const CT_OutAbstractModel &model)
{
    //DEBUG_MODEL_SEARCH("p : " << model.uniqueName() << " for " << uniqueName());

    ++m_nPossibilitiesWithoutSaving;

    if(m_saveCycles.isEmpty())
        m_saveCycles.append(CT_InModelSaveCycle());

    CT_InStdModelPossibility *possibility = createNewPossibility();
    possibility->setOutModel(model);

    m_saveCycles.last().append(possibility);
    m_possibilitiesGroup->addPossibility(possibility);

    possibilityCreated(possibility);
}

CT_InStdModelPossibility* CT_InAbstractModel::createNewPossibility() const
{
    return new CT_InStdModelPossibility();;
}

bool CT_InAbstractModel::existModelInPossibilities(const CT_OutAbstractModel &model) const
{
    QListIterator<CT_InStdModelPossibility*> it(possibilitiesGroup()->getPossibilities());

    while(it.hasNext())
    {
        CT_OutAbstractModel *pOutModel = it.next()->outModel();
        if((pOutModel->uniqueName() == model.uniqueName())
                && (pOutModel->step() == model.step()))
            return true;
    }

    return false;
}

void CT_InAbstractModel::internalSetChoiceMode(CT_InAbstractModel::ChoiceMode mode)
{
    m_possibilitiesGroup->setEnable(true);

    if(mode == CT_InAbstractModel::C_ChooseOneIfMultiple)
        m_possibilitiesGroup->setExclusive(true);
    else if(mode == CT_InAbstractModel::C_ChooseMultipleIfMultiple)
        m_possibilitiesGroup->setExclusive(false);
    else if(mode != CT_InAbstractModel::C_DontChoose)
        m_possibilitiesGroup->setEnable(false);

    m_choiceMode = mode;
}

void CT_InAbstractModel::internalSetFinderMode(CT_InAbstractModel::FinderMode mode)
{
    m_finderMode = mode;
}

void CT_InAbstractModel::staticCopyPossibilitiesToModel(const CT_InAbstractModel *src, CT_InAbstractModel *dst)
{
    dst->clearPossibilitiesSaved();

    QListIterator< CT_InModelSaveCycle > it(src->m_saveCycles);

    while(it.hasNext())
    {
        CT_InModelSaveCycle newCycle;
        QListIterator<CT_InStdModelPossibility*> itP(it.next());

        while(itP.hasNext())
        {
            CT_InStdModelPossibility *cpy = itP.next()->copy();

            dst->m_possibilitiesGroup->addPossibility(cpy);
            newCycle.append(cpy);
        }

        dst->m_saveCycles.append(newCycle);

        itP = newCycle;

        while(itP.hasNext())
            dst->possibilityCreated(itP.next());
    }
}

CT_InStdModelPossibilityGroup* CT_InAbstractModel::possibilitiesGroup() const
{
    return m_possibilitiesGroup;
}

int CT_InAbstractModel::recursiveFindPossibilitiesInModel(const CT_OutAbstractModel &model, bool savePossibilities, bool searchMultiple)
{
    BEGIN_RECURSIVE_DEBUG

    if(!needOutputModel()) {
        END_RECURSIVE_DEBUG
        return 1;
    }

    int ok = 0;

    DEBUG_MODEL_SEARCH("canBeComparedWith(" << model.uniqueName() << ") ?");

    // if this model can be compared with the out model
    if(canBeComparedWith(model))
    {
        DEBUG_MODEL_SEARCH("YES");

        bool recursiveOk = false;

        // we select if we can go down in childrens of the out model and must compare them with this INPUT model
        if(mustCompareThisWithChildrenOfOutModel())
        {
            DEBUG_MODEL_SEARCH("Go recursively down in childrens of " << model.uniqueName());

            bool continueLoop = true;

            // get children of the output model
            QList<CT_AbstractModel*> child = model.childrens();
            QListIterator<CT_AbstractModel*> it(child);

            // call this method with the next children
            while(it.hasNext()
                  && continueLoop)
            {
                CT_OutAbstractModel *oam = (CT_OutAbstractModel*)it.next();

                // go down
                if(recursiveFindPossibilitiesInModel(*oam, savePossibilities, searchMultiple) != 0)
                {
                    recursiveOk = true;

                    if(!searchMultiple)
                        continueLoop = false;
                }
            }

            if(!continueLoop && (m_nPossibilitiesWithoutSaving > 0)) {
                DEBUG_MODEL_SEARCH_PRECISE_1("Error return 2");
                END_RECURSIVE_DEBUG
                return 2;
            }

            if(recursiveOk)
                ok = 2;
        }

        // we select if this model can be a possibility for this model
        if(canAddPossibilityForThisModel(model))
        {
            // get children of this model
            QList<CT_AbstractModel*> iChild = childrensToFindPossibilities(savePossibilities);
            QListIterator<CT_AbstractModel*> itI(iChild);

            bool atLeastOneChildrenIsObligatory = false;

            while(itI.hasNext() && !atLeastOneChildrenIsObligatory)
            {
                CT_InAbstractModel *iam = (CT_InAbstractModel*)itI.next();

                // if this model is obligatory we set the atLeastOneObligatory variable to inform that
                // at least one children is obligatory
                if(iam->finderMode() == CT_InAbstractModel::F_IsObligatory)
                    atLeastOneChildrenIsObligatory = true;
            }

            DEBUG_MODEL_SEARCH_PRECISE_1("mustCompareChildrenWithThisModel ?");

            // if we must compare the current OUTPUT model with childrens of this model
            if(mustCompareOutModelWithChildrenOfThisModel())
            {
                DEBUG_MODEL_SEARCH_PRECISE_1("YES");

                bool atLeastOneNotFounded = false;
                QList<bool> deleteLastSaveCycleList;

                itI.toFront();

                // we compare all children model of this model with the current output model
                while(itI.hasNext())
                {
                    CT_InAbstractModel *iam = (CT_InAbstractModel*)itI.next();
                    bool isOk = false;

                    inModelToBeCompared(iam, savePossibilities);

                    // we start a new save cycle
                    iam->startSaveCycle();

                    inModelToBeCompared(iam, &model, savePossibilities);

                    // if output model match criteria for the children and recursively
                    if(iam->recursiveFindPossibilitiesInModel(model, savePossibilities, searchMultiple) != 0)
                        isOk = true;
                    else if(iam->finderMode() == CT_InAbstractModel::F_IsObligatory)
                        atLeastOneNotFounded = true;

                    deleteLastSaveCycleList.append(isOk);

                    inModelCompared(iam, &model, savePossibilities);

                    // finish the save cycle
                    iam->finishSaveCycle();

                    inModelComparisonResult(iam, isOk, savePossibilities);
                }

                if(atLeastOneNotFounded)
                {
                    itI.toFront();

                    QListIterator<bool> itL(deleteLastSaveCycleList);

                    while(itI.hasNext() && itL.hasNext())
                    {
                        if(itL.next() == true)
                            ((CT_InAbstractModel*)itI.next())->deleteLastSaveCycle();
                    }

                    DEBUG_MODEL_SEARCH_PRECISE_1("one children is not ok but continue");
                }
                else
                {
                    ok = 1;
                }
            }
            else
            {
                DEBUG_MODEL_SEARCH_PRECISE_1("NO");
            }

            DEBUG_MODEL_SEARCH_PRECISE_1("mustCompareChildrenWithChildrenOfThisModel ?");

            // we select if we must compare children of this model with children of the output model
            if(mustCompareChildrenWithChildrenOfOutModel())
            {
                DEBUG_MODEL_SEARCH_PRECISE_1("YES");

                bool allOk = true;
                QList<bool> deleteLastSaveCycleList;

                itI.toFront();

                // get children of the output model
                QList<CT_AbstractModel*> oChild = model.childrens();
                QListIterator<CT_AbstractModel*> itO(oChild);

                // we compare all children model of this model with all children model of the output model
                while(itI.hasNext()
                      && allOk)
                {
                    CT_InAbstractModel *iam = (CT_InAbstractModel*)itI.next();
                    bool tmpOk = false;
                    bool continueLoop = true;

                    inModelToBeCompared(iam, savePossibilities);

                    // we start a new save cycle
                    iam->startSaveCycle();

                    itO.toFront();

                    while(itO.hasNext()
                          && continueLoop)
                    {
                        CT_OutAbstractModel *oam = (CT_OutAbstractModel*)itO.next();

                        inModelToBeCompared(iam, oam, savePossibilities);

                        // if output model match criteria for the children and recursively
                        if(iam->recursiveFindPossibilitiesInModel(*oam, savePossibilities, searchMultiple) != 0)
                        {
                            tmpOk = true;

                            if(!searchMultiple) {
                                DEBUG_MODEL_SEARCH("Don't continue loop");
                                continueLoop = false;
                            }
                        }

                        inModelCompared(iam, oam, savePossibilities);
                    }

                    // finish the save cycle
                    iam->finishSaveCycle();

                    if(!tmpOk)
                    {
                        iam->deleteLastSaveCycle();

                        if(iam->finderMode() == CT_InAbstractModel::F_IsObligatory)
                            allOk = false;
                    }

                    deleteLastSaveCycleList.append(tmpOk);

                    inModelComparisonResult(iam, tmpOk, savePossibilities);
                }

                if(allOk)
                {
                    ok = 1;
                }
                else
                {
                    itI.toFront();

                    QListIterator<bool> itL(deleteLastSaveCycleList);

                    while(itI.hasNext() && itL.hasNext())
                    {
                        if(itL.next() == true)
                            ((CT_InAbstractModel*)itI.next())->deleteLastSaveCycle();
                    }

                    DEBUG_MODEL_SEARCH_PRECISE_1("one children is not ok");
                    DEBUG_MODEL_SEARCH_PRECISE_1("return nPossibilitiesSaved() > 0 : " << m_nPossibilitiesWithoutSaving << " > 0");
                    possibilityNotCreated();

                    END_RECURSIVE_DEBUG
                    return ok;
                    //return (m_nPossibilitiesWithoutSaving > 0) || ok;
                }
            }
            else
            {
                DEBUG_MODEL_SEARCH_PRECISE_1("NO");
            }

            // if this model have no children or if always are optionnal we consider that is ok
            if(!atLeastOneChildrenIsObligatory)
                ok = 1;
        }

        // if all criteria are met
        if(ok == 1)
        {
            if(searchMultiple || (nPossibilitiesSaved() == 0))
            {
                // if this model already exist in possibilities
                if(existModelInPossibilities(model))
                    possibilityNotCreated();
                else if(savePossibilities) { // if we must save possibilities, we add this model to possibilities
                    DEBUG_MODEL_SEARCH("addToPossibility( " << model.uniqueName() << " )");
                    addToPossibility(model);
                }
                else if(!savePossibilities) {
                    possibilityNotCreated();
                    ++m_nPossibilitiesWithoutSaving;
                }
            }
            else
            {
                possibilityNotCreated();
            }
        }
        else
        {
            DEBUG_MODEL_SEARCH(model.uniqueName() << " not added");
        }

        // if this model is optionnal we consider that is ok but
        // we don't have saved the model to possibilities
        //if(finderMode() == CT_InAbstractModel::F_IsOptional)
            //ok = true;

        if(recursiveOk)
            ok = 2;
    }
    else
    {
        DEBUG_MODEL_SEARCH("NO");
    }

    DEBUG_MODEL_SEARCH("return " << ok);

    END_RECURSIVE_DEBUG

    return ok;
}
