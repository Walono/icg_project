// Copyright (C) 2014 - LGG EPFL
#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

in vec4 vpoint_mv[3];
in vec3 light_dir_g[3], view_dir_g[3];

out vec3 normal_mv;
out vec3 light_dir, view_dir;

void main() {
    ///>>>>>>>>>> TODO >>>>>>>>>>>
    /// TODO 4.2: Flat shading.
    /// 1) compute the triangle normal normal_mv.
    ///<<<<<<<<<< TODO <<<<<<<<<<<
    for(int i = 0; i < gl_in.length(); i++) {
        gl_Position = gl_in[i].gl_Position;
        light_dir = light_dir_g[i];
        view_dir = view_dir_g[i];
        EmitVertex();
    }
    EndPrimitive();
}
