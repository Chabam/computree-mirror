#include "ct_reader_ply.h"

#include <QDialog>

#include "readers/headers/ct_plyheader.h"
#include "readers/tools/ply/plyheaderreader.h"
#include "readers/tools/ply/plyfilepartreader.h"
#include "readers/tools/ply/plycomputreewrappers.h"

#include "ct_itemdrawable/ct_scene.h"
#include "ct_itemdrawable/ct_pointsattributescolor.h"
#include "ct_itemdrawable/ct_pointsattributesnormal.h"
#include "ct_itemdrawable/ct_pointsattributesscalartemplated.h"

#include "ct_colorcloud/ct_colorcloudstdvector.h"
#include "ct_normalcloud/ct_normalcloudstdvector.h"
#include "ct_cloud/ct_standardcloudstdvectort.h"

#include "ct_view/tools/ct_configurablewidgettodialog.h"

#define DEF_Scene "sce"
#define DEF_Color "col"
#define DEF_Normal "nor"
#define DEF_Scalar "sca"

#define readConfiguration(SNG, ELNAME, VAR) snv = SNG->firstValueByTagName(ELNAME); \
                                            if(snv == NULL) \
                                                return false; \
                                            VAR = snv->value().toInt(&ok); \
                                            if(!ok) \
                                                return false;

#define readNewConfiguration(ELNAME, VAR)   if(!reader.parameter(this, ELNAME, value)) \
                                                return false; \
                                            VAR = value.toInt(&ok); \
                                            if(!ok) \
                                                return false;

#define readNewConfigurationInfo(ELNAME, VAR)   if(!reader.parameterInfo(this, pID, ELNAME, value)) \
                                                    return false; \
                                                VAR = value.toInt(&ok); \
                                                if(!ok) \
                                                    return false;

CT_Reader_PLY::CT_Reader_PLY() : CT_AbstractReader()
{
}

QString CT_Reader_PLY::GetReaderName() const
{
    return tr("Points, Fichier PLY");
}

CT_StepsMenu::LevelPredefined CT_Reader_PLY::getReaderSubMenuName() const
{
    return CT_StepsMenu::LP_Points;
}

bool CT_Reader_PLY::configure()
{
    PlyHeaderReader headerReader;
    headerReader.setFilePath(filepath());

    if(!headerReader.process()) {
        PS_LOG->addErrorMessage(LogInterface::reader, tr("L'en-tête du fichier \"%1\" n'est pas formé correctement.").arg(filepath()));
        return false;
    }

    GPly_CT_ReadConfiguration_Wrapper config;
    config.plyConfigurationWidget()->setHeader(headerReader.getHeader());

    if(m_config.vertex.elementIndex != -1)
        config.plyConfigurationWidget()->setConfiguration(m_config);

    if (CT_ConfigurableWidgetToDialog::exec(&config) == QDialog::Accepted)
        return setConfiguration(config.plyConfigurationWidget()->getConfiguration());

    return false;
}

bool CT_Reader_PLY::setConfiguration(const PlyReadConfiguration &config)
{
    m_config = config;
    return true;
}

PlyReadConfiguration CT_Reader_PLY::getConfiguration() const
{
    return m_config;
}

CT_FileHeader* CT_Reader_PLY::createHeaderPrototype() const
{
    return new CT_PLYHeader();
}

void CT_Reader_PLY::saveSettings(SettingsWriterInterface &writer) const
{
    SuperClass::saveSettings(writer);

    writer.addParameter(this, "elementIndex", m_config.vertex.elementIndex);
    writer.addParameter(this, "xPropertyIndex", m_config.vertex.xPropertyIndex);
    writer.addParameter(this, "yPropertyIndex", m_config.vertex.yPropertyIndex);
    writer.addParameter(this, "zPropertyIndex", m_config.vertex.zPropertyIndex);

    foreach (const PlyColorConfiguration& cc, m_config.colors) {
        const int pID = writer.addParameter(this, "Color", true);

        writer.addParameterInfo(this, pID, "elementIndex", cc.elementIndex);
        writer.addParameterInfo(this, pID, "rPropertyIndex", cc.rPropertyIndex);
        writer.addParameterInfo(this, pID, "gPropertyIndex", cc.gPropertyIndex);
        writer.addParameterInfo(this, pID, "bPropertyIndex", cc.bPropertyIndex);
        writer.addParameterInfo(this, pID, "aPropertyIndex", cc.aPropertyIndex);
    }

    foreach (const PlyNormalConfiguration& cc, m_config.normals) {
        const int pID = writer.addParameter(this, "Normal", true);
        writer.addParameterInfo(this, pID, "elementIndex", cc.elementIndex);
        writer.addParameterInfo(this, pID, "nxPropertyIndex", cc.nxPropertyIndex);
        writer.addParameterInfo(this, pID, "nyPropertyIndex", cc.nyPropertyIndex);
        writer.addParameterInfo(this, pID, "nzPropertyIndex", cc.nzPropertyIndex);
        writer.addParameterInfo(this, pID, "curvaturePropertyIndex", cc.ncPropertyIndex);
    }

    foreach (const PlyScalarConfiguration& cc, m_config.scalars) {
        const int pID = writer.addParameter(this, "Scalar", true);
        writer.addParameterInfo(this, pID, "elementIndex", cc.elementIndex);
        writer.addParameterInfo(this, pID, "valuePropertyIndex", cc.sPropertyIndex);
    }
}

