#include "pb_csvexporter.h"

#include "pb_csvexporterconfiguration.h"
#include "views/exporters/csv/pbg_csvconfigurationdialog.h"
#include "pb_csvexportercolumn.h"

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"

#include <QFile>
#include <QTextStream>
#include <QVector>
#include <limits>
#include <QFileInfo>

PB_CSVExporter::PB_CSVExporter() : SuperClass()
{
    _configuration = NULL;
    _mapKeyChanged = true;
}

PB_CSVExporter::PB_CSVExporter(const PB_CSVExporter &other) : SuperClass(other)
{
    _mapKeyChanged = other._mapKeyChanged;
    _mapItemToExport = other._mapItemToExport;

    _configuration = ((other._configuration == NULL) ? NULL : new PB_CSVExporterConfiguration(*other._configuration));

    QMapIterator<CT_OutAbstractSingularItemModel*, QList<CT_AbstractItemDrawable*>* > it(other._mapItemToExport);

    while(it.hasNext()) {
        it.next();
        _mapItemToExport.insert(it.key(), new QList<CT_AbstractItemDrawable*>(*it.value()));
    }
}

PB_CSVExporter::~PB_CSVExporter()
{
    delete _configuration;
    clearMap();
}

QString PB_CSVExporter::getExporterCustomName() const
{
    return tr("Export d'attributs (csv)");
}

CT_StepsMenu::LevelPredefined PB_CSVExporter::getExporterSubMenuName() const
{
    return CT_StepsMenu::LP_Items;
}

void PB_CSVExporter::init()
{
    addNewExportFormat(FileFormat("csv", tr("Fichiers csv")));

    setToolTip(tr("Exporte tous les attributs d'un niveau d'items (une ligne pour chaque instance d'item)"));
}

bool PB_CSVExporter::setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list)
{
    clearErrorMessage();

    QList<CT_AbstractItemDrawable*> myList;

    for(CT_AbstractItemDrawable* item : list) {
        if(dynamic_cast<CT_AbstractSingularItemDrawable*>(item) != NULL)
            myList.append(item);
    }

    if(myList.isEmpty())
    {
        setErrorMessage(tr("Aucun Singular Item"));
        return false;
    }

    if(SuperClass::setItemDrawableToExport(myList))
    {
        QMap<CT_OutAbstractSingularItemModel*, QList<CT_AbstractItemDrawable*>* > newMap;

        for(CT_AbstractItemDrawable* item : list) {
            CT_AbstractSingularItemDrawable *sItem = static_cast<CT_AbstractSingularItemDrawable*>(item);
            CT_OutAbstractSingularItemModel* sModel = static_cast<CT_OutAbstractSingularItemModel*>(sItem->model());

            QList<CT_AbstractItemDrawable*>* list = newMap.value(sModel, NULL);

            if(list == NULL)
            {
                list = new QList<CT_AbstractItemDrawable*>();
                newMap.insert(sModel, list);
            }

            list->append(item);
        }

        _mapKeyChanged = true;

        QMapIterator<CT_OutAbstractSingularItemModel*, QList<CT_AbstractItemDrawable*>* > itMap(_mapItemToExport);

        if(itMap.hasNext())
        {
            _mapKeyChanged = false;

            while(itMap.hasNext()
                  && !_mapKeyChanged)
            {
                if(!newMap.contains(itMap.next().key()))
                    _mapKeyChanged = true;
            }
        }

        _mapItemToExport = newMap;

        configureExportWithLastConfigurationAndNewItemToExport();

        return !_mapItemToExport.isEmpty();
    }

    clearMap();
    _mapKeyChanged = true;

    return false;
}

bool PB_CSVExporter::configureExport()
{
    QList<CT_OutAbstractSingularItemModel*> list = _mapItemToExport.keys();

    if(_mapKeyChanged
            || (_configuration == NULL))
    {
        delete _configuration;
        _configuration = new PB_CSVExporterConfiguration(list);
    }
    else
    {
        _configuration->setList(list);
    }

    PBG_CSVConfigurationDialog dialog(*_configuration);

    bool ret = (dialog.exec() == QDialog::Accepted) && (!_configuration->getColumns().isEmpty());

    _mapKeyChanged = false;

    return ret;
}

