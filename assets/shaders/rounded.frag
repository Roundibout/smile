#version 460 core

in vec2 vRectSize;
in vec4 vColor;
in vec4 vCorner;
in vec2 vLocalPos;

out vec4 FragColor;

void main() {
    float alpha = 1.0;

    // bottom-left corner
    if (vLocalPos.x < vCorner.x && vLocalPos.y < vCorner.x) {
        vec2 center = vec2(vCorner.x, vCorner.x);
        float dist = length(vLocalPos - center);
        float aa = 1.0;
        alpha = 1.0 - smoothstep(vCorner.x - aa, vCorner.x + aa, dist);
    }

    // bottom-right corner
    else if (vLocalPos.x > vRectSize.x - vCorner.y && vLocalPos.y < vCorner.y) {
        vec2 center = vec2(vRectSize.x - vCorner.y, vCorner.y);
        float dist = length(vLocalPos - center);
        float aa = 1.0;
        alpha = 1.0 - smoothstep(vCorner.y - aa, vCorner.y + aa, dist);
    }

    // top-right corner
    else if (vLocalPos.x > vRectSize.x - vCorner.z && vLocalPos.y > vRectSize.y - vCorner.z) {
        vec2 center = vec2(vRectSize.x - vCorner.z, vRectSize.y - vCorner.z);
        float dist = length(vLocalPos - center);
        float aa = 1.0;
        alpha = 1.0 - smoothstep(vCorner.z - aa, vCorner.z + aa, dist);
    }

    // top-left corner
    else if (vLocalPos.x < vCorner.w && vLocalPos.y > vRectSize.y - vCorner.w) {
        vec2 center = vec2(vCorner.w, vRectSize.y - vCorner.w);
        float dist = length(vLocalPos - center);
        float aa = 1.0;
        alpha = 1.0 - smoothstep(vCorner.w - aa, vCorner.w + aa, dist);
    }

    // Elsewhere, inside the straight edges, alpha stays 1.0

    FragColor = vec4(vColor.rgb, vColor.a * alpha);
}