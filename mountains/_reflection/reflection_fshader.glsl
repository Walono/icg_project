#version 330 core


uniform vec3 Ia, Id, Is;
uniform float p;

out vec3 color;
in vec2 uv;
in vec3 newPos;

in vec3 normal_mv;
in vec3 light_dir;
in vec3 view_dir;

uniform sampler2D grass;
uniform sampler2D snow;
uniform sampler2D rock;
uniform sampler2D grassSnow;
uniform sampler2D mixTexTerrain;
uniform sampler2D mixSnowGrass;
uniform sampler2D heightmap;



float rand(vec2 co){
    return fract(sin(dot(co.xy, vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
	float pond = (newPos.y + 0.3);
	float altNeige = 0.6f;
	float transitionNeigeSize = 0.1f;
	

	vec3 grassVec = texture(grass, uv*10).xyz;
	vec3 snowVec = texture(snow, uv).xyz;
	vec3  rockVec = texture(rock, uv).xyz;
	float mixRock = texture(mixTexTerrain, uv).x;
	vec3 grassSnowVec = texture(grassSnow, uv).xyz;
	float snowGrassMix = texture(grassSnow, uv).x;
	
	color = mix(grassVec, rockVec, mixRock);

	if(newPos.y>altNeige-transitionNeigeSize){
		float relativeY = (newPos.y-(altNeige-transitionNeigeSize))/(transitionNeigeSize);
		if(snowGrassMix-0.5>0){
			color = mix(color, mix(grassVec, snowVec, 0.9), relativeY);
		}
		else if(newPos.y>altNeige){
			float  relativeY2 = (newPos.y-altNeige)/(transitionNeigeSize);
			color = mix(color, mix(grassVec, snowVec, 0.9), relativeY2*3);
		}
			
	}
	/// calculate normale
	vec2 position = vec2(newPos.x, newPos.y);
	
	/*vec3 pointFond = vec3(newPos.x, texture(heightmap,((position +vec2(0.0,(1/1023.0f))+ vec2(1.0, 1.0)) * 0.5)).x , newPos.z+(1/1023.0f));
	vec3 vecFond = pointFond-newPos;
	
	vec3 pointDevant = vec3(newPos.x, texture(heightmap, ((position -vec2(0.0,(1/1023.0f))+ vec2(1.0, 1.0)) * 0.5)).x, newPos.z-(1/1023.0f));
	vec3 vecDevant = pointDevant-newPos;
	
	vec3 pointDroite = vec3(newPos.x+(1/1023.0f), texture(heightmap, ((position +vec2((1/1023.0f), 0.0)+ vec2(1.0, 1.0)) * 0.5)).x, newPos.z);
	vec3 vecDroite = pointDroite-newPos;
	
	vec3 pointGauche = vec3(newPos.x-(1/1023.0f), texture(heightmap, ((position -vec2((1/1023.0f), 0.0)+ vec2(1.0, 1.0)) * 0.5)).x, newPos.z);
	vec3 vecGauche = pointGauche-newPos;
	
	vec3 v1 = cross(vecGauche, vecFond);
	vec3 v2 = cross(vecFond, vecDroite);
	vec3 v3 = cross(vecDroite, vecDevant);
	vec3 v4 = cross(vecDevant, vecGauche);
	
	vec3 normal_mv = normalize(v1+v2+v3+v4);*/
	

	
	//normal_mv = (1,1,1);
	
	//calculate shading
	vec3 mirror_reflex = normalize(reflect(-light_dir, normal_mv));
    vec3 ambiant = Ia * color;
    float NL = max(dot(normal_mv, light_dir), 0);
    vec3 diffuse = (Id * color) * NL;
    float VR = max(dot(view_dir, mirror_reflex), 0);
    vec3 specular = (Is * color) * pow(VR, p);
    color = diffuse;
    //color = ambiant;
	
	
	
/*
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
    }*/
}
