#version 460 core

layout(location = 0) in vec2 aUnitPos;
layout(location = 1) in vec2 aRectPos;
layout(location = 2) in vec2 aRectSize;
layout(location = 3) in vec4 aColor;
layout(location = 4) in float aCorner;

uniform mat4 uProjection;

out vec2 vRectSize;
out vec4 vColor;
out float vCorner;
out vec2 vLocalPos;

void main() {
    // Convert from unit quad into actual rect coords
    vec2 worldPos = aRectPos + aUnitPos * aRectSize;

    gl_Position = uProjection * vec4(worldPos, 0.0, 1.0);

    vRectSize = aRectSize;
    vColor = aColor;
    vCorner = aCorner;
    vLocalPos = aUnitPos * aRectSize;
}