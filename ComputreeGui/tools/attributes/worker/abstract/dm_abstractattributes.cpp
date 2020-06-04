#include "dm_abstractattributes.h"

#include "ct_itemdrawable/abstract/ct_abstractattributes.h"

DM_AbstractAttributes::DM_AbstractAttributes(bool local) : DM_AbstractWorker(),
    m_displayAlone(false),
    m_doc(nullptr),
    m_attributes(nullptr),
    mLocalOnly(local)
{
}

DM_AbstractAttributes::~DM_AbstractAttributes()
{
}

void DM_AbstractAttributes::setDisplayableName(const QString &name)
{
    m_name = name;
}

QString DM_AbstractAttributes::displayableName() const
{
    if(m_name.isEmpty()
            && (abstractAttributes() != nullptr))
        return abstractAttributes()->model()->displayableName();

    return m_name;
}

CT_AbstractAttributes* DM_AbstractAttributes::abstractAttributes() const
{
    return m_attributes;
}

void DM_AbstractAttributes::setDisplayAlone(bool val)
{
    m_displayAlone = val;
}

bool DM_AbstractAttributes::isDisplayedAlone() const
{
    return m_displayAlone;
}

bool DM_AbstractAttributes::mustApplyToLocalIndex() const
{
    return mLocalOnly;
}

void DM_AbstractAttributes::setDocument(const GDocumentViewForGraphics *doc)
{
    m_doc = const_cast<GDocumentViewForGraphics*>(doc);
}

GDocumentViewForGraphics *DM_AbstractAttributes::document() const
{
    return m_doc;
}

void DM_AbstractAttributes::apply()
{
    setProgress(0);
    setError("");

    if(!process(document()) && !isAnError())
        setError(tr("Une erreur inconnu est survenu lors du traitement."));

    setFinished();
}

void DM_AbstractAttributes::setAttributes(const CT_AbstractAttributes *att)
{
    m_attributes = const_cast<CT_AbstractAttributes*>(att);

    if(att != nullptr)
        connect(att, SIGNAL(destroyed()), this, SLOT(slotAttributesDeleted()), Qt::DirectConnection);
}

void DM_AbstractAttributes::slotAttributesDeleted()
{
    m_attributes = nullptr;
    attributesDeleted();
}
