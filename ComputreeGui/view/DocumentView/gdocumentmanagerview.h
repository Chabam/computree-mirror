/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : MichaÃ«l KREBS (ARTS/ENSAM)

 This file is part of Computree version 2.0.

 Computree is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Computree is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Computree.  If not, see <http://www.gnu.org/licenses/>.

*****************************************************************************/


#ifndef GDOCUMENTMANAGERVIEW_H
#define GDOCUMENTMANAGERVIEW_H

#include "dm_documentmanagerview.h"
#include "ct_step/abstract/ct_virtualabstractstep.h"

#include "tools/graphicsview/dm_colorlinearinterpolator.h"

#include <QMdiArea>
#include <QMutex>
#include <QTimer>

class GDocumentView;
class GGraphicsViewSynchronizedGroup;

class GDocumentManagerView : public QMdiArea, public DM_DocumentManagerView, public DM_IDocumentCloseFilter, public DM_IDocumentAddFilter
{
    Q_OBJECT

public:
    explicit GDocumentManagerView(QWidget *parent = 0);
    ~GDocumentManagerView();

    void setSyncManager(const GGraphicsViewSynchronizedGroup *syncMan);

    bool addAllItemDrawableOfResultToActiveDocument(CT_AbstractResult &res, DM_AsynchroneProgress &progress) override;
    bool addAllItemDrawableOfModelToDocument(CT_AbstractResult &res, CT_OutAbstractItemModel &model, DM_DocumentView *doc, DM_AsynchroneProgress &progress) override;
    bool addAllItemDrawableOfResultToDocument(CT_AbstractResult &res, DM_DocumentView *doc, DM_AsynchroneProgress &progress) override;

    bool addAllItemDrawableOfListToActiveDocument(QList<CT_AbstractItemDrawable*> &itemList, DM_AsynchroneProgress &progress) override;
    bool addAllItemDrawableOfListToDocument(QList<CT_AbstractItemDrawable*> &itemList, DM_DocumentView *doc, DM_AsynchroneProgress &progress) override;

    bool removeAllItemDrawableOfResultFromDocuments(CT_AbstractResult &res, DM_AsynchroneProgress &progress) override;
    bool removeAllItemDrawableOfModelFromDocuments(CT_OutAbstractItemModel &model, DM_AsynchroneProgress &progress) override;
    bool removeAllItemDrawableOfModelFromDocument(CT_OutAbstractItemModel &model, DM_DocumentView *doc, DM_AsynchroneProgress &progress) override;
    bool removeAllItemDrawableOfListFromDocuments(QList<CT_AbstractItemDrawable *> &itemList, DM_AsynchroneProgress &progress) override;
    bool removeAllItemDrawableOfListFromDocument(QList<CT_AbstractItemDrawable *> &itemList, DM_DocumentView *doc, DM_AsynchroneProgress &progress) override;

    DM_DocumentView* getActiveDocumentView() override;
    DM_DocumentView* getDocumentView(int index) const override;
    int nbDocumentView() const override;
    bool containsDocument(DM_Document *document) const override;

    void redrawAllDocument() override;

    DocumentInterface* new3DDocument(bool fromGui, bool inLoadConfigurationFromMainWindow, float pointSize = 1.0, bool orthographic = true, QColor *color = nullptr);
    DocumentInterface* new2DDocument(bool fromGui, bool inLoadConfigurationFromMainWindow);
    DocumentInterface* newTreeViewDocument(bool fromGui, bool inLoadConfigurationFromMainWindow);

    DocumentInterface* new3DDocument(float pointSize = 1.0, bool orthographic = true, QColor *color = nullptr) override;
    DocumentInterface* new2DDocument() override;
    DocumentInterface* newTreeViewDocument() override;
    int nDocuments() const override;
    DocumentInterface* documentAt(const int &index) const override;
    QList<DocumentInterface*> documents() const override;
    DocumentInterface* activeDocument() override;
    void activateDocument(DocumentInterface *doc) override;
    bool closeDocument(DocumentInterface *doc) override;
    bool closeDocument(const int &index) override;

    // DM_IDocumentCloseFilter
    bool canClose(const DM_Document *document) const override;

    // DM_IDocumentAddFilter
    bool canAddItemDrawable(const DM_Document *document, const CT_AbstractItemDrawable *item) const override;

    virtual QColor intermediateColorFromSelectedGradient(double key) override;


private:

    QMutex                          *m_mutex;
    int                             m_windowCount;
    QTimer                          m_timer;
    GGraphicsViewSynchronizedGroup  *m_syncMan;
    QMdiSubWindow                   *m_lastDocumentActivated;
    bool                            m_manualModeEnabled;
    CT_VirtualAbstractStep*         m_manualModeEnabledByStep;
    QList<GDocumentView*>           m_docAddedInNormalMode;
    QList<GDocumentView*>           m_docFromGuiAddedInManualMode;
    QList<GDocumentView*>           m_docToCloseAfterQuitManualMode;

    DM_ColorLinearInterpolator      m_colorLinearInterpolator;

    void addDocumentView(GDocumentView &view, bool fromGui, bool inLoadConfigurationFromMainWindow);
    QMdiSubWindow* subWindowFromDocument(DocumentInterface *doc) const;

public slots:
    void stepRequiredManualMode(CT_VirtualAbstractStep *step);
    void stepFinished(CT_VirtualAbstractStep *step);
    void newGradientSelected(QLinearGradient gradient);

signals:

    void documentAdded(DM_DocumentView *view);
    void documentToBeClosed(DM_DocumentView *view);
    void documentRemoved();
    void documentActivated(DM_DocumentView *view);
    void documentHidden(DM_DocumentView *view);
    void documentShowned(DM_DocumentView *view);

private slots:

    void slotDocumentDestroyed();
    void subWindowHasBeenActivated(QMdiSubWindow *window);
    void verifyNumberWindow();
};

#endif // GDOCUMENTMANAGERVIEW_H
