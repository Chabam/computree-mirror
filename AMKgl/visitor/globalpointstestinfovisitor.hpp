#ifndef GLOBALPOINTSTESTINFOVISITOR_HPP
#define GLOBALPOINTSTESTINFOVISITOR_HPP

#include "globalpointstestinfovisitor.h"

template<typename FoundedCloudIndex>
GlobalPointsTestInfoVisitor<FoundedCloudIndex>::GlobalPointsTestInfoVisitor()
{
    m_infoCloud = nullptr;
    m_testFunction = nullptr;

    m_foundedPoints = new FoundedCloudIndex();
}

template<typename FoundedCloudIndex>
GlobalPointsTestInfoVisitor<FoundedCloudIndex>::~GlobalPointsTestInfoVisitor()
{
    delete m_foundedPoints;
}

template<typename FoundedCloudIndex>
void GlobalPointsTestInfoVisitor<FoundedCloudIndex>::setCurrentInformation(const ItemModel *model, const PermanentItemScene *scene)
{
    Q_UNUSED(model)

    m_infoCloud = scene->getPointCloudAttributesProvider()->getInfoCloud();
}

template<typename FoundedCloudIndex>
void GlobalPointsTestInfoVisitor<FoundedCloudIndex>::addPointsToCompute(const Eigen::Vector3d &offset, std::vector<GLuint> &indices, const size_t &firstPointGlobalIndex)
{
    Q_UNUSED(offset)

    if((m_infoCloud != nullptr)
            && (m_testFunction != nullptr)) {

        std::vector<GLuint>::const_iterator it = indices.begin();
        std::vector<GLuint>::const_iterator end = indices.end();

        ElementInfo* firstSelectInfo = &(*m_infoCloud)[firstPointGlobalIndex];

        while(it != end) {
            if(m_testFunction(firstSelectInfo[*it]))
                m_foundedPoints->addIndex(firstPointGlobalIndex + (*it));

            ++it;
        }
    }
}

template<typename FoundedCloudIndex>
void GlobalPointsTestInfoVisitor<FoundedCloudIndex>::setFunction(testFunction f)
{
    m_testFunction = f;
}

template<typename FoundedCloudIndex>
FoundedCloudIndex* GlobalPointsTestInfoVisitor<FoundedCloudIndex>::getFoundedPoints() const
{
    return m_foundedPoints;
}

template<typename FoundedCloudIndex>
FoundedCloudIndex* GlobalPointsTestInfoVisitor<FoundedCloudIndex>::takeFoundedPoints()
{
    FoundedCloudIndex* s = m_foundedPoints;
    m_foundedPoints = new FoundedCloudIndex();

    return s;
}

#endif // GLOBALPOINTSTESTINFOVISITOR_HPP
