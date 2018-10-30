#include "globalpointssetinfovisitor.h"

#include "tools/pointcloudattributesprovider.h"
#include "scene/permanentscene.h"

GlobalPointsSetInfoVisitor::GlobalPointsSetInfoVisitor()
{
    m_infoCloud = NULL;
    m_setFunction = nullptr;
}

void GlobalPointsSetInfoVisitor::setFunction(GlobalPointsSetInfoVisitor::functionForSet f)
{
    m_setFunction = f;
}

void GlobalPointsSetInfoVisitor::setCurrentInformation(const ItemModel *model, const PermanentItemScene *scene)
{
    Q_UNUSED(model)

    m_infoCloud = scene->getPointCloudAttributesProvider()->getInfoCloud();
}

void GlobalPointsSetInfoVisitor::addPointsToCompute(const Eigen::Vector3d &offset, std::vector<GLuint> &indices, const size_t &firstPointGlobalIndex)
{
    Q_UNUSED(offset)

    if((m_infoCloud != NULL)
            && (m_setFunction != nullptr)) {

        std::vector<GLuint>::const_iterator it = indices.begin();
        std::vector<GLuint>::const_iterator end = indices.end();

        ElementInfo* firstSelectInfo = &(*m_infoCloud)[firstPointGlobalIndex];

        while(it != end) {
            m_setFunction(firstSelectInfo[*it]);
            ++it;
        }
    }
}
