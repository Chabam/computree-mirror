#ifndef PB_STEPLOOPONFILES_H
#define PB_STEPLOOPONFILES_H

#include "ct_step/ct_stepbeginloop.h"
#include "ct_reader/abstract/ct_abstractreader.h"
#include "ct_itemdrawable/ct_loopcounter.h"
#include "ct_itemdrawable/ct_standarditemgroup.h"
#include "ct_result/ct_resultgroup.h"

#include <QDirIterator>


class PB_StepLoopOnFiles: public CT_StepBeginLoop
{
    Q_OBJECT
    typedef CT_StepBeginLoop SuperClass;

public:

    /*! \brief Step constructor
     * 
     * Create a new instance of the step
     * 
     * \param dataInit Step parameters object
     */
    PB_StepLoopOnFiles(CT_StepInitializeData &dataInit);
    ~PB_StepLoopOnFiles();

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

    void savePostSettings(SettingsWriterInterface& writer) const override;
    bool restorePostSettings(SettingsReaderInterface &reader) override;

protected:

    void createPreConfigurationDialog();

    bool postConfigure();

    /*! \brief Output results specification
     * 
     * Specification of output results models created by the step (OUT)
     */
    void createOutResultModelListProtected(CT_OutResultModelGroup *firstResultModel);

    /*! \brief Algorithm of the step
     * 
     * Step computation, using input results, and creating output results
     */
    void compute(CT_ResultGroup *outRes, CT_StandardItemGroup *group);

private:

    /**
     * @brief Contains the classname of the selected reader
     */
    QString                                         m_readerSelectedClassName;

    /**
     * @brief Contains the path of the selected folder
     */
    QString                                         m_folderPath;

    /**
     * @brief Iterator to iterate over files in the compute methods
     */
    QDirIterator*                                   m_folderIterator;
};

#endif // PB_STEPLOOPONFILES_H
