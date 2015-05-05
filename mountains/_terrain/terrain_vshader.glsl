#version 330 core

uniform mat4 P;
uniform mat4 MV;
uniform sampler2D heightmap;
uniform vec3 light_pos;

in vec2 position;
out vec2 uv;
out vec3 newPos;

out vec3 light_dir;
out vec3 view_dir;

void main() {
	mat4 mvp = P*MV;
    uv = (position + vec2(1.0, 1.0)) * 0.5;
	
    float height = texture(heightmap, uv).x;
    vec3 pos_3d = vec3(position.x, height, position.y);
    ///--phong shading---///
    vec4 vpoint_mv = MV * vec4(pos_3d, 1.0);
    
    light_dir = normalize(light_pos - vpoint_mv.xyz);
	view_dir = normalize(-vpoint_mv.xyz);
	///---///
    gl_Position = mvp * vec4(pos_3d, 1.0);
	newPos = pos_3d.xyz;
}
