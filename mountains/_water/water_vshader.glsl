#version 330 core
const float pi = 3.14159f;
uniform mat4 P;
uniform mat4 MV;

uniform sampler2D heightmap;
uniform float time;
uniform vec3 light_pos;
in vec2 position;

out vec2 uv;
out vec3 newPos;
out float seaLvl;

out vec3 normal_mv;
out vec3 light_dir;
out vec3 view_dir;

float wave(float x, float y) {
	float height = texture(heightmap, uv).x;
	float amplitude;
	if((seaLvl - height) < 0.025){
		amplitude = 50 / (seaLvl - height);
	} else {
		amplitude = 500;
	}	
	return (sin((time*0.8+x*16+y*10)*8)+cos((time+16*x+16*y)*6)+sin((time*1.1+16*x+14*y)*6)+cos((time+20*x+8*y)*6))/amplitude;
}


void main() {
	seaLvl = 0.45f;
	mat4 mvp = P*MV;
    uv = (position + vec2(1.0, 1.0)) * 0.5;
   

	float height = seaLvl + wave(uv.x, uv.y);
	
    vec3 pos_3d = vec3(position.x, height, position.y);
    vec4 vpoint_mv = MV*vec4(pos_3d, 1.0);
    light_dir = normalize(light_pos - vpoint_mv.xyz);
	view_dir = normalize(-vpoint_mv.xyz);

	float delta = 1/128.0f; /// -> 1/128
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
	normal_mv = normalize((inverse(transpose(MV)) * vec4(vnormal, 1.0)).xyz);


    gl_Position = mvp * vec4(pos_3d, 1.0);
	newPos = pos_3d;
	
}
