#version 330 core
out vec4 color;
in vec2 uv;
in vec3 newPos;
uniform sampler2D grass;
uniform sampler2D snow;
uniform sampler2D rock;

void main() {
	float pond = (newPos.y + 0.3);

	vec4 grassVec = texture(grass, uv);
	vec4 snowVec = texture(snow, uv);
	vec4  rockVec = texture(rock, uv);


	if (newPos.y > 0.65) {
		color = snowVec;
	}
	else if (newPos.y > 0.55 && newPos.y < 0.65){
		color = mix(grassVec, snowVec, 0.2);
	}
	else if (newPos.y < 0.375) {
	color = rockVec;
	}
	else {
		color = grassVec;
    }
}
