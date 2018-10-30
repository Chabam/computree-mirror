#ifndef GENERICCLOUDMODE_H
#define GENERICCLOUDMODE_H

namespace AMKgl {
    enum GenericCloudMode {
        GCM_Vertex,         // colors, normals, etc... is set to vertex (a face will be colored by vertex)
        GCM_Object          // colors, normals, etc... is set to object (a face will be colored by face)
    };
}

#endif // GENERICCLOUDMODE_H
