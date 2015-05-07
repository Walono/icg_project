// Copyright (C) 2014 - LGG EPFL
#version 330 core
uniform vec3 Ia, Id, Is;
uniform vec3 ka, kd, ks;

in vec3 normal_mv;
in vec3 light_dir;
in vec3 view_dir;

uniform float p;
uniform sampler2D tex2D;


out vec3 color;

void main() {
    /*color = vec3(0.0,0.0,0.0);

    const vec3 COLORS[6] = vec3[](
        vec3(1.0,0.0,0.0),
        vec3(0.0,1.0,0.0),
        vec3(0.0,0.0,1.0),
        vec3(1.0,1.0,0.0),
        vec3(0.0,1.0,1.0),
        vec3(1.0,0.0,1.0));
    int index = int( mod(gl_PrimitiveID,6) );
    color = COLORS[index];*/

    ///>>>>>>>>>> TODO >>>>>>>>>>>
    /// TODO 3.2: Artistic shading.
    /// 1) compute the output color by doing a look-up in the texture using the texture sampler tex.
    ///<<<<<<<<<< TODO <<<<<<<<<<<
    vec3 mirror_reflex = normalize(reflect(-light_dir, normal_mv));
    vec3 ambiant = Ia * ka;
    float NL = max(dot(normal_mv, light_dir), 0);
    float VR = max(dot(view_dir, mirror_reflex), 0);
    color = texture(tex2D, vec2(NL, VR));
}
