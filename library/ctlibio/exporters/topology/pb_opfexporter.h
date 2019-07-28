#ifndef PB_OPFEXPORTER_H
#define PB_OPFEXPORTER_H

#include "ct_exporter/abstract/ct_abstractexporter.h"

class QTextStream;
class CT_TTreeGroup;
class CT_TNodeGroup;
class CT_AbstractSingularItemDrawable;
class CT_MeshModel;
class CT_ItemAttributeList;
class CT_AbstractItemAttribute;
class CT_Mesh;

class PB_OPFExporter : public CT_AbstractExporter
{
    Q_OBJECT
    typedef CT_AbstractExporter SuperClass;

public:
    PB_OPFExporter();
    PB_OPFExporter(const PB_OPFExporter& other) = default;

    QString getExporterCustomName() const override;

    CT_StepsMenu::LevelPredefined getExporterSubMenuName() const override;

    void init() override;

    bool setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list) override;

    bool configureExport() override;

    CT_EXPORTER_DECL_COPY(PB_OPFExporter)

protected:
    bool protectedExportToFile() override;

private:
    QList<CT_Mesh*>        m_meshes;

    bool writeAttributesBdd(QTextStream &stream, CT_TTreeGroup *topology, const QString &prefix);
    void recursiveSearchAttributes(CT_OutAbstractModel *model, QHash<QString, QString> &out);
    bool writeMeshAndShapeBdd(QTextStream &stream, const QString &prefix);
    void recursiveSearchMesh(CT_TNodeGroup *node);
    bool writeMesh(QTextStream &stream, CT_Mesh *mesh, const int &index, const QString &prefix);
    bool writeShape(QTextStream &stream, CT_Mesh *mesh, const int &index, const QString &prefix);
    bool recursiveWriteFile(QTextStream &stream, const QString &type, CT_TNodeGroup *node, const QString &prefix);
    bool writeAttribute(QTextStream &stream, CT_TNodeGroup *node, CT_AbstractSingularItemDrawable *l, CT_AbstractItemAttribute *att, const QString &prefix);
    bool writeGeometry(QTextStream &stream, CT_TNodeGroup *node, CT_MeshModel *mesh, const QString &prefix);
};

#endif // PB_OPFEXPORTER_H
