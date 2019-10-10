#ifndef CT_ABSTRACTEXPORTER_H
#define CT_ABSTRACTEXPORTER_H

#include "ctlibexporter_global.h"

#include "ct_view/tools/fileformat.h"
#include "settingsinterfaces.h"

#include "ct_model/inModel/manager/ct_inmodelstructuremanager.h"
#include "ct_model/outModel/manager/ct_outmodelstructuremanager.h"

#include "ct_exporter/tools/ct_exporterinmodelstructuremanager.h"

#include "ct_handle/ct_handleinresultgroup.h"
#include "ct_handle/ct_handleinstdzeroormoregroup.h"
#include "ct_handle/ct_handleinstdgroup.h"
#include "ct_handle/ct_handleinsingularitem.h"
#include "ct_handle/ct_handleinstditemattribute.h"
#include "ct_handle/ct_handleinstditemattributet.h"

#include "ct_itemdrawable/ct_loopcounter.h"

class CT_AbstractCloudIndex;

#define CT_EXPORTER_DECL_COPY(CLASSNAME) \
CT_AbstractExporter* createInstance() const override { return new CLASSNAME(); } \
CT_AbstractExporter* copy() const override { return new CLASSNAME(*this); }

/**
 * @brief An exporter can export in multiple ways :
 *        First way : All items or groups selected are in the same file
 *        Second way : Items or groups selected are separated in multiple file (the user must choose which attribute contains the name of files)
 *        Third way : Some exporter can export piece by piece. That mean that you can export some elements, next you can choose
 *        other element an export them, etc... an finally merge all elements in the same file
 */
class CTLIBEXPORTER_EXPORT CT_AbstractExporter : public QObject
{
    Q_OBJECT

public:
    CT_AbstractExporter(int subMenuLevel = 0);

    /**
     * @brief Copy constructor. You must redefine this method to allow
     *        others developper to do a full copy of your exporter !
     * @warning Don't forget to add the macro Q_OBJECT in your class.
     */
    CT_AbstractExporter(const CT_AbstractExporter& other);
    ~CT_AbstractExporter();

    /**
     * @brief Returns the displayable name of the exporter. By default return the result of method "uniqueName()"
     */
    virtual QString displayableName() const;

    /**
     * @brief Returns the unique name (you can use it to compare exporters because it was unique). By default
     *        return the result of "metaObject()->className()"
     */
    virtual QString uniqueName() const;

    /**
     * @brief Set the sub menu level where we can store this reader
     */
    void setSubMenuLevel(int level);

    /**
     * @brief Returns the sub menu level where we can store this reader
     */
    int subMenuLevel() const;

    /**
     * @brief Set the export filepath. You can overload this method if you want to verify something in the filepath.
     *        return false if you don't accept the filepath.
     */
    virtual bool setFilePath(const QString &filepath);

    /**
     * @brief Set the prefix of the filename.
     */
    void setFileNamePrefix(const QString& fileNamePrefix);

    /**
     * @brief export filepath getter
     */
    QString filePath() const;

    /**
     * @brief Configure your exporter. Returns false if configuration is canceled.
     */
    virtual bool configure() { return true; }

    /**
     * @brief This method is called when the core will save settings of this object
     *        You can redefine it in your class to save your settings
     */
    virtual void saveSettings(SettingsWriterInterface& writer) const;

    /**
     * @brief This method is called when the core will restore settings of this object
     *        You can redefine it in your class to restore your settings
     * @returns Returns true if all settings was perfectly restored, false if a setting cannot
     *          be restored and this object must be configured manually.
     */
    virtual bool restoreSettings(SettingsReaderInterface& reader);

