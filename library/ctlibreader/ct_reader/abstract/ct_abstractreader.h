#ifndef CT_ABSTRACTREADER_H
#define CT_ABSTRACTREADER_H

#include "ctlibreader_global.h"
#include "ct_view/tools/fileformat.h"
#include "ct_reader/tools/ct_readeroutmodelstructuremanager.h"
#include "ct_itemdrawable/ct_fileheader.h"
#include "ct_itemdrawable/ct_standarditemgroup.h"

#include "ct_handle/ct_handleoutstdgroup.h"
#include "ct_handle/ct_handleoutsingularitem.h"
#include "ct_handle/ct_handleoutstditemattribute.h"
#include "ct_handle/ct_handleoutstditemattributewrapper.h"

#include <QObject>

#define READER_ALL_COPY_IMP(argClass) CT_AbstractReader* copy() const override { return new argClass(); } \
                                      CT_AbstractReader* copyFull() const override { return new argClass(*this); }

#define READER_COPY_IMP(argClass) CT_AbstractReader* copy() const override { return new argClass(); }
#define READER_COPY_FULL_IMP(argClass) CT_AbstractReader* copyFull() const override { return new argClass(*this); }

class CTLIBREADER_EXPORT CT_AbstractReader : public QObject
{
    Q_OBJECT

public:
    CT_AbstractReader();
    CT_AbstractReader(const CT_AbstractReader& other);
    virtual ~CT_AbstractReader();

    /**
      * @brief Init the reader (add readable formats, add out itemdrawable model, etc...)
      */
    //void init(bool initOutItemDrawableList = true);

    /**
     * @brief Returns a displayable name of the reader. By default return the result of method "uniqueName()"
     */
    virtual QString displayableName() const;

    /**
     * @brief Returns the unique name (you can use it to compare readers because it was unique). By default
     *        return the result of "metaObject()->className()"
     */
    virtual QString uniqueName() const;

    /**
     * @brief Returns the sub menu level where we can store this reader (by default "Others")
     */
    //virtual CT_StepsMenu::LevelPredefined getReaderSubMenuName() const;

    /**
      * @brief Set the filepath of the file to read. The signal "filePathModified()" will be emitted if the filepath is accepted.
      * @warning You must NOT change parameter of your class in this method, just check if the file is valid ! Change your parameter in method
      *          "configure()" if you want.
      * @return false if the filepath can not be modified (see method "filePathCanBeModified()" for more information) or if the file can not
      *         be read or open or does not exist. True otherwise.
      */
    virtual bool setFilePath(const QString& filepath);

    /**
      * @brief Return the filepath of the file
      */
    QString filepath() const;

    /**
     * @brief Enable/Disable the modification of the filepath
     */
    void setFilePathCanBeModified(bool enable);

    /**
     * @brief Return true if the filepath can be modified
     */
    bool filePathCanBeModified() const;

    // By default CT_AbstractReader don't have a Bounding Box : redefine in children class of geographical files
    //virtual bool hasBoundingBox() {return false;}

    /**
     * @brief Configure the reader.
     *        Call it after the method "setFilePath" and before the method "protectedCreateOutItemDrawableModelList"
     * @return false if the user as canceled the configuration
     */
    virtual bool configure() { return true; }

    /**
     * @brief Redefine this method if you have a special header. By default return a new CT_FileHeader empty.
     */
    virtual CT_FileHeader* createHeaderPrototype() const;

    /**
     * @brief Call this method to save settings of the reader like the filepath.
     * @warning If you redefine this method don't forget to call the method of the SuperClass if you redefine it !
     */
    virtual void saveSettings(SettingsWriterInterface& writer) const;

    /**
     * @brief Call this method to restore settings of the reader like the filepath.
     * @warning If you redefine this method don't forget to call the method of the SuperClass if you redefine it !
     */
    virtual bool restoreSettings(SettingsReaderInterface& reader);

    /**
     * @brief Call it to declare all output models.
     */
    void declareOutputModelsIn(CT_ReaderOutModelStructureManager& manager);

    /**
     * @brief Call it to declare all output models in a group of your result.
     * @param manager : The manager of output models
     * @param hParentGroup : The parent group where to add output models. It can be an input or an output handle
     *                       but must be of type CT_HandleXXXStdGroup
     */
    template<typename HandleGroup>
    void declareOutputModelsInGroup(CT_OutModelStructureManager& manager, HandleGroup& hParentGroup) {
        CT_ReaderOutModelStructureManager rManager = CT_ReaderOutModelStructureManager::createFromHandle(manager, hParentGroup);
        this->declareOutputModelsIn(rManager);
    }

