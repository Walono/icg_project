// Copyright (C) 2014 - LGG EPFL
#version 330 core
uniform vec3 Ia, Id, Is;
uniform vec3 ka, kd, ks;
uniform float p;

uniform sampler1D tex1D;

in vec3 normal_mv;
in vec3 light_dir;
in vec3 view_dir;

out vec3 color;

void main() {
    //color = vec3(0.0,0.0,0.0);

    ///>>>>>>>>>> TODO >>>>>>>>>>>
    /// TODO 2.2: Toon shading.
    /// 1) compute ambient term.
    /// 2) compute diffuse term using the texture sampler tex.
    /// 3) compute specular term using the texture sampler tex.
    ///<<<<<<<<<< TODO <<<<<<<<<<<
    
    vec3 mirror_reflex = normalize(reflect(-light_dir, normal_mv));
    
    vec3 ambiant = Ia * ka;
    float NL = max(dot(normal_mv, light_dir), 0);
    float textureNL = texture1D(tex1D,NL);
    vec3 diffuse = (Id * kd) * max(textureNL, 0);
    float VR = max(dot(view_dir, mirror_reflex), 0);
    float textureVR = texture1D(tex1D,VR);
    vec3 specular = (Is * ks) * pow(textureVR,p);
    color = ambiant + diffuse + specular;
}