    /**
     * @brief Call it to declare all input models in a specified manager. If you call this method you
     *        will not be able to call method "wereAllInputsFoundInOutputsOfThisManager" and
     *        "findInputsInOutputsOfThisManager" because you use your own manager. But this method
     *        must be used in a step that use an exporter !
     * @param manager : the manager to use to declare input models
     * @param nameOfTheFileContainsTurnNameFromACounter : If false the name of the file will be the name set in the method "setFilePath".
     *                                                    If true only the directory of the filepath set in method "setFilePath" will be
     *                                                    used and the filename will be the turn name returned by a CT_LoopCounter.
     * @param exportEachItemInSeperateFile : If false all items or groups will be exported in the same file.
     *                                       If true each item or each group (if the exporter export only groups) will be exported in a seperate file. Only the directory
     *                                       of the filepath set in method "setFilePath" will be used. The name of the file will be a value of an attribute of the item (if
     *                                       the parameter "nameOfTheFileContainsTurnNameFromACounter" is set to true the file will contains the turn name returned by a CT_LoopCounter too.
     * @param fileNameUsedFromAttributeOfAnotherItem : If false the filename depends on previous parameter.
     *                                                   If true (can only be true if "exportEachItemInSeperateFile" is true otherwise it will be ignored) the name of the file
     *                                                   will be a value of an attribute of another item it the same group as as the item to export (if the parameter "nameOfTheFileContainsTurnNameFromACounter"
     *                                                   is set to true the file will contains the turn name returned by a CT_LoopCounter too.
     */
    void declareInputModels(CT_InModelStructureManager& manager,
                            bool nameOfTheFileContainsTurnNameFromACounter = false,
                            bool exportEachItemInSeperateFile = false,
                            bool fileNameUsedFromAttributeOfAnotherItem = false);

    /**
     * @brief Call it to declare all input models in manager of this class. If you call this method you
     *        will be able to call method "wereAllInputsFoundInOutputsOfThisManager" and
     *        "findInputsInOutputsOfThisManager". This method
     *        must be used when the exporter is used as a standalone (not in a step but in gui per example) !
     */
    void declareInputModels();

    /**
     * @brief If you have called the method "declareInputModels()" (without argruments) you can get the
     *        inputs model manager used to declare inputs.
     */
    const CT_ExporterInModelStructureManager* inputsModelManager() const;

    /**
     * @brief Returns true if it was at least one model in the model manager of this class
     * @warning You can only call this method if you have called the method "declareInputModels()".
     */
    bool needInputs() const;

    /**
     * @brief Find necessary input in output declared in specified output manager. All possibility of input models
     *        will not be modified when you call this method because a copy is made before
     *        do the process.
     * @param manager : the manager that has output models
     * @warning You can only call this method if you have called the method "declareInputModels()".
     * @return false if all input models doesn't find necessary
     *         input. Otherwise if all input has found what they want this method returns true.
     */
    bool wereAllInputsFoundInOutputsOfThisManager(const CT_OutModelStructureManager& manager) const;

    /**
     * @brief Find necessary input in output declared in specified output manager. All possibility of input models
     *        will be modified when you call this method !
     * @param manager : the manager that has output models
     * @warning You can only call this method if you have called the method "declareInputModels()".
     * @return false if all input models doesn't find necessary
     *         input. Otherwise if input has found what they want this method returns true and
     *         possibilities can be used to see which out models has been saved.
     */
    bool findInputsInOutputsOfThisManager(const CT_OutModelStructureManager& manager);

    /**
     * @brief Call it to display configuration dialog to let the user choose inputs/outputs link
     * @return false if user cancel, true otherwise
     */
    bool showInputsConfigurationDialog();

    /**
      * \brief Return available export formats (per example : CSV or XML or DXF, etc...)
      *
      *        Use the method addNewExportFormat() in the init() method to add new format.
      */
    const QList<FileFormat>& exportFormats() const;

    /**
     * @brief Returns true if the extension is present in readable formats
     * @param extension : must be the extension of the file WITHOUT the comma "."
     */
    bool isExtensionPresentInReadableFormat(const QString& extension) const;

