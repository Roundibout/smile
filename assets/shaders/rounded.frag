#version 460 core

in vec2 vRectSize;
in vec4 vColor;
in float vCorner;
in vec2 vLocalPos;

out vec4 FragColor;

void main() {
    // vLocalPos is the pixel relative to bottom-left corner
    vec2 cornerCenter = vec2(
        (vLocalPos.x < vCorner) ? vCorner : (vLocalPos.x > vRectSize.x - vCorner) ? vRectSize.x - vCorner : vLocalPos.x,
        (vLocalPos.y < vCorner) ? vCorner : (vLocalPos.y > vRectSize.y - vCorner) ? vRectSize.y - vCorner : vLocalPos.y
    );

    // distance from pixel to corner center
    float distToCorner = length(vLocalPos - cornerCenter);

    // smooth alpha over 1–2 pixels (adjust for desired softness)
    float aa = 1.0; // 1 pixel smoothstep
    float alpha = smoothstep(vCorner, vCorner - aa, distToCorner);

    // multiply by color alpha
    FragColor = vec4(vColor.rgb, vColor.a * alpha);
}