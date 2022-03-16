#version 330 core  // OpenGL 3.3

layout (location = 0) in vec3 position; // Only one argument here: the vertex position
layout (location = 1) in vec3 color;

out vec3 clr;
void main()
{
    clr = color;
    // Output position (in normalized device coordinates) is directly the position
    gl_Position = vec4(position, 1.0);
}