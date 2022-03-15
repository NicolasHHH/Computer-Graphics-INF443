#version 330 core  // OpenGL 3.3
//GLSL 3.30

layout (location = 0) in vec3 position; // Only one argument here: the vertex position
// le premier param est reçu dans position

uniform vec3 translation;
uniform mat3 rotation;

void main()
{
    // vec3 p = position + translation;
    vec3 p = rotation * position;

    // position vec3 ==> gl_Position vec4
    // Output position (in normalized device coordinates) is directly the position
    // gl_Position = vec4(position, 2.0); // homothésie, divisé par 2
    // gl_Position = vec4(position, 1.0)*2.0; // aucun effet, même chose après normalisation
    gl_Position = vec4(p, 1.0);
    // gl_Position.xyz *= 2.0; // grandir;
}