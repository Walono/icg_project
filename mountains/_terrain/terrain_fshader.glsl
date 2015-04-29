#version 330 core
out vec3 color;
in vec2 uv;
in vec3 newPos;

void main() {

	
	if(newPos.y < 0.5 && newPos.y > 0.4) {
	color = vec3(0.f,0.f,1.f);
	}
	else if( newPos.y < 0.4 && newPos.y > 0.1){
		color = vec3(0.f,1.f,0.f);
	}
	else {
	color = vec3(1.f,0.f,0.f);
	}
    
}
