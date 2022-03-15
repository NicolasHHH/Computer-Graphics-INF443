#version 330 core  // OpenGL 3.3
//GLSL 3.30

layout (location = 0) in vec3 position; // Only one argument here: the vertex position
// le premier param est reçu dans position

void main()
{
    // position vec3 ==> gl_Position vec4
    // Output position (in normalized device coordinates) is directly the position
    gl_Position = vec4(position, 1.0);
}