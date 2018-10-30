#ifndef IAMKGLLOG_H
#define IAMKGLLOG_H

#include <QString>

/**
 * @brief Inherit this class to capture logs if you want. Used by "IEasyAccess" class.
 */
class IAMKglLog {
public:
    virtual ~IAMKglLog() {}

    /**
     * @brief Redefine this method to get debug message
     */
    virtual void addDebugMessage(const QString& mess) { Q_UNUSED(mess); }

    /**
     * @brief Redefine this method to get debug message
     */
    virtual void addInfoMessage(const QString& mess) { Q_UNUSED(mess); }

    /**
     * @brief Redefine this method to get debug message
     */
    virtual void addWarningMessage(const QString& mess) { Q_UNUSED(mess); }

    /**
     * @brief Redefine this method to get debug message
     */
    virtual void addErrorMessage(const QString& mess) { Q_UNUSED(mess); }
};

#endif // IAMKGLLOG_H
