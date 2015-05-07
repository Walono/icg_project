// Copyright (C) 2014 - LGG EPFL
#version 330 core
uniform vec3 Ia, Id, Is;
uniform vec3 ka, kd, ks;
uniform float p;

in vec4 vpoint_mv;
in vec3 light_dir, view_dir;

out vec3 color;

void main() {
    color = vec3(0.0,0.0,0.0);
    const vec3 COLORS[6] = vec3[](
        vec3(1.0,0.0,0.0),
        vec3(0.0,1.0,0.0),
        vec3(0.0,0.0,1.0),
        vec3(1.0,1.0,0.0),
        vec3(0.0,1.0,1.0),
        vec3(1.0,0.0,1.0));
    int index = int( mod(gl_PrimitiveID,6) );
    color = COLORS[index];

    ///>>>>>>>>>> TODO >>>>>>>>>>>
    /// TODO 4.2: Flat shading.
    /// 1) compute triangle normal using dFdx and dFdy
    /// 1) compute ambient term.
    /// 2) compute diffuse term.
    /// 3) compute specular term.
    ///<<<<<<<<<< TODO <<<<<<<<<<<
    
    vec3 X = dFdx(vpoint_mv.xyz);
	vec3 Y = dFdy(vpoint_mv.xyz);
	vec3 normal=normalize(cross(X,Y));
	vec3 mirror_reflex = normalize(reflect(-light_dir, normal));
	
	vec3 ambiant = Ia * ka;
    float NL = max(dot(normal, light_dir), 0);
    vec3 diffuse = (Id * kd) * NL;
    float VR = max(dot(view_dir, mirror_reflex), 0);
    vec3 specular = (Is * ks) * pow(VR, p);
    color = ambiant + diffuse + specular;
}
