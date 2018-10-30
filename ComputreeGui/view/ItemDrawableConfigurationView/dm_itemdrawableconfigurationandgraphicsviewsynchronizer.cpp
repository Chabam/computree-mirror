#include "dm_itemdrawableconfigurationandgraphicsviewsynchronizer.h"

#include "view/DocumentView/gdocumentviewforgraphics.h"

DM_ItemDrawableConfigurationAndGraphicsViewSynchronizer::DM_ItemDrawableConfigurationAndGraphicsViewSynchronizer()
{
    m_managerView = NULL;
}

void DM_ItemDrawableConfigurationAndGraphicsViewSynchronizer::setItemDrawableConfigurationView(GItemDrawableConfigurationManagerView *managerView)
{
    if(m_managerView != NULL)
        disconnect(m_managerView, NULL, this, NULL);

    m_managerView = managerView;

    if(m_managerView != NULL)
        connect(m_managerView, SIGNAL(applyNewConfiguration(CT_ItemDrawableConfiguration*)), this, SLOT(valueHasChangedForConfiguration(CT_ItemDrawableConfiguration*)), Qt::DirectConnection);
}

void DM_ItemDrawableConfigurationAndGraphicsViewSynchronizer::valueHasChangedForConfiguration(CT_ItemDrawableConfiguration *config)
{
    GDocumentViewForGraphics *doc = dynamic_cast<GDocumentViewForGraphics*>(m_managerView->getDocumentView());

    if(doc != NULL)
        doc->updateDrawing3DOfItemDrawablesInGraphicsView(m_managerView->itemDrawablesForConfiguration(config));
}
