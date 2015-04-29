#version 330 core

uniform mat4 mvp;

uniform sampler2D heightmap;

in vec2 position;
out vec2 uv;
out vec3 newPos;

void main() {
    uv = (position + vec2(1.0, 1.0)) * 0.5;
	
    float height = texture(heightmap, uv).x;
    vec3 pos_3d = vec3(position.x, height, position.y);

    gl_Position = mvp * vec4(pos_3d, 1.0);
	newPos = pos_3d.xyz;
}
