
///// VERTEX //////

// vertex
attribute vec3 vertex;

///// COLOR //////

// color
attribute vec4 color;

///// MATRIX /////

// matrix
attribute mat3 params;

// current vertex's theta phi
attribute vec2 thetaPhi;

void main(void)
{
    if(prepareToComputeVertex(color, true)) {

        float startTheta = params[1][0];
        float endTheta = params[1][1];
        float startPhi = params[1][2];
        float endPhi = params[2][0];

        float radius = params[2][1];

        mat4 newMatrix = mat4(radius);

        newMatrix[3] = vec4(params[0], 1);
        newMatrix[3][3] = 1.0;

        vec4 newVertex = vec4(0, 0, 0, 1);

        bool ok;

        if(startTheta > endTheta)
        {
            ok = ((thetaPhi[0] >= 0) && (thetaPhi[0] <= startTheta)) || (thetaPhi[0] >= endTheta);
        }
        else
        {
            ok = (thetaPhi[0] >= startTheta) && (thetaPhi[0] <= endTheta);
        }

        if(ok)
        {
            if(startPhi > endPhi)
            {
                ok = ((thetaPhi[1] >= 0) && (thetaPhi[1] <= startPhi))|| (thetaPhi[1] >= endPhi);
            }
            else
            {
                ok = (thetaPhi[1] >= startPhi) && (thetaPhi[1] <= endPhi);
            }
        }

        if(ok)
        {
            newVertex = vec4(vertex, 1);
        }

        gl_Position = gl_ModelViewProjectionMatrix * newMatrix * newVertex;
    }
}

