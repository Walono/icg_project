#version 330 core

uniform vec3 Ia, Id, Is;
uniform vec3 ka, kd, ks;
uniform float p;

out vec4 color;

in vec2 uv;
in vec3 newPos;

in vec3 normal_mv;
in vec3 light_dir;
in vec3 view_dir;

uniform sampler2D grass;
uniform sampler2D snow;
uniform sampler2D rock;

void main() {
	float pond = (newPos.y + 0.3);

	vec4 grassVec = texture(grass, uv);
	vec4 snowVec = texture(snow, uv);
	vec4  rockVec = texture(rock, uv);


	if (newPos.y > 0.65) {
		vec4 precolor = snowVec;
	}
	else if (newPos.y > 0.55 && newPos.y < 0.65){
		vec4 precolor = mix(grassVec, snowVec, 0.2);
	}
	else if (newPos.y < 0.375) {
		vec4 precolor = rockVec;
	}
	else {
		vec4 precolor = grassVec;
    }
	
	vec3 mirror_reflex = normalize(reflect(-light_dir, normal_mv));
    vec3 ambiant = Ia * ka;
    float NL = max(dot(normal_mv, light_dir), 0);
    vec3 diffuse = (Id * kd) * NL;
    float VR = max(dot(view_dir, mirror_reflex), 0);
    vec3 specular = (Is * ks) * pow(VR, p);
    vec3 phong = ambiant+diffuse+specular;
	precolor.x *= phong.x;
	precolor.y *= phong.y;
	precolor.z *= phong.z;
	color = precolor;
	
}
