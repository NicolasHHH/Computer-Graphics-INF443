#version 330 core  // OpenGL 3.3

in vec3 clr;
// The output variable of the fragment is the color (R,G,B,A)
out vec4 FragColor;

void main()
{
    FragColor = vec4(clr, 1.0);
}