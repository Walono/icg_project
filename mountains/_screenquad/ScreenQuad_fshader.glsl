#version 330 core
uniform sampler2D tex;
uniform float tex_width;
uniform float tex_height;
uniform float time;
in vec2 uv;
out vec3 color;

float rgb_2_luma(vec3 c){ return .3*c[0] + .59*c[1] + .11*c[2]; }

float gridSize = 1;


float smoothf(float x){
	return 6*pow(x,6) - 15*pow(x,4) + 10*pow(x,3);
}

float mixNumber(float x, float y, float alpha){
	return (1-alpha)*x + alpha*y;
}


void main() {
/*#if 0
    ///--- x=-1
    float t_00 = rgb_2_luma( textureOffset(tex, uv, ivec2(-1, -1)).rgb );
    float t_01 = rgb_2_luma( textureOffset(tex, uv, ivec2(-1,  0)).rgb );
    float t_02 = rgb_2_luma( textureOffset(tex, uv, ivec2(-1, +1)).rgb );
    ///--- x=0
    float t_10 = rgb_2_luma( textureOffset(tex, uv, ivec2( 0, -1)).rgb );
    float t_12 = rgb_2_luma( textureOffset(tex, uv, ivec2( 0, +1)).rgb );
    ///--- x=+1
    float t_20 = rgb_2_luma( textureOffset(tex, uv, ivec2(+1, -1)).rgb );
    float t_21 = rgb_2_luma( textureOffset(tex, uv, ivec2(+1,  0)).rgb );
    float t_22 = rgb_2_luma( textureOffset(tex, uv, ivec2(+1, +1)).rgb );
    ///--- Gradients
    float sx = -1*t_00 -2*t_01 -1*t_02    +1*t_20 +2*t_21 +1*t_22;
    float sy = -1*t_00 -2*t_10 -1*t_20    +1*t_02 +2*t_12 +1*t_22;
    float g = 1-sqrt(sx*sx + sy*sy);
    
    ///--- Composite
//    color = texture(tex,uv).rgb; ///< passthrough shading
//    color = abs( vec3(sx, sx, sx) ); ///< derivatives x
//    color = abs( vec3(sy, sy, sy) ); ///< derivatives y
    tmp2 = vec3(g, g, g);
#else
    ///--- Gaussian convolution
    const float std = 2; ///< standard deviation (<.3 disable)
    // float std = .1; ///< standard deviation (<.3 disable)

    if(mode==0){
		 vec3 color_tot = vec3(0,0,0);
		float weight_tot = 0;
		int SIZE = 1 + 2 * 3 * int( ceil(std) );
		for(int i=-SIZE; i<=SIZE; i++){
			for(int j=-SIZE; j<=SIZE; j++){
				float w = exp(-(i*i+j*j)/(2.0*std*std*std*std));
				vec3 neigh_color = texture(tex, uv+vec2(i/tex_width,j/tex_height)).rgb;
				color_tot += w * neigh_color; 
				weight_tot += w;
			}
		}
		tmp2 = color_tot / weight_tot; ///< ensure \int w = 1
	}
	if(mode==1){
		float weight_tot = 0;
		const int SIZE = 1 + 2 * 3 * int( ceil(std) );
		vec3 tmp[SIZE];
		for(int i=-SIZE; i<=SIZE; i++){
			for(int j=-SIZE; j<=SIZE; j++){
				float w = exp(-(i*i)/(2.0*std*std));
				vec3 neigh_color = texture(tex, uv+vec2(i/tex_width,j/tex_height)).rgb;
				tmp[i] += w * neigh_color; 
				weight_tot += w;
			}
		}
		vec3 tmp_2 = vec3(0,0,0);
		for(int j=-SIZE; j<=SIZE; j++){
				float w = exp(-(j*j)/(2.0*std*std));
				vec3 neigh_color = tmp[j];
				tmp_2 += w * neigh_color; 
				weight_tot += w;
		}
		
		
	
		
	tmp2 = tmp_2 ;// weight_tot; ///< ensure \int w = 1
	}
    
#endif*/
	
    
    
     
    
	
	color = texture(tex,uv).rgb;


}