    /**
      * @brief Return the list of out itemdrawable model
      */
    //const QList<CT_OutStdSingularItemModel*>& outItemDrawableModels() const;

    /**
      * @brief Return the list of out group model
      */
    //const QList<CT_OutStdGroupModel*>& outGroupsModel() const;

    /**
      * @brief Return all readable formats
      */
    const QList<FileFormat>& readableFormats() const;

    /**
     * @brief Returns true if the extension is present in readable formats
     * @param extension : must be the extension of the file WITHOUT the comma "."
     */
    bool isExtensionPresentInReadableFormat(const QString& extension) const;

    /**
     * @brief Return true if it was a read error
     */
    bool isReadError() const;

    /**
     * @brief Return true if the read is stopped
     */
    bool isStopped() const;

    /**
     * @brief Returns a tooltip to show in GUI. None by default.
     */
    QString toolTip() const;

    /**
     * @brief Return the error message. You can set them by a call to the method setErrorMessage()
     */
    QString errorMessage() const;

    /**
     * @brief Clear the error message
     */
    void clearErrorMessage();

    /**
     * @brief Return the list of out ItemDrawable created in the method "readFile" that have the type passed in parameter and remove
     *        them from this reader. Set the model and change the result of the ItemDrawable returned if result != NULL or model != NULL.
     *
     * @example QList<CT_AbstractSingularItemDrawable*> scenes = takeItemDrawableOfType(CT_Scene::staticGetType()); // if you want to change the model and the result yourself
     * @example QList<CT_AbstractSingularItemDrawable*> scenes = takeItemDrawableOfType(CT_Scene::staticGetType(), outResult, outSceneModel); // if you want to change the model and the result in this method
     *
     * @warning this method remove ItemDrawable from the reader !
     */
    //QList<CT_AbstractSingularItemDrawable*> takeItemDrawableOfType(const QString &type, const CT_AbstractResult *result = NULL, CT_OutAbstractItemModel *model = NULL);

    /**
     * @brief Return the list of out ItemDrawable created in the method "readFile" that have the modelName passed in parameter and remove
     *        them from this reader. Set the model and change the result of the ItemDrawable returned by this method if result != NULL or model != NULL.
     *
     * @example QList<CT_AbstractSingularItemDrawable*> scenes = takeItemDrawableOfModel(DEF_Reader_XXX_OutScene); // if you want to change the model and the result yourself
     * @example QList<CT_AbstractSingularItemDrawable*> scenes = takeItemDrawableOfModel(DEF_Reader_XXX_OutScene, outResult, outSceneModel); // if you want to change the model and the result in this method
     *
     * @warning this method remove ItemDrawable from the reader !
     */
    //QList<CT_AbstractSingularItemDrawable*> takeItemDrawableOfModel(const QString &modelName, const CT_AbstractResult *result = NULL, CT_OutAbstractItemModel *model = NULL);

    /**
     * @brief Return the list of out ItemDrawable created in the method "readFile" that have the modelName passed in parameter and remove
     *        them from this reader. The outModelName is used to search your model in your step and set it (with the result) to all itemdrawable returned.
     *
     * @example QList<CT_AbstractSingularItemDrawable*> scenes = takeItemDrawableOfModel(DEF_Reader_XXX_OutScene, outResult, DEF_MyOutScene);
     *
     * @warning this method remove ItemDrawable from the reader !
     */
    //QList<CT_AbstractSingularItemDrawable*> takeItemDrawableOfModel(const QString &modelName, const CT_AbstractResult *result, const QString &outModelName);

    /**
     * @brief Return the first ItemDrawable in the list that was created in the method "readFile" that have the type passed in parameter and remove
     *        them from this reader. Set the model and change the result of the ItemDrawable returned if result != NULL or model != NULL.
     *
     * @example CT_AbstractSingularItemDrawable *scene = takeFirstItemDrawableOfType(CT_Scene::staticGetType()); // if you want to change the model and the result yourself
     * @example CT_AbstractSingularItemDrawable *scene = takeFirstItemDrawableOfType(CT_Scene::staticGetType(), outResult, outSceneModel); // if you want to change the model and the result in this method
     *
     * @warning this method remove ItemDrawable from the reader !
     */
    //CT_AbstractSingularItemDrawable* takeFirstItemDrawableOfType(const QString &type, const CT_AbstractResult *result = NULL, CT_OutAbstractItemModel *model = NULL);