    /**
     * @brief Return true if it was an export error
     */
    bool isExportError() const;

    /**
     * @brief Returns true if the process must be stopped or was stopped
     */
    bool isStopped() const;

    /**
     * @brief Returns a tooltip to show in GUI. None by default.
     */
    QString toolTip() const;

    /**
     * @brief Return the error message. You can set them by a call to the method setErrorMessage()
     */
    QString errorMessage() const;

    /**
     * @brief Clear the error message
     */
    void clearErrorMessage();

    /**
     * @brief Inherit this method if you want to check something. CloudIndex will be backup if the method "canExportPoints()" returns true
     * @warning Always call method of class parent !
     */
    virtual bool setPointsToExport(const QList<CT_AbstractCloudIndex*> &list);

    /**
     * @brief Inherit this method if you want to check something. CloudIndex will be backup if the method "canExportFaces()" returns true
     * @warning Always call method of class parent !
     */
    virtual bool setFacesToExport(const QList<CT_AbstractCloudIndex*> &list);

    /**
     * @brief Inherit this method if you want to check something. CloudIndex will be backup if the method "canExportEdges()" returns true
     * @warning Always call method of class parent !
     */
    virtual bool setEdgesToExport(const QList<CT_AbstractCloudIndex*> &list);

    /**
     * @brief return true if the exporter can export points (false by default)
     */
    bool canExportPoints() const;

    /**
     * @brief return true if the exporter can export edges (false by default)
     */
    bool canExportEdges() const;

    /**
     * @brief return true if the exporter can export faces (false by default)
     */
    bool canExportFaces() const;

    /**
     * @brief Return true if the exporter has something to export
     */
    bool hasSomethingToExport() const;

    /**
      * \brief Retourne la liste des points a exporter
      */
    const QList<CT_AbstractCloudIndex*>& pointsToExport() const;

    /**
      * \brief Retourne la liste des faces a exporter
      */
    const QList<CT_AbstractCloudIndex*>& facesToExport() const;

    /**
      * \brief Retourne la liste des edges a exporter
      */
    const QList<CT_AbstractCloudIndex*>& edgesToExport() const;

    /**
     * @brief return true if the exporter can export data piece by piece
     *
     * If it is :
     * 1) First method createExportFileForPieceByPieceExport() is called to create an empty file
     * 2) exportOnePieceOfDataToFile() method is called multiple times (for example in a loop)
     * 3) A the end, finalizePieceByPieceExport() method is called to correctly finish the export (for example by updating the header)
     *
     * Return false by default
     */
    virtual bool canExportPieceByPiece() const;

    /**
     * @brief Create the file, prepared for piece by piece export
     *
     * @return true is creation was a success
     */
    virtual bool createExportFileForPieceByPieceExport();

    /**
     * @brief Export one piece of data to the file
     *
     * @return true is the export was a success
     */
    virtual bool exportOnePieceOfDataToFile();

    /**
     * @brief Finalize the piece by piece export
     *
     * This method is called to correctly finish the export (for example by updating the header)
     *
     * @return true is the finalization process was a success
     */
    virtual bool finalizePieceByPieceExport();

    /**
     * @brief Calle it to export to the file (not piece by piece !)
     */
    bool exportToFile();

    /**
     * @brief Return a new instance of the exporter
     */
    virtual CT_AbstractExporter* createInstance() const = 0;

    /**
     * @brief Return a full copy of the exporter. You must redefine the copy constructor
     *        and add the macro Q_OBJECT in your class to be sure that this function will
     *        be correct.
     */
    virtual CT_AbstractExporter* copy() const = 0;

protected:
    /**
      * \brief Ajouter un nouveau format d'exportation.
      */
    void addNewExportFormat(const FileFormat &format);

    /**
     * @brief Set the tooltip to show in gui
     */
    void setToolTip(const QString &t);

