#ifndef CT_READER_TRAJECTORY_H
#define CT_READER_TRAJECTORY_H

#include "ct_reader/abstract/ct_abstractreader.h"

#include "ct_reader/extensions/ct_readerpointsfilteringextension.h"

#include "ctlibio/ctlibio_global.h"
#include "ct_reader_trajectory_def_model.h"

#include "Eigen/Core"


class CT_ColorCloudStdVector;
class CT_NormalCloudStdVector;

/**
 * @brief Reader that can load a ascii file (*.asc *.xyz *.csv etc...) that contains a point cloud and/or normals and/or colors and/or intensity
 */
class CTLIBIO_EXPORT CT_Reader_Trajectory : public CT_AbstractReader, public CT_ReaderPointsFilteringExtension
{
    Q_OBJECT
    typedef CT_AbstractReader SuperClass;

public:
    CT_Reader_Trajectory();

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

    void saveSettings(SettingsWriterInterface& writer) const override;
    bool restoreSettings(SettingsReaderInterface& reader) override;

    void setXColumnIndex(int index);
    void setYColumnIndex(int index);
    void setZColumnIndex(int index);

    void setGPSTimeColumnIndex(int index);

    void setFirstConfiguration(bool first);

    void setLinesToSkip(int skip);
    void setHasHeader(bool hasHeader);
    void setValueSeparator(QString sep);
    void setLocaleName(QString locale);

    int xColumnIndex() const;
    int yColumnIndex() const;
    int zColumnIndex() const;

    int gpsTimeColumnIndex() const;


    bool canLoadPoints() const;

    CT_AbstractReader* copy() const;
    READER_COPY_FULL_IMP(CT_Reader_Trajectory)

private:
    bool    m_firstConfiguration;
    int     m_columnXIndex;
    int     m_columnYIndex;
    int     m_columnZIndex;
    int     m_columnGPSTimeIndex;
    int     m_nLinesToSkip;
    bool    m_hasHeader;
    QString m_separator;
    QString m_localeName;

protected:
    void protectedInit();
    void protectedCreateOutItemDrawableModelList();
    bool protectedReadFile();

    int maxColumnIndex() const;

    void skipLines(QTextStream &stream, qint64 &currentSizeRead) const;

    bool readPoint(const QStringList &wordsOfLine, const QLocale &locale, Eigen::Vector3d &point, double &gpsTime) const;
};

#endif // CT_READER_TRAJECTORY_H
