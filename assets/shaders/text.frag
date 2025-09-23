#version 330 core

in vec2 vTexCoord;
in vec4 vColor;

uniform sampler2D uTexture;

out vec4 fragColor;

void main() {
    float alpha = texture(uTexture, vTexCoord).r;
    fragColor = vec4(vColor.rgb, vColor.a * alpha);
}