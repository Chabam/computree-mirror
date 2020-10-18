#include "tools/attributes/worker/dm_attributesscalart.h"

#include "dm_guimanager.h"

#include "scene/permanentitemscenebymodel.h"
#include "tools/pointcloudattributesprovider.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtConcurrentMap>
#else
#include <QtConcurrent/QtConcurrentMap>
#endif

template<typename Type>
DM_AttributesScalarT<Type>::DM_AttributesScalarT(bool local) : DM_AbstractAttributesScalar(local)
{
    m_as = nullptr;
    m_autoAdjust = true;
    m_manualMin = 0;
    m_manualMax = 1;
    m_useSharedGradient = false;

    connect(&m_watcher, SIGNAL(progressRangeChanged(int,int)), this, SLOT(setProgressRanged(int,int)), Qt::DirectConnection);
    connect(&m_watcher, SIGNAL(progressValueChanged(int)), this, SLOT(setProgress(int)), Qt::DirectConnection);
    connect(this, SIGNAL(canceled()), &m_watcher, SLOT(cancel()));
}

template<typename Type>
DM_AttributesScalarT<Type>::~DM_AttributesScalarT()
{
}

template<typename Type>
void DM_AttributesScalarT<Type>::setAutoAdjust(bool automatic)
{
    m_autoAdjust = automatic;

    if(m_autoAdjust)
        autoAdjustMinMax();
}

template<typename Type>
void DM_AttributesScalarT<Type>::setMin(double min)
{
    m_manualMin = min;
    setAutoAdjust(false);
}

template<typename Type>
void DM_AttributesScalarT<Type>::setMax(double max)
{
    m_manualMax = max;
    setAutoAdjust(false);
}

template<typename Type>
void DM_AttributesScalarT<Type>::setGradient(const QLinearGradient &gradient)
{
    m_gradient = gradient;
}

template<typename Type>
void DM_AttributesScalarT<Type>::setUseSharedGradient(bool val)
{
    m_useSharedGradient = val;
}

template<typename Type>
bool DM_AttributesScalarT<Type>::setTypeAttributes(const Type *ta, const CT_AbstractAttributesScalar *as)
{
    if(ta != dynamic_cast<const Type*>(as))
        return false;

    setAttributes(ta);
    m_as = const_cast<CT_AbstractAttributesScalar*>(as);

    if(m_autoAdjust)
        autoAdjustMinMax();

    return true;
}

template<typename Type>
bool DM_AttributesScalarT<Type>::isUsedSharedGradient() const
{
    return m_useSharedGradient;
}

template<typename Type>
bool DM_AttributesScalarT<Type>::isAutoAdjust() const
{
    return m_autoAdjust;
}

template<typename Type>
bool DM_AttributesScalarT<Type>::autoAdjust() const
{
    return m_autoAdjust;
}

template<typename Type>
double DM_AttributesScalarT<Type>::min() const
{
    return m_manualMin;
}

template<typename Type>
double DM_AttributesScalarT<Type>::max() const
{
    return m_manualMax;
}

template<typename Type>
QLinearGradient DM_AttributesScalarT<Type>::gradient() const
{
    return m_gradient;
}

template<typename Type>
bool DM_AttributesScalarT<Type>::process(GDocumentViewForGraphics *doc)
{
    if(m_as == nullptr)
        return false;

    PermanentItemScene* scene = doc->getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr);

    if(scene == nullptr)
        return false;

    double range = m_manualMax-m_manualMin;

    if(qFuzzyIsNull(range))
        range = 1;

    DM_ColorLinearInterpolator interpolator;
    interpolator.constructFromQGradient(m_gradient);

    AMKgl::GlobalColorCloud* colorArray = scene->getPointCloudAttributesProvider()->createOrGetColorCloud();

    if(colorArray == nullptr)
        return false;

    ConcurrentMapInfo info;
    info.m_interpolator = &interpolator;
    info.m_as = m_as;
    info.m_cc = colorArray;
    info.m_fAccess = &m_fAccess;
    info.m_eAccess = &m_eAccess;
    info.m_manualMin = m_manualMin;
    info.m_range = range;
    info.mApplyLocal = mustApplyToLocalIndex();

    staticApply(&info);

    doc->dirtyColorsOfPoints();

    return true;
}

template<typename Type>
void DM_AttributesScalarT<Type>::attributesDeleted()
{
    m_as = nullptr;
}

template<typename Type>
CT_AbstractAttributesScalar* DM_AttributesScalarT<Type>::scalarAttributes() const
{
    return m_as;
}

template<typename Type>
Type* DM_AttributesScalarT<Type>::abstractTypeAttributes() const
{
    return dynamic_cast<Type*>(abstractAttributes());
}

template<typename Type>
void DM_AttributesScalarT<Type>::autoAdjustMinMax()
{
    if(mustApplyToLocalIndex())
    {
        m_manualMin = m_as->minLocalScalarAsDouble();
        m_manualMax = m_as->maxLocalScalarAsDouble();
    }
    else
    {
        m_manualMin = m_as->minScalarAsDouble();
        m_manualMax = m_as->maxScalarAsDouble();
    }
}
