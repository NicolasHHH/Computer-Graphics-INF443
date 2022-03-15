#version 330 core  // OpenGL 3.3

// The output variable of the fragment is the color (R,G,B,A)
out vec4 FragColor;
uniform vec3 clr;

void main()
{
    FragColor = vec4(clr[0], clr[1], clr[2], 1.0); // couleur jaune

}