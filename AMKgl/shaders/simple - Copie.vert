#version 120

/////// COORDINATE SYSTEM //////

// coordinate system
uniform vec3 cs;

// the matrix of the camera (without offset)
uniform mat4 camMatrix;

///// COLOR //////

// true if must use frontColor attribute, false if must use gl_Color
uniform bool useColorAttribute;

// color
attribute vec4 frontColor;

///// SELECTION /////

// color of selected points
uniform vec4 selectionColor;

void main()
{
    if(useColorAttribute) {
        gl_FrontColor = frontColor;
    } else {
        gl_FrontColor = gl_Color;
    }

    gl_Position = gl_ModelViewProjectionMatrix * camMatrix * (gl_Vertex + vec4(cs, 0));
}
