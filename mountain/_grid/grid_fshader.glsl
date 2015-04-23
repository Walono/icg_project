#version 330 core


out vec3 color;
in vec2 uv;


highp float rand(vec2 co)
{
    highp float a = 12.9898;
    highp float b = 78.233;
    highp float c = 43758.5453;
    highp float dt= dot(co.xy ,vec2(a,b));
    highp float sn= mod(dt,3.14);
    return fract(sin(sn) * c);
}


void main() {
	float rand = rand(uv.xy);
    color = vec3(rand, rand, rand);
}
