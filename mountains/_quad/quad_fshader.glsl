#version 330 core

in vec2 uv;

out vec3 color;

float rand (vec2 co){
    return fract(sin(dot(co.xy, vec2(12.9898,78.233))) * 43758.5453);
}

vec2 rand2(vec2 co){
	vec2[8] g = vec2[](vec2(1,1), vec2(-1,1), vec2(1,-1), vec2(-1,-1),
					vec2(1,0), vec2(-1,0), vec2(1,0), vec2(-1,0));
    int r = int(floor(rand(co)*8));
    return g[r];
}

float fade (float x){
	float x2 = x * x;
	float x3 = x2 * x;
	return 6 * x3 * x2 - 15 * x2 * x2 + 10 * x3;
}

float inoise(vec2 pos) {
    vec2 P = pos;
    P.x = P.x > 0.0 ? int(P.x) : int (P.x - 1);	
    P.y = P.y > 0.0 ? int(P.y) : int (P.y - 1);	
	vec2 v00 = P;
	vec2 v01 = P + vec2 (0, 1);
	vec2 v10 = P + vec2 (1, 0);
	vec2 v11 = P + vec2 (1, 1);
	
    vec2 g00 = rand2 (v00);
    vec2 g01 = rand2 (v01);
    vec2 g10 = rand2 (v10);
    vec2 g11 = rand2 (v11);

    vec2 a = pos - v00;
    vec2 b = pos - v10;
    vec2 c = pos - v01;
    vec2 d = pos - v11;
    
    float s = dot (g00, a) * 0.5 + 0.5;
    float t = dot (g10, b) * 0.5 + 0.5;
    float u = dot (g01, c) * 0.5 + 0.5;
    float v = dot (g11, d) * 0.5 + 0.5;
    
    float s_t = mix (s, t, fade (a.x));
    float u_v = mix (u, v, fade (a.x));
    float noise = mix (s_t, u_v, fade (a.y));
    
    return noise;
}

void main() {
	float noise = (inoise(uv*30) + inoise(uv*10) + inoise(uv*5) + inoise(uv*20) + inoise(uv*100))/5;
	color = vec3(noise, noise, noise);
}
