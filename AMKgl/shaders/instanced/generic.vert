///// VERTEX //////

// vertex
attribute vec3 vertex;

///// COLOR //////

// color
attribute vec4 color;

///// MATRIX /////

// matrix
attribute mat4 params;

void main()
{
    if(prepareToComputeVertex(color, true)) {

        mat4 newMatrix = params;
        newMatrix[0][3] = 0;
        newMatrix[1][3] = 0;
        newMatrix[2][3] = 0;
        newMatrix[3][3] = 1;

        gl_Position = gl_ModelViewProjectionMatrix * newMatrix * vec4(vertex, 1);
    }
}
