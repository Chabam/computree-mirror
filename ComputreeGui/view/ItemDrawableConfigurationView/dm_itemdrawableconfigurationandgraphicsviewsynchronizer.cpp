#include "dm_itemdrawableconfigurationandgraphicsviewsynchronizer.h"

#include "view/DocumentView/gdocumentviewforgraphics.h"

DM_ItemDrawableConfigurationAndGraphicsViewSynchronizer::DM_ItemDrawableConfigurationAndGraphicsViewSynchronizer()
{
    m_managerView = nullptr;
}

void DM_ItemDrawableConfigurationAndGraphicsViewSynchronizer::setItemDrawableConfigurationView(GItemDrawableConfigurationManagerView *managerView)
{
    if(m_managerView != nullptr)
        disconnect(m_managerView, nullptr, this, nullptr);

    m_managerView = managerView;

    if(m_managerView != nullptr)
        connect(m_managerView, SIGNAL(applyNewConfiguration(CT_ItemDrawableConfiguration*)), this, SLOT(valueHasChangedForConfiguration(CT_ItemDrawableConfiguration*)), Qt::DirectConnection);
}

void DM_ItemDrawableConfigurationAndGraphicsViewSynchronizer::valueHasChangedForConfiguration(CT_ItemDrawableConfiguration *config)
{
    GDocumentViewForGraphics *doc = dynamic_cast<GDocumentViewForGraphics*>(m_managerView->getDocumentView());

    if(doc != nullptr)
        doc->updateDrawing3DOfItemDrawablesInGraphicsView(m_managerView->itemDrawablesForConfiguration(config));
}