bool CT_Reader_PLY::restoreSettings(SettingsReaderInterface &reader)
{
    if(!SuperClass::restoreSettings(reader))
        return false;

    QVariant value;
    bool ok;
    PlyReadConfiguration config;

    readNewConfiguration("elementIndex", m_config.vertex.elementIndex);
    readNewConfiguration("xPropertyIndex", m_config.vertex.xPropertyIndex);
    readNewConfiguration("yPropertyIndex", m_config.vertex.yPropertyIndex);
    readNewConfiguration("zPropertyIndex", m_config.vertex.zPropertyIndex);

    const int nColor = reader.parameterCount(this, "Color");

    for(int i=0; i<nColor; ++i) {
        PlyColorConfiguration cc;
        const int pID = reader.parameter(this, "Color", value);

        if(pID) {
            readNewConfigurationInfo("elementIndex", cc.elementIndex);
            readNewConfigurationInfo("rPropertyIndex", cc.rPropertyIndex);
            readNewConfigurationInfo("gPropertyIndex", cc.gPropertyIndex);
            readNewConfigurationInfo("bPropertyIndex", cc.bPropertyIndex);
            readNewConfigurationInfo("aPropertyIndex", cc.aPropertyIndex);

            config.colors.append(cc);
        }
    }

    const int nNormal = reader.parameterCount(this, "Normal");

    for(int i=0; i<nNormal; ++i) {
        PlyNormalConfiguration nc;
        const int pID = reader.parameter(this, "Normal", value);

        if(pID) {
            readNewConfigurationInfo("elementIndex", nc.elementIndex);
            readNewConfigurationInfo("nxPropertyIndex", nc.nxPropertyIndex);
            readNewConfigurationInfo("nyPropertyIndex", nc.nyPropertyIndex);
            readNewConfigurationInfo("nzPropertyIndex", nc.nzPropertyIndex);
            readNewConfigurationInfo("curvaturePropertyIndex", nc.ncPropertyIndex);

            config.normals.append(nc);
        }
    }

    const int nScalar = reader.parameterCount(this, "Scalar");

    for(int i=0; i<nScalar; ++i) {
        PlyScalarConfiguration sc;
        const int pID = reader.parameter(this, "Scalar", value);

        if(pID) {
            readNewConfigurationInfo("elementIndex", sc.elementIndex);
            readNewConfigurationInfo("valuePropertyIndex", sc.sPropertyIndex);

            config.scalars.append(sc);
        }
    }

    return setConfiguration(config);
}

CT_AbstractReader* CT_Reader_PLY::copy() const
{
    return new CT_Reader_PLY();
}

bool CT_Reader_PLY::plyReadMustStop() const
{
    return isStopped();
}

void CT_Reader_PLY::plyProgressChanged(int progress)
{
    setProgress(progress);
}

void CT_Reader_PLY::protectedInit()
{
    addNewReadableFormat(FileFormat("ply", tr("Fichiers de géométrie 3D .ply")));

    setToolTip(tr("Charge un nuage de points depuis un fichier au format PLY (Objet 3D)"));
}

void CT_Reader_PLY::protectedCreateOutItemDrawableModelList()
{
    CT_AbstractReader::protectedCreateOutItemDrawableModelList();

    addOutItemDrawableModel(DEF_Scene, new CT_Scene(), tr("Scene"));

    int size = m_config.colors.size();

    for(int i=0; i<size; ++i)
        addOutItemDrawableModel(DEF_Color + QString().setNum(i), new CT_PointsAttributesColor(), tr("Color %1").arg(i+1));

    size = m_config.normals.size();

    for(int i=0; i<size; ++i)
        addOutItemDrawableModel(DEF_Normal + QString().setNum(i), new CT_PointsAttributesNormal, tr("Normal %1").arg(i+1));

    size = m_config.scalars.size();

    for(int i=0; i<size; ++i)
        addOutItemDrawableModel(DEF_Scalar + QString().setNum(i), new CT_PointsAttributesScalarTemplated<float>(), tr("Scalar %1").arg(i+1));
}

