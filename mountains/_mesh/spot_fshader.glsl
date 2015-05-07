#version 330 core

const float spot_cos_cutoff = 0.985; // cos 10 degrees
const float spot_exp = 150;

uniform vec3 Ia, Id, Is;
uniform vec3 ka, kd, ks;
uniform float p;
uniform vec3 light_pos;
uniform vec3 spot_dir;

in vec3 normal_mv;
in vec3 light_dir;
in vec3 view_dir;
in vec4 vpoint_mv;

out vec3 color;

void main() {    
    /*const vec3 COLORS[6] = vec3[](
        vec3(1.0,0.0,0.0),
        vec3(0.0,1.0,0.0),
        vec3(0.0,0.0,1.0),
        vec3(1.0,1.0,0.0),
        vec3(0.0,1.0,1.0),
        vec3(1.0,0.0,1.0));
    int index = int( mod(gl_PrimitiveID,6) );
    color = COLORS[index];*/

    ///>>>>>>>>>> TODO >>>>>>>>>>>
    /// TODO 5: Spot light.
    /// Complete the shader to obtain spot light effect
    ///<<<<<<<<<< TODO <<<<<<<<<<<
    
    vec3 mirror_reflex = normalize(reflect(-light_dir, normal_mv));
    float spotEffect = 0.0;
    
    vec3 vdir = normalize(vpoint_mv.xyz-light_pos);
    
    if(dot(normalize(-spot_dir), vdir)>spot_cos_cutoff){
		spotEffect = pow(dot(normalize(light_dir), normalize(spot_dir)),spot_exp);
	}
    vec3 ambiant = Ia * ka;
    float NL = max(dot(normal_mv, light_dir), 0);
    vec3 diffuse = (Id * kd) * NL;
    float VR = max(dot(view_dir, mirror_reflex), 0);
    vec3 specular = (Is * ks) * pow(VR, p);
    color = (ambiant + diffuse + specular)*spotEffect;
}