void PB_CSVExporter::saveSettings(SettingsWriterInterface &writer) const
{
    SuperClass::saveSettings(writer);

    if(_configuration == NULL)
        return;

    const QList<QPair<CT_OutAbstractSingularItemModel *, CT_OutAbstractItemAttributeModel *> >& columns = _configuration->getColumns();

    int i = 0;
    for(const QPair<CT_OutAbstractSingularItemModel *, CT_OutAbstractItemAttributeModel *>& column : columns) {

        const int id = writer.addParameter(this, "Column", i);
        writer.addParameterInfo(this, id, "ItemModelUniqueName", column.first->uniqueName());
        writer.addParameterInfo(this, id, "ItemAttributeModelUniqueName", column.second->uniqueName());

        ++i;
    }
}

bool PB_CSVExporter::restoreSettings(SettingsReaderInterface &reader)
{
    if(!SuperClass::restoreSettings(reader))
        return false;

    QList<CT_OutAbstractSingularItemModel*> list = _mapItemToExport.keys();

    delete _configuration;
    _configuration = new PB_CSVExporterConfiguration(list);

    const int nColumns = reader.parameterCount(this, "Column");

    QVariant value;
    for(int i=0; i<nColumns; ++i) {
        const int id = reader.parameter(this, "Column", value);

        if((id <= 0) || (value.toInt() != i))
            return false;

        if(!reader.parameterInfo(this, id, "ItemModelUniqueName", value))
            return false;

        const CT_OutAbstractSingularItemModel* refList = getItemModelByName(value.toString());

        if(refList == NULL)
            return false;

        if(!reader.parameterInfo(this, id, "ItemAttributeModelUniqueName", value))
            return false;

        const CT_OutAbstractItemAttributeModel* ref = getItemAttributeModelByName(refList, value.toString());

        if(ref == NULL)
            return false;

        _configuration->addColumn(refList, ref);
    }

    return true;
}

bool PB_CSVExporter::protectedExportToFile()
{
    if(!_mapKeyChanged)
    {
        QFileInfo exportPathInfo = QFileInfo(exportFilePath());
        QString path = exportPathInfo.path();
        QString baseName = exportPathInfo.baseName();
        QString suffix = "csv";
        QString filePath = QString("%1/%2.%4").arg(path).arg(baseName).arg(suffix);

        QFile file(filePath);

        if(file.open(QFile::WriteOnly))
        {
            QTextStream stream(&file);

            QList<PB_CSVExporterColumn*> columns;

            // ecriture de l'header
            QListIterator< QPair<CT_OutAbstractSingularItemModel *, CT_OutAbstractItemAttributeModel *> > it(_configuration->getColumns());

            while(it.hasNext())
            {
                const QPair<CT_OutAbstractSingularItemModel *, CT_OutAbstractItemAttributeModel *> &column = it.next();

                QString dispName = column.second->displayableName() .remove(" ")
                                                                    .remove('\'')
                                                                    .remove('\"')
                                                                    .remove('\t')
                                                                    .remove(';')
                                                                    .remove(':')
                                                                    .remove(',')
                                                                    .remove('.');
                stream << dispName;

                if (it.hasNext()) {stream << ";";}

                QList<CT_AbstractItemDrawable*> *lToConvert = _mapItemToExport.value(column.first);
                QList<CT_AbstractSingularItemDrawable*> lToPass;

                foreach (CT_AbstractItemDrawable *itemD, *lToConvert) {
                    lToPass.append((CT_AbstractSingularItemDrawable*)itemD);
                }

                columns.append(new PB_CSVExporterColumn(lToPass, column.second));
            }

            // ecriture des données
            int currentItemIndex = 0;
            int lastCurrentItemIndex;
            bool hasAtLeastOneNextValue;

            size_t completeSize = 0;
            size_t currentPos = 0;

            QListIterator<PB_CSVExporterColumn*> itC(columns);

            while(itC.hasNext())
                completeSize += itC.next()->size();

            do
            {
                stream << "\r\n";

                itC.toFront();
                lastCurrentItemIndex = std::numeric_limits<int>::max();
                hasAtLeastOneNextValue = false;

                // pour chaque colonne
                while(itC.hasNext())
                {
                    PB_CSVExporterColumn *column = itC.next();

                    // si cette colonne va exporter une valeur
                    // d'un item dont l'index dans la liste est le même
                    // que les autres colonnes
                    if((column->currentItemIndex() == currentItemIndex)
                            && column->hasNextValue())
                    {
                        // on exporte la valeur
                        stream << column->nextValueToString();

                        if (itC.hasNext()) {stream << ";";}

                        // on garde en mémoire l'index de l'item
                        if(column->currentItemIndex() < lastCurrentItemIndex)
                            lastCurrentItemIndex = column->currentItemIndex();

                        // si il y aura encore une valeur a exporter
                        if(column->hasNextValue()
                                && !hasAtLeastOneNextValue)
                            hasAtLeastOneNextValue = true;


                        ++currentPos;
                        setExportProgress(int((currentPos*95)/completeSize));
                    }
                    else
                    {
                        if (itC.hasNext()) {stream << ";";}
                    }
                }

                currentItemIndex = lastCurrentItemIndex;

            // tant qu'il reste au moins une valeur à exporter
            }while(hasAtLeastOneNextValue);

            qDeleteAll(columns.begin(), columns.end());

            file.close();
            setExportProgress(100);

            return true;
        }
    }

    return false;
}

