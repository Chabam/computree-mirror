#ifndef PB_STEPLOADFILEBYNAME_H
#define PB_STEPLOADFILEBYNAME_H

#include "ct_step/abstract/ct_abstractstep.h"
#include "ct_reader/abstract/ct_abstractreader.h"
#include "ct_itemdrawable/ct_loopcounter.h"
#include "ct_itemdrawable/ct_standarditemgroup.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_tools/model/ct_autorenamemodels.h"

#include <QDirIterator>
#include <QFileInfo>


class PB_StepLoadFileByName: public CT_AbstractStep
{
    Q_OBJECT
    typedef CT_AbstractStep SuperClass;

public:

    /*! \brief Step constructor
     * 
     * Create a new instance of the step
     * 
     * \param dataInit Step parameters object
     */
    PB_StepLoadFileByName(CT_StepInitializeData &dataInit);

    /*! \brief Step description
     * 
     * Return a description of the step function
     */
    QString getStepDescription() const;

    /*! \brief Step detailled description
     * 
     * Return a detailled description of the step function
     */
    QString getStepDetailledDescription() const;

    /*! \brief Step URL
     * 
     * Return a URL of a wiki for this step
     */
    QString getStepURL() const;

    /*! \brief Step copy
     * 
     * Step copy, used when a step is added by step contextual menu
     */
    CT_VirtualAbstractStep* createNewInstance(CT_StepInitializeData &dataInit);

    void savePostSettings(SettingsWriterInterface &writer) const override;
    bool restorePostSettings(SettingsReaderInterface &reader) override;

protected:

    void createPreConfigurationDialog();

//    void createPostConfigurationDialog();

    bool postConfigure();

    void createInResultModelListProtected();

    /*! \brief Output results specification
     * 
     * Specification of output results models created by the step (OUT)
     */
    void createOutResultModelListProtected();

    /*! \brief Algorithm of the step
     * 
     * Step computation, using input results, and creating output results
     */
    void compute();

private:

    /**
     * @brief Contains the classname of the selected reader
     */
    QString                                         m_readerSelectedClassName;

    /**
     * @brief Contains the path of the selected folder
     */
    QString                                         m_folderPath;

    CT_AutoRenameModels     _outHeaderModelName;
    CT_AutoRenameModels     _outReaderModelName;

};

#endif // PB_STEPLOADFILEBYNAME_H
