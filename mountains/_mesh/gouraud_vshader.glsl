// Copyright (C) 2014 - LGG EPFL
#version 330 core
uniform vec3 Ia, Id, Is;
uniform vec3 ka, kd, ks;
uniform float p;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform vec3 light_pos;

in vec3 vpoint;
in vec3 vnormal;

out vec3 vcolor;


void main() {
    mat4 MV = view * model;
    vec4 vpoint_mv = MV * vec4(vpoint, 1.0);
    gl_Position = projection * vpoint_mv;
    ///>>>>>>>>>> TODO >>>>>>>>>>>
    /// TODO 0.1: Gouraud shading.
    /// 1) compute the normal using the model_view matrix.
    /// 2) compute the light direction light_dir.
    /// 3) compute the view direction view_dir.
    /// 4) compute per vertex color
    ///<<<<<<<<<< TODO <<<<<<<<<<<
    
    vec3 normal_mv = normalize((inverse(transpose(MV)) * vec4(vnormal, 1.0)).xyz);
    vec3 light_dir = normalize(light_pos-vpoint_mv.xyz);
    vec3 view_dir = normalize(-vpoint_mv.xyz);
    vec3 mirror_reflex = normalize(reflect(-light_dir, normal_mv));
    
    vec3 ambiant = Ia*ka;
    vec3 diffuse = (Id*kd)*max(dot(normal_mv,light_dir), 0.0);
    vec3 specular = (Is * ks) * pow(max(dot(view_dir,mirror_reflex),0.0) ,p);
    vcolor = ambiant + diffuse + specular;
}