    /**
     * @brief Return the first ItemDrawable in the list that was created in the method "readFile" that have the modelName passed in parameter and remove
     *        them from this reader. Set the model and change the result of the ItemDrawable returned if result != NULL or model != NULL.
     *
     * @example CT_AbstractSingularItemDrawable *scene = takeFirstItemDrawableOfModel(DEF_Reader_XXX_OutScene); // if you want to change the model and the result yourself
     * @example CT_AbstractSingularItemDrawable *scene = takeFirstItemDrawableOfModel(DEF_Reader_XXX_OutScene, outResult, outSceneModel); // if you want to change the model and the result in this method
     *
     * @warning this method remove ItemDrawable from the reader !
     */
    //CT_AbstractSingularItemDrawable* takeFirstItemDrawableOfModel(const QString &modelName, const CT_AbstractResult *result = NULL, CT_OutAbstractItemModel *model = NULL);

    /**
     * @brief Return the first ItemDrawable in the list that was created in the method "readFile" that have the modelName passed in parameter and remove
     *        them from this reader. The outModelName is used to search your model in your step and set it (with the result) to the itemdrawable returned.
     *
     * @example CT_AbstractSingularItemDrawable *scene = takeFirstItemDrawableOfModel(DEF_Reader_XXX_OutScene, outResult, DEF_MyOutScene); // if you want to change the model and the result in this method
     *
     * @warning this method remove ItemDrawable from the reader !
     */
    //CT_AbstractSingularItemDrawable* takeFirstItemDrawableOfModel(const QString &modelName, const CT_AbstractResult *result, const QString &outModelName);

    /**
     * @brief Return the list of ItemDrawable created in the method "readFile" that have the type passed in parameter.
     *
     *        example : itemDrawableOfType( CT_Scene::staticGetType() )
     *
     * @warning this method DON'T remove ItemDrawable from the reader, it will be deleted by the reader in the destructor !
     */
    //QList<CT_AbstractSingularItemDrawable*> itemDrawableOfType(const QString &type) const;

    /**
     * @brief Return the list of ItemDrawable created in the method "readFile" that have the modelName passed in parameter.
     *
     *        example : itemDrawableOfModel( DEF_Reader_XXX_OutScene )
     *
     * @warning this method DON'T remove ItemDrawable from the reader, it will be deleted by the reader in the destructor !
     */
    //QList<CT_AbstractSingularItemDrawable*> itemDrawableOfModel(const QString &modelName) const;

    /**
     * @brief Return the first ItemDrawable created in the method "readFile" that have the modelName passed in parameter.
     *
     *        example : firstItemDrawableOfModel( DEF_Reader_XXX_OutScene )
     *
     * @warning this method DON'T remove ItemDrawable from the reader, it will be deleted by the reader in the destructor !
     */
    //CT_AbstractSingularItemDrawable* firstItemDrawableOfModel(const QString &modelName) const;

    /**
     * @brief Return the list of out Group created in the method "readFile" that have the type passed in parameter and remove
     *        them from this reader. Set the model and change the result of the Group returned if result != NULL or model != NULL.
     *
     * @warning this method remove Group from the reader !
     */
    //QList<CT_StandardItemGroup*> takeGroupOfModel(const QString &modelName, const CT_AbstractResult *result = NULL, CT_OutAbstractItemModel *model = NULL);

    /**
     * @brief Return the list of out Group created in the method "readFile" that have the type passed in parameter and remove
     *        them from this reader. The outModelName is used to search your model in your step and set it (with the result) to all groups returned.
     *
     * @warning this method remove Group from the reader !
     */
    //QList<CT_StandardItemGroup*> takeGroupOfModel(const QString &modelName, const CT_AbstractResult *result, const QString &outModelName);

    /**
     * @brief Return the first Group in the list that was created in the method "readFile" that have the type passed in parameter and remove
     *        them from this reader. Set the model and change the result of the Group returned if result != NULL or model != NULL.
     *
     * @warning this method remove Group from the reader !
     */
    //CT_StandardItemGroup* takeFirstGroupOfModel(const QString &modelName, const CT_AbstractResult *result = NULL, CT_OutAbstractItemModel *model = NULL);

    /**
     * @brief Return the first Group in the list that was created in the method "readFile" that have the type passed in parameter and remove
     *        them from this reader. The outModelName is used to search your model in your step and set it (with the result) to the group returned.
     *
     * @warning this method remove Group from the reader !
     */
    //CT_StandardItemGroup* takeFirstGroupOfModel(const QString &modelName, const CT_AbstractResult *result, const QString &outModelName);

