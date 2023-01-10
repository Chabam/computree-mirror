#include "pb_stepexportattributesasascii.h"
#include "ct_log/ct_logmanager.h"
#include "tools/pb_tools.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <math.h>
#include <QDir>

PB_StepExportAttributesAsASCII::PB_StepExportAttributesAsASCII() : SuperClass()
{
    _replaceSpecialCharacters = true;
    _shortenNames = false;
}

QString PB_StepExportAttributesAsASCII::description() const
{
    return tr("Export d'attributs - ASCII");
}
QString PB_StepExportAttributesAsASCII::detailledDescription() const
{
    return tr("Cette étape permet d'exporter des attributs d'items situés dans un même groupe. Cette étape fonctionne également au sein d'une boucle, en prenant en compte les tours surccessifs. Tout attribut de n'importe quel item peut être exporté.<br>"
                "L'export est réalisé sous forme de fichier texte, avec une ligne par item (et par tour de boucle le cas échéant), et une colonne par attribut. Un seul fichier est produit, regroupant les données de tous les tours si on est dans une boucle.");
}

QString PB_StepExportAttributesAsASCII::inputDescription() const
{
    return SuperClass::inputDescription() + tr("<br><br>Il faut sélectionner les attributs à exporter.<br>"
                                               "Ces attributs peuvent appartenir à plusieurs items, tant que ces items sont dans un même groupe.<br>"
                                               "Lorsque l'export est au sein d'une boucle, il faut également sélectionner le compteur de boucle.");
}

QString PB_StepExportAttributesAsASCII::outputDescription() const
{
    return SuperClass::outputDescription() + tr("Cette étape ne génère pas de nouvelles données.");
}

QString PB_StepExportAttributesAsASCII::detailsDescription() const
{
    return tr("N.B. : les noms d'attributs sont simplifiés : suppression des accents, remplacement des espaces et caractères spéciaux par \"_\".");
}


CT_VirtualAbstractStep* PB_StepExportAttributesAsASCII::createNewInstance() const
{
    return new PB_StepExportAttributesAsASCII();
}

//////////////////// PROTECTED METHODS //////////////////


void PB_StepExportAttributesAsASCII::declareInputModels(CT_StepInModelStructureManager& manager)
{
    manager.addResult(mInResult, tr("Résultat"));
    manager.setZeroOrMoreRootGroup(mInResult, mInRootGroup);
    manager.addGroup(mInRootGroup, mInGroupMain);
    manager.addItem(mInGroupMain, mInItemWithAttribute, tr("Item avec des attributs"));
    manager.addItemAttribute(mInItemWithAttribute, mInItemAttribute, CT_AbstractCategory::DATA_VALUE, tr("Attribut à exporter"));

    manager.addResult(mInResultCounter, tr("Résultat compteur"), QString(), true);
    manager.setRootGroup(mInResultCounter, mInGroupCounter);
    manager.addItem(mInGroupCounter, mInLoopCounter, tr("Compteur"));
}

void PB_StepExportAttributesAsASCII::declareOutputModels(CT_StepOutModelStructureManager&)
{
}

void PB_StepExportAttributesAsASCII::fillPostInputConfigurationDialog(CT_StepConfigurableDialog* postInputConfigDialog)
{
    postInputConfigDialog->addTitle(tr("Export ASCII tabulaire (1 fichier en tout)"));
    postInputConfigDialog->addFileChoice(tr("Choix du fichier"), CT_FileChoiceButton::OneNewFile, tr("Fichier texte (*.txt)"), _outASCIIFileName, tr("S'il existe déjà, le fichier sera écrasé. Le fichier contiendra les données pour tous les tours de boucle. "));

    postInputConfigDialog->addEmpty();
    postInputConfigDialog->addBool(tr("Supprimer les caractères spéciaux dans les noms de champs"), "", "", _replaceSpecialCharacters, tr("Si cette case est cochée tous les caractères accentués seront remplacés par leur version non accentuée, et tous les caractères spéciaux seront remplacés par \"_\", dans les noms de métriques"));
    postInputConfigDialog->addBool(tr("Raccourcir les noms de métriques (à 10 caractères)"), "", "", _shortenNames, tr("Si cette case est cochée les noms de métriques sont raccourcis si nécessaire pour ne pas dépasser 10 caractères. Dans ce cas un fichier texte est produit contenant les correspondances entre noms complets et noms raccourcis. "));
}

