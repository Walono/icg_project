#version 330 core
const float pi = 3.14159f;
uniform mat4 mvp;

uniform sampler2D heightmap;

uniform float time;
in vec2 position;
out vec2 uv;
out vec3 newPos;
out float seaLvl;

in float waveTime;
in float waveHeight;
in float waveLength;

float newTime;

float funcWaveHeight(float x, float y) {


	float waveHeight = (sin((time*0.8+x*16+y*10)*8)+cos((time+16*x+16*y)*6)+sin((time*1.1+16*x+14*y)*6)+cos((time+20*x+8*y)*6))/500;

	return waveHeight;
	
}

void main() {
	seaLvl = 0.4f;

    uv = (position + vec2(1.0, 1.0)) * 0.5;
	
    float height = texture(heightmap, uv).x;


		float wave = funcWaveHeight(uv.x, uv.y);
		height = seaLvl + wave;

	
    vec3 pos_3d = vec3(position.x, height, position.y);

    gl_Position = mvp * vec4(pos_3d, 1.0);
	newPos = pos_3d.xyz;
}
