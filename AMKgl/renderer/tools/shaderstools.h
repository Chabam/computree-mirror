#ifndef SHADERSTOOLS_H
#define SHADERSTOOLS_H

#include "amkglapp.h"
#include <QOpenGLShaderProgram>

/**
  * @brief Call the FUNC_TO_CALL only if PROGRAM is not NULL. If the function return an error we add it to the log
  *        and clean the program from memory
  */
#define ShadersTools_CHECK_PROGRAM_ERROR(PROGRAM, FUNC_TO_CALL) if((PROGRAM != NULL) && !(ok = FUNC_TO_CALL)) { \
                                                                    AMKglLOG->addErrorMessage(QString(PROGRAM->metaObject()->className()) + ": " + PROGRAM->log()); \
                                                                    delete PROGRAM; \
                                                                    PROGRAM = NULL; \
                                                                  }

class ShadersTools
{
public:

    /**
     * @brief Merge shaders to one source code
     * @param sourceFilePath: filepath of shaders in the order of concatenation
     * @param finalSourceCode : (OUT) final merged source code
     * @param error : will contains an error string if the method return false
     * @return true if the merge was ok
     */
    static bool staticMergeShadersFromSourceFile(const QStringList &sourceFilePath, QString& finalSourceCode, QString* error = NULL);
};

#endif // SHADERSTOOLS_H