    /**
     * @brief Return the list of Group created in the method "readFile" that have the type passed in parameter.
     *
     * @warning this method DON'T remove Group from the reader, it will be deleted by the reader in the destructor !
     */
    //QList<CT_StandardItemGroup*> groupOfModel(const QString &modelName) const;

    /**
     * @brief Return a empty copy of the reader
     */
    virtual CT_AbstractReader* copy() const = 0;

    /**
     * @brief Returns a copy with all parameters set and models
     */
    virtual CT_AbstractReader* copyFull() const = 0;

public slots:

    /**
     * @brief Read the header of the file and return it
     * @return NULL if it was no header or if it can not be readed. Check isReadError() to know if it was an error and get it with method errorMessage()
     */
    CT_FileHeader* readHeader();

    /**
     * @brief Read the file and the header
     * @return Return false if it was a problem when read the file, check the error with method isReadError() and get it with method errorMessage()
     */
    bool readFile(CT_StandardItemGroup* group);

    /**
     * @brief Cancel the read process
     */
    void cancel();

protected:

    /**
     * @brief Add a new readable format
     */
    void addNewReadableFormat(const FileFormat &format);

    /**
     * @brief Set the tooltip to show in gui
     */
    void setToolTip(const QString &t);

    /**
     * @brief Redefine it to declare all output models.
     */
    virtual void internalDeclareOutputModelsIn(CT_ReaderOutModelStructureManager& manager) = 0;

    /**
     * @brief Add the ItemDrawable you will create in result (it's a model). If you create multiple times the
     *        same ItemDrawable you must add multiple model with different name.
     */
    //void addOutItemDrawableModel(CT_OutStdSingularItemModel *item);
    /*void addOutItemDrawableModel(const QString &modelName,
                                 CT_AbstractSingularItemDrawable *item,
                                 const QString &displayableName = "",
                                 const QString &description = "");*/

    /**
     * @brief Add a new out ItemDrawable you have created in the method "protectedReadFile" for the model name "modelName"
     */
    //void addOutItemDrawable(const QString &modelName, CT_AbstractSingularItemDrawable *item);

    /**
     * @brief Add the Group you will create in result (it's a model). If you create multiple times the
     *        same Group you must add multiple model with different name.
     */
    //void addOutGroupModel(CT_OutStdGroupModel *gr);

    /**
     * @brief Add a new out Group you have created in the method "protectedReadFile" for the model name "modelName"
     */
    //void addOutGroup(const QString &modelName, CT_StandardItemGroup *gr);

    /**
     * @brief Returns true if the model with the unique name is contained in this reader
     */
    //bool containsItemDrawableModel(const QString &uniqueName) const;

    /**
      * \brief Set the progression when you read the file
      */
    void setProgress(int progress);

    /**
      * \brief Set a error message
      */
    void setErrorMessage(const QString &err);

    /**
     * @brief Init your reader (add readable formats, etc...)
     */
    //virtual void protectedInit() = 0;

    /*!
     *  \brief In this method you must add all ItemDrawable you create in result. Use the method "addOutItemDrawableModel". Called
     */
    //virtual void protectedCreateOutItemDrawableModelList();

    /**
     * @brief Inherit this method to read and get header of the filepath passed in parameter
     * @param filepath : the path to the file
     * @param error : output error
     * @return NULL if it was an error
     */
    virtual CT_FileHeader* internalReadHeader(const QString& filepath, QString& error) const;

    /**
     * @brief Inherit this method to read the file
     */
    virtual bool internalReadFile(CT_StandardItemGroup* group) = 0;

private:
    QList<FileFormat>                                       m_formats;
    QString                                                 m_tooltip;
    QString                                                 m_errorMess;
    int                                                     m_progress;
    bool                                                    m_error;
    bool                                                    m_stop;
    QString                                                 m_filePath;
    bool                                                    m_filepathCanBeModified;

    /*void clearOutItemDrawableModel();
    void clearOutItemDrawable();

    void clearOutGroupsModel();
    void clearOutGroups();*/

signals:
    /**
     * @brief Emitted when the reader start to read the file
     */
    void started();

    /**
     * @brief Emitted when the progress value has changed
     */
    void progressChanged(int progress);

    /**
     * @brief Emitted when the reader has finished to read the file
     */
    void finished();

    /**
     * @brief Emitted when the filepath has been modified
     */
    void filePathModified();
};

#endif // CT_ABSTRACTREADER_H
