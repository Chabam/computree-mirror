#ifndef CT_ABSTRACTFILTER_H
#define CT_ABSTRACTFILTER_H

#include "ctlibfilters_global.h"
#include "ct_element/abstract/ct_abstractconfigurableelement.h"

/**
 * @brief A filter just filter something
 */
class CTLIBFILTERS_EXPORT CT_AbstractFilter : public CT_AbstractConfigurableElement
{
    Q_OBJECT
public:
    CT_AbstractFilter();
    CT_AbstractFilter(const CT_AbstractFilter &other);
    ~CT_AbstractFilter();

    /**
     * @brief Do nothing by default. You can inherit it if you want to do some change after configuration
     */
    void postConfigure() override {}

    void saveSettings(SettingsWriterInterface& writer) const override;
    bool restoreSettings(SettingsReaderInterface& reader) override;
};

#endif // CT_ABSTRACTFILTER_H
