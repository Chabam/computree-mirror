#version 120

// normal color from vertex shader
in vec4 fragColor;

void main() {
    gl_FragColor = fragColor;
}
