#version 330 core
out vec3 color;
in vec2 uv;
in vec3 newPos;

void main() {

	float pond = (newPos.y + 0.3);
	color = vec3(0.1 * pond, 1.f * pond, 0.1f * pond);
    
}
