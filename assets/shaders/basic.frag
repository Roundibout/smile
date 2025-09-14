#version 460 core

in vec4 vColor; // From vertex shader

out vec4 FragColor; // Output color of this fragment

void main()
{
    FragColor = vColor;
}