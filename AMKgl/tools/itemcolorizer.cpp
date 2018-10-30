#include "itemcolorizer.h"

#include "scene/permanentitemscenebymodel.h"
#include "renderer/ichunk.h"
#include "definecolor.h"
#include "scene/tools/dispatchinformation.h"

ItemColorizer::ItemColorizer()
{
    m_sceneToVisit = NULL;
    m_scene = NULL;
    m_item = NULL;
}

void ItemColorizer::setSceneToVisit(const PermanentItemSceneByModel *scene)
{
    m_sceneToVisit = (PermanentItemSceneByModel*)scene;
}

void ItemColorizer::setScene(const PermanentItemScene *scene)
{
    m_itemInfo = NULL;
    m_scene = (PermanentItemScene*)scene;

    if((m_scene != NULL) && (m_item != NULL))
        m_itemInfo = m_scene->getPermanentItemInformation(m_item);
}

void ItemColorizer::setItem(const AMKgl::Item *item)
{
    m_item = (AMKgl::Item*)item;
}

void ItemColorizer::setColor(const QColor &color)
{
    m_color = color;
}

void ItemColorizer::apply()
{
    if((m_sceneToVisit != NULL)
            && (m_item != NULL))
        m_sceneToVisit->visitObjectsOfOneItem(m_item, *this);
}

void ItemColorizer::staticSetColorToItem(const AMKgl::Item *item, const QColor &color, const PermanentItemSceneByModel *scene)
{
    ItemColorizer v;
    v.setItem(item);
    v.setSceneToVisit(scene);
    v.setColor(color);
    v.apply();
}

void ItemColorizer::visitChunk(const IChunk* chunk)
{
    if(m_itemInfo != NULL) {
        DispatchInformation* objInfos = m_itemInfo->getObjectInformationForChunk(chunk);

        Q_ASSERT(objInfos != NULL);

        const size_t count = objInfos->count();

        if(count > 0) {
            Basic::AbstractColorCloud* colorCloud = ((IChunk*)chunk)->createOrGetObjectColorCloud();

            Q_ASSERT(colorCloud != NULL);

            const size_t begin = objInfos->begin();
            size_t index = objInfos->begin() + count;

            do  {
                (*colorCloud)[--index] = m_color;
            }while(index != begin);
        }
    }
}
