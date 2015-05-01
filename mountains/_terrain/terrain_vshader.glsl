#version 330 core

uniform mat4 mvp;
uniform mat4 mv;
uniform vec3 light_pos;

uniform sampler2D heightmap;

in vec2 position;
out vec2 uv;
out vec3 newPos;

out vec3 normal_mv;
out vec3 light_dir;
out vec3 view_dir;

void main() {
    uv = (position + vec2(1.0, 1.0)) * 0.5;
	
    float height = texture(heightmap, uv).x;
    vec3 pos_3d = vec3(position.x, height, position.y);

    gl_Position = mvp * vec4(pos_3d, 1.0);
	newPos = pos_3d.xyz;

	float delta = 0.0078125; /// -> 1/128
	vec2 v0xz = vec2(position.x, position.y - delta);
	float h0 = texture(heightmap, (v0xz + vec2(1.0, 1.0)) * 0.5).x;
	vec3 v0 = vec3(v0xz.x, h0, v0xz.y);
	vec2 v1xz = vec2(position.x + delta, position.y);
	float h1 = texture(heightmap, (v1xz + vec2(1.0, 1.0)) * 0.5).x;
	vec3 v1 = vec3(v1xz.x, h1, v1xz.y);
	vec2 v2xz = vec2(position.x, position.y - delta);
	float h2 = texture(heightmap, (v2xz + vec2(1.0, 1.0)) * 0.5).x;
	vec3 v2 = vec3(v2xz.x, h2, v2xz.y);
	vec2 v3xz = vec2(position.x - delta, position.y);
	float h3 = texture(heightmap, (v3xz + vec2(1.0, 1.0)) * 0.5).x;
	vec3 v3 = vec3(v3xz.x, h3, v3xz.y);
	
	vec3 v01 = v1 * v0;
	vec3 v12 = v2 * v1;
	vec3 v23 = v3 * v2;
	vec3 v30 = v0 * v3;
	
	vec3 vnormal = normalize(v01 + v12 + v23 + v30);

    //vec4 vpoint_mv = mv * vec4(position, 1.0);
    //light_dir = normalize(light_pos - vpoint_mv.xyz);
    light_dir = normalize(light_pos - pos_3d);
	//view_dir = normalize(-vpoint_mv.xyz);
	//normal_mv = normalize((inverse(transpose(mv)) * vec4(vnormal, 1.0)).xyz);
	view_dir = -pos_3d;
	normal_mv = vnormal;
}