bool CT_Reader_PLY::protectedReadFile()
{
    PlyHeaderReader headerReader;
    headerReader.setFilePath(filepath());

    if(!headerReader.process())
        return false;

    QList<Ply_CT_ColorCloud_Wrapper*> colorsCloud;
    QList<Ply_CT_NormalCloud_Wrapper*> normalsCloud;
    QList<Ply_CT_ScalarCloud_Wrapper<float>*> scalarsCloud;

    PlyHeader header = headerReader.getHeader();
    PlyFilePartReader<Ply_CT_PointCloud_Wrapper,
                      Ply_CT_ScalarCloud_Wrapper<float>,
                      Ply_CT_ColorCloud_Wrapper,
                      Ply_CT_NormalCloud_Wrapper> partReader(header);

    partReader.setListener(this);

    Ply_CT_PointCloud_Wrapper wrapper;
    partReader.setVertexPart(&wrapper,
                             m_config.vertex.elementIndex,
                             m_config.vertex.xPropertyIndex,
                             m_config.vertex.yPropertyIndex,
                             m_config.vertex.zPropertyIndex);

    foreach(const PlyColorConfiguration& cc, m_config.colors) {
        Ply_CT_ColorCloud_Wrapper* colorCloud = new Ply_CT_ColorCloud_Wrapper();

        partReader.addVertexColorPart(colorCloud,
                                      cc.elementIndex,
                                      cc.rPropertyIndex,
                                      cc.gPropertyIndex,
                                      cc.bPropertyIndex,
                                      cc.aPropertyIndex);

        colorsCloud.append(colorCloud);
    }

    foreach(const PlyNormalConfiguration& cc, m_config.normals) {
        Ply_CT_NormalCloud_Wrapper* normalCloud = new Ply_CT_NormalCloud_Wrapper();

        partReader.addVertexNormalPart(normalCloud,
                                      cc.elementIndex,
                                      cc.nxPropertyIndex,
                                      cc.nyPropertyIndex,
                                      cc.nzPropertyIndex,
                                      cc.ncPropertyIndex);

        normalsCloud.append(normalCloud);
    }

    foreach(const PlyScalarConfiguration& cc, m_config.scalars) {
        Ply_CT_ScalarCloud_Wrapper<float>* scalarCloud = new Ply_CT_ScalarCloud_Wrapper<float>();

        partReader.addVertexScalarPart(scalarCloud,
                                       cc.elementIndex,
                                       cc.sPropertyIndex);

        scalarsCloud.append(scalarCloud);
    }

    partReader.process();

    CT_Scene* scene = new CT_Scene(NULL, NULL, wrapper.pcir);
    scene->updateBoundingBox();

    // add the scene
    addOutItemDrawable(DEF_Scene, scene);

    int index = 0;
    foreach(Ply_CT_ColorCloud_Wrapper* cc, colorsCloud)
        addOutItemDrawable(DEF_Color + QString().setNum(index++), new CT_PointsAttributesColor(NULL, NULL, wrapper.pcir, cc->cloud));

    index = 0;
    foreach(Ply_CT_NormalCloud_Wrapper* nc, normalsCloud)
        addOutItemDrawable(DEF_Normal + QString().setNum(index++), new CT_PointsAttributesNormal(NULL, NULL, wrapper.pcir, nc->cloud));

    index = 0;
    foreach(Ply_CT_ScalarCloud_Wrapper<float>* sc, scalarsCloud)
        addOutItemDrawable(DEF_Scalar + QString().setNum(index++), new CT_PointsAttributesScalarTemplated<float>(NULL, NULL, wrapper.pcir, sc->cloud));

    qDeleteAll(colorsCloud.begin(), colorsCloud.end());
    qDeleteAll(normalsCloud.begin(), normalsCloud.end());
    qDeleteAll(scalarsCloud.begin(), scalarsCloud.end());

    return true;
}

CT_FileHeader* CT_Reader_PLY::protectedReadHeader(const QString &filepath, QString &error) const
{
    PlyHeaderReader headerReader;
    headerReader.setFilePath(filepath);
    headerReader.process();

    CT_PLYHeader* ctH = new CT_PLYHeader();
    ctH->setHeader(headerReader.getHeader());

    return ctH;
}
