#ifndef CT_READER_PLY_H
#define CT_READER_PLY_H

#include "ct_reader/abstract/ct_abstractreader.h"

#include "ctlibio/ctlibio_global.h"
#include "ct_reader_ply_def_models.h"

#include "ctlibio/readers/tools/ply/plyreadconfiguration.h"
#include "ctlibio/readers/tools/ply/iplyreaderlistener.h"

/**
 * @brief Reader that can load a ply file (*.ply) that represent a Mesh or a PointCloud
 */
class CTLIBIO_EXPORT CT_Reader_PLY : public CT_AbstractReader, public IPlyReaderListener
{
    Q_OBJECT
    typedef CT_AbstractReader SuperClass;

public:
    CT_Reader_PLY();

    /**
     * @brief Returns a displayable name of the reader
     */
    QString GetReaderName() const;

    /**
     * @brief Returns the sub menu level where we can store this reader
     */
    CT_StepsMenu::LevelPredefined getReaderSubMenuName() const;

    /**
     * @brief Show a dialog to configure this reader
     */
    bool configure();

    /**
     * @brief Set a configuration
     */
    bool setConfiguration(const PlyReadConfiguration& config);

    /**
     * @brief Returns the configuration
     */
    PlyReadConfiguration getConfiguration() const;

    /**
     * @brief Returns an empty file header (just to know the class type)
     */
    CT_FileHeader* createHeaderPrototype() const;

    void saveSettings(SettingsWriterInterface& writer) const override;
    bool restoreSettings(SettingsReaderInterface& reader) override;

    CT_AbstractReader* copy() const;
    READER_COPY_FULL_IMP(CT_Reader_PLY)

    /**
     * @brief This method must returns true if the read must be stopped
     */
    bool plyReadMustStop() const;

    /**
     * @brief This method is called when the progress changed
     */
    void plyProgressChanged(int progress);

private:
    PlyReadConfiguration    m_config;

protected:
    void protectedInit();
    void protectedCreateOutItemDrawableModelList();
    bool protectedReadFile();
    CT_FileHeader* protectedReadHeader(const QString &filepath, QString &error) const;
};

#endif // CT_READER_PLY_H
