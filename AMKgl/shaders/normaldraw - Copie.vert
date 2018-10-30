#version 120

///// NORMAL ////
// true if must use normal attribute, false if must use gl_Normal
uniform bool useNormalAttribute;

// normal
attribute vec4 normal;

// length
uniform float normalLength;

// normal that was send to geometry shader
varying out vec4 geomNormal;

/////// COORDINATE SYSTEM //////

// coordinate system
uniform vec3 cs;

// the matrix of the camera (without offset)
uniform mat4 camMatrix;

///// SELECTION /////

// color of selected points
uniform vec4 selectionColor;

void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * camMatrix * (gl_Vertex + vec4(cs, 0));

    vec4 newNormal = gl_Vertex;

    if(useNormalAttribute) {
        newNormal = normal;
        newNormal[3] = 0;
    } else {
        newNormal = vec4(gl_Normal, 0);
    }

    newNormal = gl_Vertex + vec4(cs, 0) + (newNormal * normalLength);
    newNormal[3] = 1;

    geomNormal = gl_ModelViewProjectionMatrix * camMatrix * newNormal;
}
