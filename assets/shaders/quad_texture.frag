#version 460 core

in vec4 vColor;
in vec2 vUV;
flat in int vTexID;

uniform sampler2D uTextures[16]; // Texture slots

out vec4 FragColor;

void main()
{
    vec4 texColor = texture(uTextures[vTexID], vUV);
    FragColor = texColor * vColor;
}