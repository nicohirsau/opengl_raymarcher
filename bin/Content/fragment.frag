#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform float u_time;
uniform vec2 u_mPos;
uniform sampler2D u_texture;

#define M_PI 3.1415926535897932384626433832795

void main() {
    vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
    vec3 pos = vec3(TexCoord.x, TexCoord.y, 0.0);
    vec3 dir = normalize(vec3(
        TexCoord.x / 2.0,
        TexCoord.y / 2.0,
        1
    ));
    bool hit = false;
    float steps = 0;
    float dstToOrig = 0.0;
    float max_steps = 10.0;
    vec3 sphere_pos = vec3(u_mPos.x, u_mPos.y, 1.5);

    while(dstToOrig < 999) {
        float dst = length(
            vec3(pos.x, pos.y, pos.z) - sphere_pos
        ) - 0.75;
        
        if (dst <= 0.001) {
            vec3 d = normalize(sphere_pos - pos);
            float u = 0.5 + atan(d.z, d.x) / M_PI + u_time * 0.2;
            float v = 0.5 - asin(d.y) / M_PI;
            color = texture(u_texture, vec2(u, v)) * vec4(1.0);
            color *= 1 - steps / 45.0;
            color.w = 1.0;
            hit = true;
            break;
        }

        pos += dir * dst; 
        dstToOrig += dst;   
        steps++;
    }

    if (!hit) {
        float background_shading = 1 * (steps / max_steps);
        color = vec4(
            background_shading * (3 / 255.0), 
            background_shading * (33 / 255.0), 
            background_shading * (74 / 255.0), 
            1.0
        );
    }

    FragColor = color;
}
