#version 460 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aUV;
layout(location = 3) in float aTexID;

uniform mat4 uProjection;

out vec4 vColor;
out vec2 vUV;
flat out int vTexID;

void main()
{
    gl_Position = uProjection * vec4(aPos, 0.0, 1.0);
    vColor = aColor;
    vUV = aUV;
    vTexID = int(aTexID);
}