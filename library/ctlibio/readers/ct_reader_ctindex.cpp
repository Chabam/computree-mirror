#include "ct_reader_ctindex.h"

//#include "ctlibplugin/ct_step/tools/menu/ct_stepsmenu.h"
#include "ctliblas/readers/ct_reader_lasv2.h"
#include "ctliblaz/readers/ct_reader_laz.h"

#include "headers/ct_ctiheader.h"

#include "ct_log/ct_logmanager.h"
#include <QFile>
#include <QDataStream>
#include <QTextStream>

#include <limits>

CT_Reader_CTIndex::CT_Reader_CTIndex(int subMenuLevel) : SuperClass(subMenuLevel)
{
    addNewReadableFormat(FileFormat("cti", tr("Fichiers d'indexation de points CTI")));
    setToolTip(tr("Le format CTI indexe des listes de points depuis une liste de fichiers de référence"));
    m_reader = nullptr;
}

CT_Reader_CTIndex::CT_Reader_CTIndex(const CT_Reader_CTIndex& other) : SuperClass(other)
{
    m_reader = nullptr;
}

CT_Reader_CTIndex::~CT_Reader_CTIndex()
{
    if (m_reader != nullptr) {delete m_reader;}
}

QString CT_Reader_CTIndex::displayableName() const
{
    return tr("Index de points, Fichier CTI");
}

bool CT_Reader_CTIndex::setFilePath(const QString& filepath)
{
    QString error;
    CT_CTIHeader* header = static_cast<CT_CTIHeader*>(internalReadHeader(filepath, error));

    if (!error.isEmpty()) {
        delete header;
        return false;
    }

    if (CT_AbstractReader::setFilePath(filepath)) {

        if (m_reader != nullptr)
        {
            delete m_reader;
            m_reader = nullptr;
        }

        if (header->fileFormat() == "LAS")
        {
            m_reader = new CT_Reader_LASV2();
        } else if (header->fileFormat() == "LAZ")
        {
            m_reader = new CT_Reader_LAZ();
        }

        delete header;
        return true;
    }

    delete header;
    return false;
}

CT_FileHeader *CT_Reader_CTIndex::createHeaderPrototype() const
{
    return new CT_CTIHeader();
}


CT_FileHeader* CT_Reader_CTIndex::internalReadHeader(const QString& filepath, QString& error) const
{
    Q_UNUSED(error);

    CT_CTIHeader* header = static_cast<CT_CTIHeader*>(createHeaderPrototype());
    header->setFilePath(filepath);

    if(QFile::exists(filepath))
    {
        QFile f(filepath);

        if(f.open(QFile::ReadOnly))
        {
            QDataStream stream(&f);

            QString fileFormat;
            stream >> fileFormat;
            header->setFileFormat(fileFormat);

            double xmin, ymin, xmax, ymax;
            stream >> xmin >> ymin >> xmax >> ymax;
            header->setExtent(xmin, ymin, xmax, ymax);

            QString areaShapeType;
            stream >> areaShapeType;

            if (areaShapeType == "CIRCLE") {
                double x, y, radius;
                stream >> x >> y >> radius;

                header->setCircle2DArea(x, y, radius);

            } else if (areaShapeType == "POLYGON") {
                int nVert;
                stream >> nVert;

                QVector<Eigen::Vector2d> verts(nVert);
                for (int i = 0 ; i < nVert ; i++)
                {
                    double x, y;
                    stream >> x >> y;
                    verts[i](0) = x;
                    verts[i](1) = y;
                }
                header->setPolygon2DArea(verts);

            } else {
                header->setBox2DArea(xmin, ymin, xmax, ymax);
            }

            header->setFirstFileIndexPos(f.pos());

            f.close();
        }

        // retreive BasePath in side file
        QFileInfo info(filepath);
        QString path = info.path() + "/" + "sourcePath.txt";

        QFile fp(path);

        if(fp.open(QFile::ReadOnly | QFile::Text))
        {
            QTextStream stream(&fp);

            if (!stream.atEnd())
            {
                QString line = stream.readLine();
                header->setBasePath(line);
            }
            fp.close();
        }
    }

    return header;
}

void CT_Reader_CTIndex::internalDeclareOutputModels(CT_ReaderOutModelStructureManager& manager)
{
    // Return the unique new point cloud (from the specific reader we have loaded)
    if (m_reader != nullptr) {m_reader->declareOutputModels(manager);}
}

bool CT_Reader_CTIndex::internalReadFile(CT_StandardItemGroup* group)
{    
    QString error;
    CT_CTIHeader *header = static_cast<CT_CTIHeader*>(internalReadHeader(filepath(), error));

    if(header == nullptr) {
        PS_LOG->addErrorMessage(LogInterface::reader, tr("Impossible de lire l'en-tête du fichier %1").arg(filepath()));
        return false;
    }

    // Read existing buffer file using local variables
    if(QFile::exists(filepath()))
    {
        QFile f(filepath());

        if(f.open(QFile::ReadOnly))
        {
            f.seek(header->firstFileIndexPos());
            QDataStream stream(&f);

            QString filename;
            bool all;
            qint64 tmp;
            qint64 index;

            while(!stream.atEnd())
            {
                FileBuffer fileBuffer;

                stream >> filename;
                stream >> all;
                stream >> tmp;

                fileBuffer.nPoints = tmp;

                // Reference files include all points, and thus have nPoints = 0.
                if (all)
                {
                    fileBuffer.nPoints = 0;
                } else
                {
                    fileBuffer.indexList.resize(fileBuffer.nPoints);
                    for(qint64 i = 0 ; i < fileBuffer.nPoints; i++)
                    {
                        stream >> index;
                        fileBuffer.indexList[i] = index;
                    }
                }

                // Use data path to get absolute file path
                fileBuffer.filename = header->basePath() + "/" + filename;

                m_fileBufferList.append(fileBuffer);
            }
            f.close();
        }
    }


    delete header;

    // Finally read the data files using the selected reader
    if(m_reader != nullptr)
    {
       if(m_reader->setMultipleFilePath(m_fileBufferList))
        {
            m_reader->readFile(group);
            return true;
        }
    }
    else
    {
        QString warn = tr("Reader for %1 wasn't created properly.").arg(m_fileType);
        PS_LOG->addWarningMessage(LogInterface::reader, warn);
    }

    // If anything fails before
    return false;
}
