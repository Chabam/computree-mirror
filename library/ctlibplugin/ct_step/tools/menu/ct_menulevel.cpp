#include "ct_menulevel.h"

#include "ct_step/abstract/ct_virtualabstractstep.h"
#include "ct_step/tools/menu/ct_stepsmenu.h"

CT_MenuLevel* CT_MenuLevel::staticCreateOrGetLevelInParentLevel(const QString &customDisplayableName, CT_MenuLevel *parentLevel)
{
    Q_ASSERT(parentLevel != nullptr);

    CT_MenuLevel* level = parentLevel->levelFromDisplayableName(customDisplayableName);

    if(level == nullptr) {
        level = new CT_MenuLevel(customDisplayableName);
        parentLevel->addLevel(level);
    }

    return level;
}

CT_MenuLevel* CT_MenuLevel::staticCreateOrGetLevelInParentLevel(const CT_StepsMenu::LevelPredefined& predefinedLevel, CT_MenuLevel* parentLevel)
{
    return staticCreateOrGetLevelInParentLevel(CT_StepsMenu::staticPredefinedToString(predefinedLevel), parentLevel);
}

QString CT_MenuLevel::displayableName() const
{
    return m_displayableName;
}

bool CT_MenuLevel::addStep(CT_VirtualAbstractStep* step)
{
    return addStepToCollectionOrDeleteIt(step);
}

void CT_MenuLevel::raiseStepInMenu(CT_VirtualAbstractStep* step)
{
    const int index = m_steps.indexOf(step);

    if (index > 0)
        m_steps.move(index, index-1);
}

void CT_MenuLevel::lowerStepInMenu(CT_VirtualAbstractStep* step)
{
    const int index = m_steps.indexOf(step);

    if ((index >= 0) && (index < (m_steps.size()-1)))
        m_steps.move(index, index+1);
}

bool CT_MenuLevel::addNotFoundedStep(const CT_NotFoundedStep& nfs)
{
    if(m_stepsNotFounded.contains(nfs))
        return false;

    m_stepsNotFounded.append(nfs);
    return true;
}

QList<CT_VirtualAbstractStep *> CT_MenuLevel::steps() const
{
    if (isAFavoriteSubLevel())
    {
        return m_steps;
    }

    return m_stepsSorted.values();
}

QList<CT_MenuLevel::CT_NotFoundedStep> CT_MenuLevel::stepsNotFounded() const
{
    return m_stepsNotFounded;
}

QList<CT_MenuLevel *> CT_MenuLevel::levels() const
{
    QList<CT_MenuLevel*> l;
    QList<CT_MenuLevel*> levelsPredefinedSorted = m_levelsPredefined;
    QList<CT_MenuLevel*> levelsCustomSorted = m_levelsCustom;

    static const auto sortLevelsByDisplayableName = [](const CT_MenuLevel* l1, const CT_MenuLevel* l2) -> bool {
        return l1->displayableName().toLower() < l2->displayableName().toLower();
    };

    std::sort(levelsPredefinedSorted.begin(), levelsPredefinedSorted.end(), sortLevelsByDisplayableName);
    std::sort(levelsCustomSorted.begin(), levelsCustomSorted.end(), sortLevelsByDisplayableName);

    l.append(levelsPredefinedSorted);
    l.append(levelsCustomSorted);

    return l;
}

QList<CT_MenuLevel *> CT_MenuLevel::levelsWithSteps() const
{
    QList<CT_MenuLevel*> ret;

    const LevelCollection levs = levels();

    for(CT_MenuLevel* l : levs) {
        if(l->isStepsNotEmptyRecursively())
            ret.append(l);
    }

    return ret;
}

CT_MenuLevel* CT_MenuLevel::parentLevel() const
{
    return m_parent;
}

CT_MenuLevel::CT_MenuLevel()
{
    m_parent = nullptr;
}

CT_MenuLevel::CT_MenuLevel(const QString &displayableName)
{
    m_displayableName = displayableName;
    m_parent = nullptr;
}

CT_MenuLevel::~CT_MenuLevel()
{
    for(CT_VirtualAbstractStep* s : m_steps) {
        disconnect(s, nullptr, this, nullptr);
        delete s;
    }

    const LevelCollection levs = levels();

    for(CT_MenuLevel* l : levs) {
        disconnect(l, nullptr, this, nullptr);
        delete l;
    }
}

bool CT_MenuLevel::isStepsNotEmptyRecursively() const
{
    if(!m_steps.isEmpty())
        return true;

    const LevelCollection levs = levels();

    for(CT_MenuLevel* l : levs) {
        if(l->isStepsNotEmptyRecursively())
            return true;
    }

    return false;
}

bool CT_MenuLevel::addStepToCollectionOrDeleteIt(CT_VirtualAbstractStep* step)
{
    if(existStepInCollection(step)) {
        delete step;
        return false;
    }

    m_steps.append(step);
    m_stepsSorted.insert(step->description().toLower(), step);

    connect(step, SIGNAL(destroyed(QObject*)), this, SLOT(stepDeleted(QObject*)), Qt::DirectConnection);

    if(isAFavoriteSubLevel())
        step->pluginStaticCastT<>()->initStep(step);

    return true;
}

bool CT_MenuLevel::existStepInCollection(const CT_VirtualAbstractStep* step)
{
    const QString className = step->metaObject()->className();

    for(CT_VirtualAbstractStep* s : m_steps) {
        if(s->metaObject()->className() == className && s->name() == step->name())
            return true;
    }

    return false;
}

void CT_MenuLevel::addLevel(CT_MenuLevel *level)
{
    if(CT_StepsMenu::staticIsPredefinedLevelString(level->displayableName()))
        m_levelsPredefined.append(level);
    else
        m_levelsCustom.append(level);

    level->setParentLevel(this);

    connect(level, SIGNAL(destroyed(QObject*)), this, SLOT(subLevelDeleted(QObject*)), Qt::DirectConnection);
}

void CT_MenuLevel::setParentLevel(CT_MenuLevel *parent)
{
    m_parent = parent;
}

CT_MenuLevel* CT_MenuLevel::levelFromDisplayableName(const QString& name)
{
    const LevelCollection levs = levels();

    for(CT_MenuLevel* l : levs) {
        if(l->displayableName() == name)
            return l;
    }

    return nullptr;
}

bool CT_MenuLevel::isAFavoriteSubLevel() const
{
    const QString favoritesName = CT_StepsMenu::staticOperationToDisplayableName(CT_StepsMenu::LO_Favorites);
    CT_MenuLevel* l = const_cast<CT_MenuLevel*>(this);

    while((l = l->parentLevel()) != nullptr)
    {
        if(l->displayableName() == favoritesName)
            return true;
    }

    return false;
}

void CT_MenuLevel::subLevelDeleted(QObject* level)
{
    CT_MenuLevel* mLevel = static_cast<CT_MenuLevel*>(level);

    if(!m_levelsCustom.removeOne(mLevel))
        m_levelsPredefined.removeOne(mLevel);
}

void CT_MenuLevel::stepDeleted(QObject* step)
{
    m_steps.removeOne(static_cast<CT_VirtualAbstractStep*>(step));
}
