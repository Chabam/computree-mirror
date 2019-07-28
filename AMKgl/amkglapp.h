#ifndef AMKGLAPP_H
#define AMKGLAPP_H

#include "interfaces/ieasyaccess.h"

#define AMKglEA AMKglApp::uniqueInstance()->getEasyAccess()
#define AMKglLOG AMKglApp::uniqueInstance()->getEasyAccess()->getLogger()

/**
 * @brief This is the main class that enable the use of the AMKgl motor
 */
class AMKglApp
{
public:
    static AMKglApp* uniqueInstance() {
        if(UNIQUE_INSTANCE == nullptr) {
            UNIQUE_INSTANCE = new AMKglApp();
        }

        return UNIQUE_INSTANCE;
    }

	~AMKglApp();

    /**
     * @brief Before use anything of the motor your must set your own IEasyAccess object !
     */
    void setEasyAccess(const IEasyAccess* ea);

    /**
     * @brief Returns the IEasyAccess object
     */
    IEasyAccess* getEasyAccess() const;

private:
    static AMKglApp*    UNIQUE_INSTANCE;
    IEasyAccess*        m_easyAccess;

    AMKglApp();
};

#endif // AMKGLAPP_H
