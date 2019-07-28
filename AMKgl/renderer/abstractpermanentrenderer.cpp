#include "abstractpermanentrenderer.h"

AbstractPermanentRenderer::AbstractPermanentRenderer()
{
    m_attributesAccessor = nullptr;
    m_currentColor = Basic::LocalColor(QColor(Qt::white));
}

void AbstractPermanentRenderer::setAttributesAccessor(const IAttributesAccessor *accessor)
{
    m_attributesAccessor = (IAttributesAccessor*)accessor;
}

IAttributesAccessor* AbstractPermanentRenderer::getAttributesAccessor() const
{
    return m_attributesAccessor;
}

AnyElementAttributesProvider* AbstractPermanentRenderer::getAnyElementsAttributesProvider() const
{
    return ((getAttributesAccessor() == nullptr) ? nullptr : getAttributesAccessor()->getAnyElementsAttributesProvider());
}

FlagsPropertyManager* AbstractPermanentRenderer::getFlagsPropertyManager() const
{
    return (FlagsPropertyManager*)&m_flagsPropertyManager;
}

void AbstractPermanentRenderer::setCurrentColor(const Basic::LocalColor &color)
{
    if(m_currentColor != color) {
        m_currentColor = color;
        currentColorChanged();
    }
}

Basic::LocalColor AbstractPermanentRenderer::getCurrentColor() const
{
    return m_currentColor;
}