void PB_StepExportAttributesAsASCII::compute()
{
    QScopedPointer<QFile> fileASCII;
    QScopedPointer<QTextStream> streamASCII;
    bool firstTurnFromCounter = true;
    _inLoop = false;

    const QString exportBaseName = createExportBaseName(firstTurnFromCounter, _inLoop);

    if (firstTurnFromCounter)
    {
        _names.clear();
        _modelsKeys.clear();

        computeModelsKeysAndNames(mInItemAttribute);
        PB_Tools::renameDuplicates(_names, _itemNames);

        if (_shortenNames) {_shortNames =  PB_Tools::computeShortNames(_names);}

        if (_replaceSpecialCharacters)
        {
            replaceSpecialCharacters(_names);
            if (_shortenNames) {replaceSpecialCharacters(_shortNames);}
        }

        if (_shortenNames && !_outASCIIFileName.isEmpty())
        {
            PB_Tools::createCorrespondanceFile(_outASCIIFileName.first(), _names, _shortNames);
        }

        std::sort(_modelsKeys.begin(), _modelsKeys.end());

        if (isStopped()) {return;}
    }

    if(isStopped() || !exportInAscii(fileASCII, streamASCII, firstTurnFromCounter))
        return;

    // IN results browsing
    for(const CT_StandardItemGroup* grp : mInGroupMain.iterateInputs(mInResult))
    {
        if(isStopped()) {return;}

        QMap<QString, QPair<const CT_AbstractSingularItemDrawable*, const CT_AbstractItemAttribute*> > indexedAttributes;

        auto iteratorItemWithAttribute = grp->singularItems(mInItemWithAttribute);


        for(const CT_AbstractSingularItemDrawable* item : iteratorItemWithAttribute)
        {
            auto iteratorAttributes = item->itemAttributesByHandle(mInItemAttribute);

            for(const CT_AbstractItemAttribute* attr : iteratorAttributes)
            {
                addToIndexedAttributesCollection(item, attr, indexedAttributes);
            }
        }

        const bool hasMetricsToExport = !(indexedAttributes.isEmpty());

        if (!streamASCII.isNull() && _inLoop)
        {
            (*streamASCII.data()) << exportBaseName << "\t";
        }

        for (int i = 0 ; i < _modelsKeys.size() ; i++)
        {
            const QString key = _modelsKeys.at(i);

            const auto pair = indexedAttributes.value(key);

            if (hasMetricsToExport && !streamASCII.isNull())
            {
                if (pair.first != nullptr)
                {
                    (*streamASCII.data()) << pair.second->toString(pair.first, nullptr);
                }

                if (i < _modelsKeys.size() - 1) {(*streamASCII.data()) << "\t";} else {(*streamASCII.data()) << "\n";}
            }
        }
    }
}

QString PB_StepExportAttributesAsASCII::createExportBaseName(bool& first, bool& inLoop) const
{
    inLoop = false;
    for(const CT_LoopCounter* counter : mInLoopCounter.iterateInputs(mInResultCounter))
    {
        inLoop = true;

        if (counter->currentTurn() > 1)
            first = false;

        QFileInfo fileinfo(counter->turnName());

        if (fileinfo.exists())
            return fileinfo.baseName();

        return counter->turnName();
    }

    return QString("noName");
}

void PB_StepExportAttributesAsASCII::computeModelsKeysAndNamesForModels(const CT_OutAbstractModel* itemModel, const CT_OutAbstractItemAttributeModel* attModel)
{
    const QString itemDN = itemModel->displayableName();
    const QString attrDN = attModel->displayableName();

    const QString key = computeKeyForModels(itemModel, attModel);

    if(!_modelsKeys.contains(key))
    {
        _modelsKeys.append(key);
        _names.insert(key, attrDN);
        _itemNames.insert(key, itemDN);
    }
}

QString PB_StepExportAttributesAsASCII::computeKeyForModels(const CT_OutAbstractModel* itemModel, const CT_OutAbstractModel* attModel) const
{
    return QString("ITEM_%1_ATTR_%2").arg(size_t(itemModel->recursiveOriginalModel())).arg(size_t(attModel->recursiveOriginalModel()));
}

bool PB_StepExportAttributesAsASCII::exportInAscii(QScopedPointer<QFile>& fileASCII, QScopedPointer<QTextStream>& streamASCII, const bool firstTurnFromCounter)
{
    if (!_outASCIIFileName.isEmpty())
    {
        fileASCII.reset(new QFile(_outASCIIFileName.first()));

        if (firstTurnFromCounter)
        {
            if (fileASCII->open(QIODevice::WriteOnly | QIODevice::Text))
            {
                streamASCII.reset(new QTextStream(fileASCII.data()));

                if (_inLoop)
                {
                    (*streamASCII) << "Name\t";
                }

                for (int i = 0 ; i < _modelsKeys.size() ; i++)
                {
                    QString metricName;
                    if (_shortenNames) {metricName = _shortNames.value(_modelsKeys.at(i));}
                                  else {metricName = _names.value(_modelsKeys.at(i));}

                    (*streamASCII) << metricName;
                    if (i < _modelsKeys.size() - 1) {(*streamASCII) << "\t";} else {(*streamASCII) << "\n";}
                }

            } else {
                fileASCII.reset(nullptr);
                PS_LOG->addErrorMessage(LogInterface::step, displayableCustomName() + tr(" : Impossible de créer le fichier d'export ASCII. Arrêt des traitements."));
                stop();
                return false;
            }
        } else {
            if (!fileASCII->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
            {
                fileASCII.reset(nullptr);
                PS_LOG->addErrorMessage(LogInterface::step, displayableCustomName() + tr(" : Impossible d'ouvrir le fichier d'export ASCII. Arrêt des traitements."));
                stop();
                return false;
            }

            streamASCII.reset(new QTextStream(fileASCII.data()));
        }
    }

    return true;
}


void PB_StepExportAttributesAsASCII::addToIndexedAttributesCollection(const CT_AbstractSingularItemDrawable* item, const CT_AbstractItemAttribute* attribute, QMap<QString, QPair<const CT_AbstractSingularItemDrawable*, const CT_AbstractItemAttribute*> >& indexedAttributes) const
{
    indexedAttributes.insert(computeKeyForModels(item->model(), attribute->model()), qMakePair(item, attribute));
}

void PB_StepExportAttributesAsASCII::replaceSpecialCharacters(QMap<QString, QString> &names) const
{
    QMutableMapIterator<QString, QString> it(names);
    while (it.hasNext())
    {
        it.next();
        it.setValue(PB_Tools::replaceSpecialCharacters(it.value()));
    }
}
