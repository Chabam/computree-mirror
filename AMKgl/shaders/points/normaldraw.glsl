#version 150

layout(points) in;
layout(line_strip, max_vertices = 2) out;

// normal from vertex shader
in vec4 geomNormal[];

// normal color from vertex shader
in vec4 geomColor[];

// normal color that was send to frag shader
out vec4 fragColor;

void main() {
    gl_Position = gl_in[0].gl_Position;
    fragColor = geomColor[0];
    EmitVertex();

    gl_Position = geomNormal[0];
    fragColor = geomColor[0];
    EmitVertex();

    EndPrimitive();
}
