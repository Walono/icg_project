#version 330 core
out vec4 color;
in vec2 uv;
in vec3 newPos;
in float seaLvl;
uniform sampler2D heightmap;


void main() {
	float height = texture(heightmap, uv).x;
	if(height > seaLvl + 0.025f){
		color = vec4(0.2f, 0.2f, 0.7f, 0.f);
	} else{
		color = vec4(0.2f, 0.2f, 0.7f, 0.75f);
	}
	
}
