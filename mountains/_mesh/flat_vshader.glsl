// Copyright (C) 2014 - LGG EPFL
#version 330 core

in vec3 vpoint;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform vec3 light_pos;

out vec4 vpoint_mv;
out vec3 light_dir, view_dir;

void main() {
    mat4 MV = view * model;
    vpoint_mv = MV * vec4(vpoint, 1.0);
    gl_Position = projection * vpoint_mv;
    ///>>>>>>>>>> TODO >>>>>>>>>>>
    /// TODO 4.1: Flat shading.
    /// 1) compute the light direction light_dir.
    /// 2) compute the view direction view_dir.
    ///<<<<<<<<<< TODO <<<<<<<<<<<
    light_dir = normalize(light_pos - vpoint_mv.xyz);
	view_dir = normalize(-vpoint_mv.xyz);
}
