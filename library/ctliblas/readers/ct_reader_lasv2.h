#ifndef CT_READER_LASV2_H
#define CT_READER_LASV2_H

#include "ctliblas/ctliblas_global.h"

#include "ctliblas/readers/headers/ct_lasheader.h"

#include "ct_reader/abstract/ct_abstractreader.h"


#include "ct_itemdrawable/ct_scene.h"
#include "ct_itemdrawable/ct_pointsattributesscalartemplated.h"
#include "ct_itemdrawable/ct_pointsattributesscalarmaskt.h"
#include "ct_itemdrawable/ct_pointsattributescolor.h"
#include "ctliblas/itemdrawable/las/ct_stdlaspointsattributescontainer.h"


/**
 * @brief Read LAS File (http://www.asprs.org/Committee-General/LASer-LAS-File-Format-Exchange-Activities.html)
 */
class CTLIBLAS_EXPORT CT_Reader_LASV2 : public CT_AbstractReader
{
    Q_OBJECT
    typedef CT_AbstractReader SuperClass;

public:

    struct PointCore0_5
    { 
        quint8  entire; // Edge of Flight Line (1 bit) - Scan Direction Flag (1 bit) - Number of Returns (3 bits) - Return Number (3 bits)

        typedef quint8 MASK;
    };

    struct PointCore6_10
    {
        quint16 entire; // Number of Returns (4 bits) - Return Number (4 bits)
                        // Edge of Flight Line (1 bit) - Scan Direction Flag (1 bit) - Scanner Channel (2 bits) - Classification Flags (4 bits)

        typedef quint16 MASK;
    };

    CT_Reader_LASV2(int subMenuLevel = 0);
    CT_Reader_LASV2(const CT_Reader_LASV2 &other);
    virtual ~CT_Reader_LASV2();

    QString displayableName() const override;

    bool setFilePath(const QString& filepath) override;

    bool configure();
    CT_FileHeader* createHeaderPrototype() const;

    virtual bool hasBoundingBox() {return true;}

    bool restoreSettings(SettingsReaderInterface &reader) override;

    READER_ALL_COPY_IMP(CT_Reader_LASV2)

protected:

    void internalDeclareOutputModels(CT_ReaderOutModelStructureManager& manager) override;

    CT_FileHeader* internalReadHeader(const QString &filepath, QString &error) const override;

    bool internalReadFile(CT_StandardItemGroup* group) override;

private:
    CT_LASHeader *m_headerFromConfiguration;

    CT_HandleOutSingularItem<CT_Scene>                                          m_hOutScene;
    CT_HandleOutSingularItem<CT_StdLASPointsAttributesContainer>                m_hOutAllAtt;

    CT_HandleOutSingularItem<CT_PointsAttributesScalarMaskT<PointCore6_10> >    m_hOutReturnNumber;
    CT_HandleOutSingularItem<CT_PointsAttributesScalarMaskT<PointCore6_10> >    m_hOutNumberOfReturn;
    CT_HandleOutSingularItem<CT_PointsAttributesScalarMaskT<PointCore6_10> >    m_hOutClassificationFlag;
    CT_HandleOutSingularItem<CT_PointsAttributesScalarMaskT<PointCore6_10> >    m_hOutScannerChannel;
    CT_HandleOutSingularItem<CT_PointsAttributesScalarMaskT<PointCore6_10> >    m_hOutScanDirectionFlag;
    CT_HandleOutSingularItem<CT_PointsAttributesScalarMaskT<PointCore6_10> >    m_hOutEdgeOfFlightLine;

    CT_HandleOutSingularItem<CT_PointsAttributesScalarTemplated<quint16> >      m_hOutIntensity;
    CT_HandleOutSingularItem<CT_PointsAttributesScalarTemplated<quint8> >       m_hOutClassification;
    CT_HandleOutSingularItem<CT_PointsAttributesScalarTemplated<quint8> >       m_hOutUserData;
    CT_HandleOutSingularItem<CT_PointsAttributesScalarTemplated<quint16> >      m_hOutPointSourceID;

    CT_HandleOutSingularItem<CT_PointsAttributesScalarTemplated<qint16> >       m_hOutScanAngle;

    CT_HandleOutSingularItem<CT_PointsAttributesScalarTemplated<double> >       m_hOutGPSTime;

    CT_HandleOutSingularItem<CT_PointsAttributesColor>                          m_hOutColor;
    CT_HandleOutSingularItem<CT_PointsAttributesScalarTemplated<quint16> >      m_hOutRed;
    CT_HandleOutSingularItem<CT_PointsAttributesScalarTemplated<quint16> >      m_hOutGreen;
    CT_HandleOutSingularItem<CT_PointsAttributesScalarTemplated<quint16> >      m_hOutBlue;

    CT_HandleOutSingularItem<CT_PointsAttributesScalarTemplated<quint8> >       m_hOutWavePacketDescriptorIndex;
    CT_HandleOutSingularItem<CT_PointsAttributesScalarTemplated<quint64> >      m_hOutByteOffsetToWaveformData;
    CT_HandleOutSingularItem<CT_PointsAttributesScalarTemplated<quint32> >      m_hOutWaveformPacketSizeInBytes;
    CT_HandleOutSingularItem<CT_PointsAttributesScalarTemplated<float> >        m_hOutReturnPointWaveformLocation;

    CT_HandleOutSingularItem<CT_PointsAttributesScalarTemplated<quint16>>       m_hOutNIR;

};

CT_TYPEINFO_MACRO(CT_Reader_LASV2::PointCore0_5, CT_Reader_LASV2::PointCore0_5)
CT_TYPEINFO_MACRO(CT_Reader_LASV2::PointCore6_10, CT_Reader_LASV2::PointCore6_10)

#endif // CT_READER_LASV2_H
