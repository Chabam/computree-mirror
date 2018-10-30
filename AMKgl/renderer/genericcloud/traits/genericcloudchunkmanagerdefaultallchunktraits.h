#ifndef GENERICCLOUDCHUNKMANAGERDEFAULTALLCHUNKTRAITS_H
#define GENERICCLOUDCHUNKMANAGERDEFAULTALLCHUNKTRAITS_H

#include "genericchunkmanagerdefaultinitchunktraits.h"
#include "genericcloudchunkmanagerdefaultcreatechunktraits.h"

template<typename SuperClassT>
class GenericCloudChunkManagerDefaultAllChunkTraits : public GenericChunkManagerDefaultInitChunkTraits< GenericCloudChunkManagerDefaultCreateChunkTraits< SuperClassT > >
{
public:
    GenericCloudChunkManagerDefaultAllChunkTraits(GLenum glMode,
                                                     const quint8& nVertexPerObject,
                                                     const size_t& beginGlobalIndex,
                                                     const size_t& cloudSize) : GenericChunkManagerDefaultInitChunkTraits< GenericCloudChunkManagerDefaultCreateChunkTraits< SuperClassT > >(
                                                                                    glMode,
                                                                                    nVertexPerObject,
                                                                                    beginGlobalIndex,
                                                                                    cloudSize) {}
};

#endif // GENERICCLOUDCHUNKMANAGERDEFAULTALLCHUNKTRAITS_H
