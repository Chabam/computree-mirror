
///// VERTEX //////

// vertex
attribute vec3 vertex;

///// COLOR //////

// color
attribute vec4 color;

// to true if we must use frontColor attribute
uniform bool useFrontColor;

void main()
{
    if(prepareToComputeVertex(color, useFrontColor)) {
        gl_Position = gl_ModelViewProjectionMatrix * vec4(vertex, 1);
    }
}
