#ifndef CT_CTIHEADER_H
#define CT_CTIHEADER_H

#include "ctlibbuffer/ctlibbuffer_global.h"
#include "ct_reader/abstract/ct_abstractreader.h"
#include "ctlibplugin/ct_step/tools/menu/ct_stepsmenu.h"
#include "ctliblas/readers/ct_reader_lasv2.h"
#include "ctliblaz/readers/ct_reader_laz.h"
#include "ct_itemdrawable/ct_fileheader.h"
#include "ct_log/ct_logmanager.h"

class CTLIBBUFFER_EXPORT CT_CTIHeader : public CT_FileHeader
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_CTIHeader, CT_FileHeader, CTI Header)

    using SuperClass = CT_FileHeader;

public:
    CT_CTIHeader() { SuperClass(); };
    CT_CTIHeader(const CT_CTIHeader& other) = default;

    void setFileType(const QString & fileType)
    {
        m_fileType = fileType;

        // Using data format and list of files, create the appropriate reader.
        if(m_fileType == "LAS")
        {
            m_reader = new CT_Reader_LASV2(CT_StepsMenu::LP_Points);
        }
        if(m_fileType == "LAZ")
        {
            m_reader = new CT_Reader_LAZ(CT_StepsMenu::LP_Points);
        }
        // NOTE : Prepare only the LAS and LAZ format for the beginning.
//        if(m_fileType == "XYB")
//        {
//            m_reader = new CT_Reader_XYB(CT_StepsMenu::LP_Points);
//        }
//        if(m_fileType == "ASCI")
//        {
//            m_reader = new CT_Reader_ASCII(CT_StepsMenu::LP_Points);
//        }
    }

    void setDataPath()
    {
        // Get data path, and test if side-car file exists beside.
        // If yes, read it and get the new data path.
        auto file_info = QFileInfo(filePath());
        m_data_path = file_info.path();
        QString side_file = "data_path.txt";

        if(QFile::exists(m_data_path + side_file))
        {
            QFile f(m_data_path + side_file);

            if(f.open(QFile::ReadOnly))
            {
                QString path;
                QTextStream stream(&f);
                stream << path;

                auto info = QFileInfo(path);
                if(info.isAbsolute())
                    m_data_path = path;
                else
                    m_data_path += path;

                if(!QFile::exists(m_data_path))
                {
                    QString warn = tr("Path indicated in %1 file does not exists.").arg(side_file);
                    PS_LOG->addWarningMessage(LogInterface::reader, warn);
                    m_data_path = file_info.path();
                }
            }
        }
    }

    QString            getDataPath() const {return m_data_path;}
    CT_AbstractReader* getReader  () const {return m_reader;}

    CT_ITEM_COPY_IMP(CT_CTIHeader)

private:
    QString            m_data_path;
    QString            m_fileType;
    CT_AbstractReader* m_reader;
};

#endif // CT_CTIHEADER_H
