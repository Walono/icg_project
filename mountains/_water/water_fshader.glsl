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

const float transparency = 0.5f;


void main() {
	float height = texture(heightmap, uv).x;

	vec3 color_tex = vec3(0.2f, 0.2f, 0.7f);
	
	
	vec3 mirror_reflex = normalize(reflect(-light_dir, normal_mv));
    vec3 ambiant = Ia * (color_tex*0.3);
    float NL = max(dot(normal_mv, light_dir), 0);
    vec3 diffuse = (Id * color_tex) * NL;
    float VR = max(dot(view_dir, mirror_reflex), 0);
    vec3 specular = (Is * (color_tex*1.3)) * pow(VR, p);
    color_tex = ambiant+diffuse+specular;
    color = vec4(color_tex, transparency);
	
}
