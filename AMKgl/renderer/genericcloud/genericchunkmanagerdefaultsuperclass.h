#ifndef GENERICCHUNKMANAGERDEFAULTSUPERCLASS_H
#define GENERICCHUNKMANAGERDEFAULTSUPERCLASS_H

#include "renderer/attributes/iattributesaccessor.h"

/**
 * @brief Default super class of a GenericChunkManager
 */
template<typename RendererContextT>
class GenericChunkManagerDefaultSuperClass
{
public:
    typedef IAttributesAccessor AttributesAccessor;
    typedef RendererContextT    RendererContext;

    GenericChunkManagerDefaultSuperClass()
    {
        m_attributesAccessor = NULL;
        m_currentContext = NULL;
    }

    virtual ~GenericChunkManagerDefaultSuperClass() {}

    /**
     * @brief Set the attributes accessor
     */
    void setAttributesAccessor(const AttributesAccessor* aa)
    {
        lock();

        if(m_attributesAccessor != aa) {
            m_attributesAccessor = (AttributesAccessor*)aa;
            attributesAccessorChanged(aa);
        }

        unlock();
    }

    /**
     * @brief Returns the attributes accessor
     */
    IAttributesAccessor* getAttributesAccessor() const
    {
        return m_attributesAccessor;
    }

    /**
     * @brief Set the current context
     */
    void setCurrentContext(const RendererContext *c)
    {
        lock();

        if(m_currentContext != c) {
            m_currentContext = (RendererContext*)c;
            currentContextChanged(c);
        }

        unlock();
    }

    /**
     * @brief Returns the current context
     */
    RendererContext* getCurrentContext() const
    {
        return m_currentContext;
    }

    /**
     * @brief Lock to be thread safe
     */
    virtual void lock() = 0;

    /**
     * @brief Unlock the previous mutex
     */
    virtual void unlock() = 0;

private:
    /**
     * @brief Attributes accessor
     */
    AttributesAccessor*                     m_attributesAccessor;

    /**
     * @brief Current context
     */
    RendererContext*                        m_currentContext;

protected:

    /**
     * @brief Called when the attributes accessor changed
     */
    virtual void attributesAccessorChanged(const AttributesAccessor* aa)
    {
        Q_UNUSED(aa)
    }

    /**
     * @brief Called when the current context changed
     */
    virtual void currentContextChanged(const RendererContext* c)
    {
        Q_UNUSED(c)
    }
};

#endif // GENERICCHUNKMANAGERDEFAULTSUPERCLASS_H
