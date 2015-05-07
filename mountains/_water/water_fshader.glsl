#version 330 core
out vec4 color;
in vec2 uv;
in vec3 newPos;
in float seaLvl;

in vec3 normal_mv;
in vec3 light_dir;
in vec3 view_dir;

uniform vec3 Ia, Id, Is;
uniform float p;

uniform sampler2D heightmap;

const float transparency = 0.75f;


void main() {
	float height = texture(heightmap, uv).x;
	if(height > seaLvl + 0.005){
		color = vec4(0.2f, 0.2f, 0.f, 0.0f);
	} else{
		color = vec4(0.2f, 0.2f, 0.7f, transparency);
	}
	
	vec4 mirror_reflex = vec4(normalize(reflect(-light_dir, normal_mv)),transparency);
    vec4 ambiant = vec4(Ia * (color*0.3),transparency);
    float NL = max(dot(normal_mv, light_dir), 0);
    vec4 diffuse = vec4((Id * color) * NL,transparency);
    float VR = max(dot(view_dir, mirror_reflex), 0);
    vec4 specular = vec4((Is * (color*1.3)) * pow(VR, p),transparency);
    color = ambiant+diffuse+specular;
	
}
