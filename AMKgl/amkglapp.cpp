#include "amkglapp.h"
#include "tools/opengl/openglinfo.h"

AMKglApp* AMKglApp::UNIQUE_INSTANCE = nullptr;

AMKglApp::~AMKglApp()
{
	delete OPENGL_INFO;
}

void AMKglApp::setEasyAccess(const IEasyAccess *ea)
{
    m_easyAccess = (IEasyAccess*)ea;
}

IEasyAccess* AMKglApp::getEasyAccess() const
{
    return m_easyAccess;
}

AMKglApp::AMKglApp()
{
    m_easyAccess = nullptr;

    Q_INIT_RESOURCE(resource);
}
