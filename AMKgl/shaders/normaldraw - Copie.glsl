#version 150

layout(points) in;
layout(line_strip, max_vertices = 2) out;

// normal from vertex shader
in vec4 geomNormal[];

void main() {
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    gl_Position = geomNormal[0];
    EmitVertex();

    EndPrimitive();
}
