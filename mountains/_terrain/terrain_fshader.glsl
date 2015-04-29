#version 330 core
out vec3 color;
in vec2 uv;
in vec3 newPos;

void main() {

	float pond = (newPos.y + 0.3);

	vec3 green =  vec3(0.1 * pond, 1.f * pond, 0.1f * pond);
	vec3 white =  vec3(1.f * pond, 1.f * pond, 1.f * pond);
	vec3 yellow = vec3(1.f * pond, 1.f * pond, 0.1f * pond);


	if (newPos.y > 0.65) {
		color = white;
	}
	else if (newPos.y > 0.55 && newPos.y < 0.65){
		color = mix(green, white, 0.2);
	}
	else if (newPos.y < 0.375) {
	color = yellow;
	}
	else {
		color = green;
    }
}
