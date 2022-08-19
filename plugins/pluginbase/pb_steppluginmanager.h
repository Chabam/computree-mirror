/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Alexandre P

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


#ifndef PB_STEPPLUGINMANAGER_H
#define PB_STEPPLUGINMANAGER_H

#include "ct_abstractstepplugin.h"
#include "ct_log/ct_fileloglistener.h"

#ifdef USE_GDAL
#include "gdal.h"
#endif

class CT_AbstractConfigurableElement;
class CT_AbstractReader;
class CT_AbstractExporter;

class PB_StepPluginManager : public CT_AbstractStepPlugin
{
public:
    PB_StepPluginManager();
    ~PB_StepPluginManager() override;

    QString getPluginURL() const override {return QString("http://rdinnovation.onf.fr/projects/plugin-base/wiki");}

    virtual QString getPluginOfficialName() const override {return "Base";}

    bool init() override;

    QString getPluginRISCitation() const override;

    /**
     * @brief Returns a collection that contains a copy of raster metrics available in all plugins
     */
    const QList<CT_AbstractConfigurableElement *>& rasterMetricsAvailable() const;

    /**
     * @brief Returns a collection that contains a copy of xyz metrics available in all plugins
     */
    const QList<CT_AbstractConfigurableElement *>& xyzMetricsAvailable() const;

    /**
     * @brief Returns a collection that contains a copy of xyz filters available in all plugins
     */
    const QList<CT_AbstractConfigurableElement *>& xyzFiltersAvailable() const;

    /**
     * @brief Returns a collection that contains a copy of readers available in all plugins
     */
    const QList<CT_AbstractReader *>& readersAvailable() const;

    /**
     * @brief Search in all available readers the reader with the specified classname and return it if founded, nullptr otherwise
     * @param readerClassName : the classname of the reader
     * @return Returns the reader with the specified classname if founded, nullptr otherwise
     */
    CT_AbstractReader* readerAvailableByUniqueName(const QString &readerClassName) const;

    /**
     * @brief Returns a collection that contains a copy of exporters available in all plugins. The key
     *        is constructed like this : QString("%2 - %1").arg(exporter->uniqueName()).arg(format.description())
     */
    const QMap<QString, CT_AbstractExporter*>& exportersAvailable() const;

    /**
     * @brief Returns a collection that contains a copy of exporters available in all plugins that can export points piece by piece. The key
     *        is constructed like this : QString("%2 - %1").arg(exporter->uniqueName()).arg(format.description())
     */
    const QMap<QString, CT_AbstractExporter*>& exportersForPointsWithPieceByPieceAvailable() const;
protected:

    QSettings* initQSettings() override;

    bool loadGenericsStep() override;
    bool loadOpenFileStep() override;
    bool loadCanBeAddedFirstStep() override;
    bool loadActions() override;
    bool loadExporters() override;
    bool loadReaders() override;
    bool loadFilters() override;
    bool loadMetrics() override;

    bool loadAfterAllPluginsLoaded() final;
    //void aboutToBeUnloaded();

private:
    //CT_FileLogListener  m_fileLog;
    CT_FileLogListener* _logListener;

    QList<CT_AbstractConfigurableElement *> m_rasterMetricsOfAllPlugins;
    QList<CT_AbstractConfigurableElement *> m_xyzMetricsOfAllPlugins;
    QList<CT_AbstractConfigurableElement *> m_xyzFiltersOfAllPlugins;
    QList<CT_AbstractReader*>               m_readersOfAllPlugins;
    QMap<QString, CT_AbstractExporter*>     m_exportersOfAllPlugins;
    QMap<QString, CT_AbstractExporter*>     m_exportersOfAllPluginsForPointsWithPieceByPieceAvailable;

    #ifdef USE_GDAL
    static void staticGdalErrorHandler(CPLErr eErrClass, int err_no, const char *msg);
    #endif

    /**
     * @brief Search in all plugins raster metrics available and add it to the collection
     */
    void initRasterMetricsCollection();

    /**
     * @brief Search in all plugins xyz metrics available and add it to the collection
     */
    void initXyzMetricsCollection();

    /**
     * @brief Search in all plugins xyz filters available and add it to the collection
     */
    void initXyzFiltersCollection();

    /**
     * @brief Search in all plugins readers available and add it to the collection
     */
    void initReadersCollection();

    /**
     * @brief Search in all plugins exporters available and add it to the collection
     */
    void initExportersCollection();
};

#endif // PB_STEPPLUGINMANAGER_H
