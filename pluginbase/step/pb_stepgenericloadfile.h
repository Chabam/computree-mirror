#ifndef PB_STEPGENERICLOADFILE_H
#define PB_STEPGENERICLOADFILE_H

#include "ct_step/abstract/ct_abstractsteploadfile.h"
#include "ct_reader/abstract/ct_abstractreader.h"

/**
 * @brief Step that can use any type of reader to load one file. You must create a step for each reader.
 */
class PB_StepGenericLoadFile : public CT_AbstractStepLoadFile
{
    Q_OBJECT
    using SuperClass = CT_AbstractStepLoadFile;

public:
    /**
     * @brief Construct a step that can use a reader to load one file
     * @param reader : the reader to use (the life in memory of the reader will be managed by this step)
     */
    PB_StepGenericLoadFile(CT_AbstractReader* reader);
    ~PB_StepGenericLoadFile();

    void init();

    /**
     * @brief Redefined to return the unique name of the reader
     */
    QString name() const final;

    /**
     * @brief Redefined to return the displayable name of the reader
     */
    QString displayableName() const final;

    /**
     * @brief Redefined to return the tooltip of the reader
     */
    QString description() const final;

    /**
     * @brief Redefined to return the tooltip of the reader or, if empty, type of files
     *        that can be loaded by the reader
     */
    QString detailledDescription() const final;

    /**
     * @brief Redefined to return readable format of the reader
     */
    QList<FileFormat> fileExtensionAccepted() const final;

    /**
     * @brief Redefined to verify that the file can be read by the reader, if not return false
     */
    bool setFilePath(const QString& newFilePath) final;

    void savePreSettings(SettingsWriterInterface& writer) const final;
    bool restorePreSettings(SettingsReaderInterface &reader) final;

    void savePostSettings(SettingsWriterInterface& writer) const final;
    bool restorePostSettings(SettingsReaderInterface &reader) final;

    CT_VirtualAbstractStep* createNewInstance() const final;

protected:

    bool preInputConfigure() final;

    void declareInputModels(CT_StepInModelStructureManager& manager) final;

    bool postInputConfigure() final;

    void declareOutputModels(CT_StepOutModelStructureManager& manager) final;

    void compute() final;

private:
    CT_HandleOutResultGroup                 m_hOutResult;
    CT_HandleOutStdGroup                    m_hOutRootGroup;
    CT_HandleOutSingularItem<CT_FileHeader> m_hOutFileHeader;

    CT_AbstractReader*                      m_reader;

private slots:
    void readerProgressChanged(int progress);
    void readerFilePathModified();
};

#endif // PB_STEPGENERICLOADFILE_H
