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

    float alpha;

    if ((vLocalPos.x >= vCorner && vLocalPos.x <= vRectSize.x - vCorner) ||
        (vLocalPos.y >= vCorner && vLocalPos.y <= vRectSize.y - vCorner)) {
        // Straight edge
        alpha = 1.0;
    } else {
        // Rounded corner
        float distToCorner = length(vLocalPos - cornerCenter);
        float aa = 1.0;
        alpha = 1.0 - smoothstep(vCorner - aa, vCorner + aa, distToCorner);
    }

    // Multiply by color alpha
    FragColor = vec4(vColor.rgb, vColor.a * alpha);
}