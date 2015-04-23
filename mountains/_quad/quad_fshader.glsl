#version 330 core
out vec3 color;
in vec2 uv;
uniform sampler2D tex;

float gridSize = 10;

highp float rand(vec2 co)
{
    highp float a = 12.9898;
    highp float b = 78.233;
    highp float c = 43758.5453;
    highp float dt= dot(co.xy ,vec2(a,b));
    highp float sn= mod(dt,3.14);
    return fract(sin(sn) * c);
}

highp vec2 rand2(vec2 co){
	vec2 co2 = vec2(co.x+co.y,co.x-co.y);
	 return vec2(rand(co), rand(co2));
}
   
   
vec2 toFragCoord(vec2 v){
	return vec2(v.x / gridSize, v.y / gridSize);
   }
   
float smoothf(float x){
	float x2 = x * x;
	float x3 = x2 * x;
	return 6 * x3 * x2 - 15 * x2 * x2 + 10 * x3;
}

float mixNumber(float x, float y, float alpha){
	return (1-alpha)*x + (alpha)*y;
}

void main() {
	vec2 pos = uv;
	///TODO perlin noise
	
	
	
	//vec2 pos = vec2(gl_PointCoord.x, gl_PointCoord.y);
	int x0 = int(floor(pos.x * gridSize));
	int x1 = x0+1;
	int y0 = int(floor(pos.y * gridSize));
	int y1 = y0+1;
	
	vec2 x0y0 = toFragCoord(vec2(x0, y0));
	vec2 x0y1 = toFragCoord(vec2(x0, y1));
	vec2 x1y0 = toFragCoord(vec2(x1, y0));
	vec2 x1y1 = toFragCoord(vec2(x1, y1));
	
	

    vec2 g00 = normalize(rand2(x0y0));
    vec2 g10 = normalize(rand2(x1y0));
    vec2 g01 = normalize(rand2(x0y1));
    vec2 g11 = normalize(rand2(x1y1));
  /*
    vec2 g00 = normalize(vec2(10,1));
    vec2 g10 = normalize(vec2(10,1));
    vec2 g01 = normalize(vec2(10,1));
    vec2 g11 = normalize(vec2(10,1));*/
    
    vec2 a = pos-x0y0;
    vec2 b = pos-x1y0;
    vec2 c = pos-x0y1;
    vec2 d = pos-x1y1;
    
 
    float s = dot(g00,a);
    float t = dot(g10,b);
    float u = dot(g01,c);
    float v = dot(g11,d);
    
    
    float s_t = mixNumber(s,t,smoothf(fract(pos.x*gridSize)));
    float u_v = mixNumber(u,v,smoothf(fract(pos.x*gridSize)));
    float noise = mixNumber(s_t,u_v,smoothf(fract(pos.y*gridSize)));
    
    /*vec2 posg = vec2(pos.x*gridSize,pos.y*gridSize);
    vec2 x0y0g = vec2(x0, y0);
	vec2 x0y1g = vec2(x0, y1);
	vec2 x1y0g = vec2(x1, y0);
	vec2 x1y1g = vec2(x1, y1);
	vec2 g00g = normalize(rand2(x0y0g));
    vec2 g10g = normalize(rand2(x1y0g));
    vec2 g01g = normalize(rand2(x0y1g));
    vec2 g11g = normalize(rand2(x1y1g));
    vec2 ag = posg-x0y0g;
    vec2 bg = posg-x1y0g;
    vec2 cg = posg-x0y1g;
    vec2 dg = posg-x1y1g;
    float sg = dot(g00g,ag);
    float tg = dot(g10g,bg);
    float ug = dot(g01g,cg);
    float vg = dot(g11g,dg);
	float s_tg = mixNumber(sg,tg,smoothf(fract(posg.x)));
    float u_vg = mixNumber(u,v,smoothf(fract(posg.x)));
    float noiseg = mixNumber(s_t,u_v,smoothf(fract(posg.y)));*/
    
    color = vec3(noise, noise, noise)*30;
}