bool PB_CSVExporter::configureExportWithLastConfigurationAndNewItemToExport()
{
    if(_mapKeyChanged && (_configuration != NULL))
    {
        QList<CT_OutAbstractSingularItemModel*> list = _mapItemToExport.keys();

        QList<QPair<CT_OutAbstractSingularItemModel *, CT_OutAbstractItemAttributeModel *> > columns = _configuration->getColumns();
        QList<QPair<CT_OutAbstractSingularItemModel *, CT_OutAbstractItemAttributeModel *> > newColumns = columns;
        QVector<bool> replaced(columns.size());

        QListIterator<CT_OutAbstractSingularItemModel*> it(list);

        while(it.hasNext())
        {
            CT_OutAbstractSingularItemModel *model = it.next();
            QListIterator<CT_OutAbstractItemAttributeModel*> itA(model->itemAttributes());

            int index = 0;

            QListIterator<QPair<CT_OutAbstractSingularItemModel *, CT_OutAbstractItemAttributeModel *> > itC(columns);

            while(itC.hasNext())
            {
                const QPair<CT_OutAbstractSingularItemModel *, CT_OutAbstractItemAttributeModel *> &column = itC.next();

                // if same original model
                if(column.first->originalModel() == model->originalModel())
                {
                    itA.toFront();

                    bool ok = false;
                    while(itA.hasNext()
                          && !ok)
                    {
                        CT_OutAbstractItemAttributeModel *attModel = itA.next();

                        // if same original model
                        if(column.second->originalModel() == attModel->originalModel())
                        {
                            // we can replace this pair by this model
                            replaced.replace(index, true);

                            newColumns.replace(index, qMakePair(model, attModel));

                            ok = true;
                        }
                    }
                }

                ++index;
            }
        }

        QVectorIterator<bool> itV(replaced);

        while(itV.hasNext())
        {
            if(itV.next() == false)
                return false;
        }

        _configuration->clearColumns();

        QListIterator<QPair<CT_OutAbstractSingularItemModel *, CT_OutAbstractItemAttributeModel *> > itC(newColumns);

        while(itC.hasNext())
        {
            const QPair<CT_OutAbstractSingularItemModel *, CT_OutAbstractItemAttributeModel *> &column = itC.next();

            _configuration->addColumn(column.first, column.second);
        }

        _mapKeyChanged = false;
    }

    return !_mapKeyChanged;
}

void PB_CSVExporter::clearMap()
{
    qDeleteAll(_mapItemToExport.begin(), _mapItemToExport.end());
    _mapItemToExport.clear();
}

CT_OutAbstractSingularItemModel* PB_CSVExporter::getItemModelByName(const QString &name) const
{
    QMapIterator<CT_OutAbstractSingularItemModel*, QList<CT_AbstractItemDrawable*>* > it(_mapItemToExport);

    while(it.hasNext())
    {
        it.next();

        if(it.key()->uniqueName() == name)
            return it.key();
    }

    return NULL;
}

CT_OutAbstractItemAttributeModel* PB_CSVExporter::getItemAttributeModelByName(const CT_OutAbstractSingularItemModel *sItem, const QString &name) const
{
    QListIterator<CT_OutAbstractItemAttributeModel*> it(sItem->itemAttributes());

    while(it.hasNext())
    {
        CT_OutAbstractItemAttributeModel *ref = it.next();

        if(ref->uniqueName() == name)
            return ref;
    }

    return NULL;
}
