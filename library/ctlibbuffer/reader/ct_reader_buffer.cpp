#include "ct_reader_buffer.h"

//#include "ctlibplugin/ct_step/tools/menu/ct_stepsmenu.h"
//#include "ctliblas/readers/ct_reader_lasv2.h"
//#include "ctliblaz/readers/ct_reader_laz.h"
#include "ct_log/ct_logmanager.h"
#include "headers/ct_ctiheader.h"

#include <QFile>
#include <QTextStream>

#include <limits>

CT_Reader_BUFFER::CT_Reader_BUFFER(int subMenuLevel) : SuperClass(subMenuLevel)
{
    addNewReadableFormat(FileFormat("cti", tr("Fichiers binaire de buffer .cti")));
    setToolTip(tr("Chargement du buffer depuis un fichier format binaire CTI (CompuTree Indexes for buffer)"));
    m_reader = nullptr;
}

CT_Reader_BUFFER::CT_Reader_BUFFER(const CT_Reader_BUFFER& other) : SuperClass(other)
{
}

CT_Reader_BUFFER::~CT_Reader_BUFFER()
{
//    m_reader = nullptr;
//    delete m_reader;
}

QString CT_Reader_BUFFER::displayableName() const
{
    return tr("Buffer de points, Fichier CTI");
}

bool CT_Reader_BUFFER::setFilePath(const QString& filepath)
{
    QString error;
    CT_CTIHeader* header = static_cast<CT_CTIHeader*>(internalReadHeader(filepath, error));

    if(!error.isEmpty()) {
        delete header;
        return false;
    }

    if(CT_AbstractReader::setFilePath(filepath)) {
        m_data_path = header->getDataPath();
        m_reader = header->getReader();

        delete header;
        return true;
    }

    delete header;
    return false;
}

CT_FileHeader* CT_Reader_BUFFER::internalReadHeader(const QString& filepath, QString& error) const
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
            stream.setByteOrder(QDataStream::LittleEndian);

            QString fileType;
            stream >> fileType;

            header->setFileType(fileType);

            f.close();
        }
    }

    header->setDataPath();

    return header;
}

void CT_Reader_BUFFER::internalDeclareOutputModels(CT_ReaderOutModelStructureManager& manager)
{
    Q_UNUSED(manager);
    // Return the unique new point cloud (from the specific reader we have loaded)
    //m_reader->declareOutputModels(manager);
}

bool CT_Reader_BUFFER::internalReadFile(CT_StandardItemGroup* group)
{
    // Read existing buffer file using local variables
    if(QFile::exists(filepath()))
    {
        QFile f(filepath());

        if(f.open(QFile::ReadOnly))
        {
            QDataStream stream(&f);
            stream.setByteOrder(QDataStream::LittleEndian);

            stream >> m_fileType;

            while(!stream.atEnd() && !isStopped())
            {
                FileBuffer fileBuffer;
                quint64 tmp;

                stream >> fileBuffer.filename >> tmp;

                fileBuffer.nPoints = static_cast<size_t>(tmp);

                // Reference files include all points, and thus have nPoints = 0.
                if(fileBuffer.nPoints != 0)
                {
                    for(size_t i=0 ; i<fileBuffer.nPoints ; i++)
                    {
                        quint64 index;
                        stream >> index;
                        fileBuffer.indexList.append(index);
                    }
                }

                // Use data path to get absolute file path
                fileBuffer.filename = m_data_path + fileBuffer.filename;

                m_fileBufferList.append(fileBuffer);
            }
        }
    }

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
