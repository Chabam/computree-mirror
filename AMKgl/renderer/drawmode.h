#ifndef DRAWMODE_H
#define DRAWMODE_H

enum DrawMode {
    DM_NONE = 0,            // Draw none
    DM_BASIC = 1,           // Draw with glVertex, glColor, etc....
    DM_DISPLAY_LIST = 5,    // Draw with glVertex, glColor, etc.... but use glNewList/glCallList
    DM_RAW = 2,             // Draw with shader and attribute array
    DM_VBO = 3,             // Draw with shader and VBO
    DM_VAO = 4              // Draw with shader, VBO and VAO
};

#endif // DRAWMODE_H
