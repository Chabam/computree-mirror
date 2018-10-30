///// VERTEX //////

// vertex
attribute vec3 vertex;

///// COLOR //////

// color
attribute vec4 frontColor;

// to true if we must use frontColor attribute
uniform bool useFrontColor;

void main()
{
    if(prepareToComputeVertex(frontColor, useFrontColor)) {
        gl_Position = gl_ModelViewProjectionMatrix * vec4(vertex, 1);
    }
}