    /**
     * @brief Redefine it to declare all input models.
     */
    virtual void internalDeclareInputModels(CT_ExporterInModelStructureManager& manager) = 0;

    /**
      * \brief Défini la progression et emet un signal lorsque celle-ci change
      */
    void setExportProgress(int progress);

    /**
      * \brief Défini le message d'erreur
      */
    void setErrorMessage(const QString &err);

    /**
     * @brief Define if you can export points
     */
    void setCanExportPoints(bool e);

    /**
     * @brief Define if you can export edges
     */
    void setCanExportEdges(bool e);

    /**
     * @brief Define if you can export faces
     */
    void setCanExportFaces(bool e);

    /**
      * \brief A redéfinir dans votre exporter. Export dans le fichier les éléments sélectionnés lors de la configuration.
      */
    virtual bool internalExportToFile() = 0;

    void setMustUseModels(bool useModels);

    bool mustUseModels() const;

    virtual void clearIterators() = 0;

    /**
     * @brief You must call this method when you export items in file to know how many
     *        item must be exported in the current file
     * @param nToExport : exporter must precise how many he want to export
     * @return N items maximum to export in the current file or "nToExport" if no limit.
     */
    int maximumItemToExportInFile(const int nToExport) const;

private:
    CT_InModelStructureManager                                  mInModelSManager;
    CT_ExporterInModelStructureManager*                         m_inModelsStructureManager;

    CT_HandleInResultGroup<>                                    m_handleResultCounter;
    CT_HandleInStdGroup<>                                       m_handleGroupCounter;
    CT_HandleInSingularItem<CT_LoopCounter>                     m_handleLoopCounter;

protected:
    CT_HandleInResultGroup<>                                    m_handleResultExport;

private:
    CT_HandleInStdZeroOrMoreGroup                               m_handleZeroOrMoreGroupExport;
    CT_HandleInSingularItem<CT_AbstractSingularItemDrawable>    m_handleItemWithNameExport;
    CT_HandleInStdItemAttributeT<QString>                       m_handleItemAttributeExport;


    bool                                mMustUseModels;
    bool                                mNameOfTheFileContainsTurnNameFromACounter;
    bool                                mExportEachItemInSeparateFile;
    bool                                mFileNameUsedFromAttributeOfAnotherItem;

    bool                                mMaximumItemToExportCalled;

    QList<FileFormat>                   _formats;
    QString                             _errMsg;
    QList<CT_AbstractCloudIndex*>       _points;
    QList<CT_AbstractCloudIndex*>       _faces;
    QList<CT_AbstractCloudIndex*>       _edges;
    QString                             _filepath;
    QString                             mFileNamePrefix;
    int                                 _progress;
    bool                                _ePoints;
    bool                                _eFaces;
    bool                                _eEdges;
    bool                                m_stop;
    QString                             m_tooltip;
    int                                 m_subMenuLevel;

    /**
     * @brief Find necessary input in output declared in specified output manager. All possibility of input models
     *        will be modified when you call this method !
     * @param manager : the manager that has output models
     * @param begin : the begin iterator of a collection that contains pointer to CT_InAbstractResultModel
     * @param end : the end iterator of a collection that contains pointer to CT_InAbstractResultModel
     * @return false if the step is nullptr and this step need inputs OR if all models doesn't find necessary
     *         input. Otherwise if input has found what they want this method returns true and
     *         possibilities can be used to see which out models has been saved.
     */
    bool findInputsInOutputsOfThisManagerWithSpecifiedResultModels(const CT_OutModelStructureManager& manager,
                                                                   QVector<CT_InAbstractResultModel*>::iterator begin,
                                                                   QVector<CT_InAbstractResultModel*>::iterator end) const;
public slots:

    /**
     * @brief Call to cancel the process
     */
    void cancel();

signals:

    /**
     * @brief signal to know the progress of export
     */
    void exportInProgress(int progress);
};

#endif // CT_ABSTRACTEXPORTER_H
