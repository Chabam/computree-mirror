
///// VERTEX //////

// vertex
attribute vec3 vertex;

///// NORMAL ////

// normal
attribute vec4 normal;

// length
uniform float normalLength;

// color
uniform vec4 normalColor;

// normal that was send to geometry shader
varying vec4 geomNormal;

// normal color that was send to geometry shader
varying vec4 geomColor;

void main()
{
    if(isVisible(info)) {
        int ffe = firstFlagEnable(info);

        if(ffe != 8) {
            geomColor = flagsColors[ffe];
        } else {
            geomColor = normalColor;
        }

        vec4 newVertex = vec4(vertex, 1);
        gl_Position = gl_ModelViewProjectionMatrix * newVertex;

        vec4 newNormal = newVertex + (normal * normalLength);
        newNormal[3] = 1;

        geomNormal = gl_ModelViewProjectionMatrix * newNormal;
    } else {
        gl_Position = vec4(2.0, 2.0, 2.0, 1.0);
    }
}
