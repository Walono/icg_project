#include "icg_common.h"
#include "FrameBuffer.h"
#include "_cube/Cube.h"
#include "_quad/Quad.h"
#include "_screenquad/ScreenQuad.h"
#include "_terrain/Terrain.h"
#include "trackball.h"
#include "_water/Water.h"

#ifdef WITH_ANTTWEAKBAR
#include "AntTweakBar.h"
TwBar *bar;
#endif

enum OPTIMIZATION_MODE {
    OPTI_ON,
    OPTI_OFF
} optimode;

int width=720, height=720;

float _oldXPos = 5;
float _oldYPos = 5;
bool _moveL;
bool _moveR;
bool _moveU;
bool _moveD;

Cube cube;
Quad quad;

FrameBuffer fb(width, height);
Terrain terrain;
Water water;

mat4 projection_matrix;
mat4 view_matrix;
//mat4 view_matrix_mirrored;
mat4 trackball_matrix;
mat4 old_trackball_matrix;

vec3 light_pos = vec3(1,1,1);

Trackball trackball;

mat4 PerspectiveProjection(float fovy, float aspect, float near, float far){
	mat4 projection = mat4::Zero();
	double f = 1 / tan(fovy / 2);

	projection(0, 0) = f / aspect;
	projection(1, 1) = f;
	projection(2, 2) = (far + near) / (near - far);
	projection(2, 3) = (2 * far*near) / (near - far);
	projection(3, 2) = -1.0f;
	return projection;
}


// Gets called when the windows is resized.
void resize_callback(int width, int height) {
	_width = width;
	_height = height;

	std::cout << "Window has been resized to " << _width << "x" << _height << "." << std::endl;
	glViewport(0, 0, _width, _height);

	projection_matrix = PerspectiveProjection(45.0f, (GLfloat)_width / _height, 0.1f, 100.0f);
	GLfloat top = 1.0f;
	GLfloat right = (GLfloat)_width / _height * top;
}

void init(){
	glClearColor(0.5, 0.5, 0.5, /*solid*/1.0);
	check_error_gl();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    check_error_gl();
    cube.init();
    check_error_gl();
    quad.init();
    check_error_gl();
    #ifdef WITH_ANTTWEAKBAR
    TwInit(TW_OPENGL_CORE, NULL);
    TwWindowSize(width, height);
    bar = TwNewBar("AntTweakBar");
    check_error_gl();
    ///--- Light
    TwAddVarRW(bar, "Ia", TW_TYPE_COLOR3F, terrain.Ia.data(), NULL);
    TwAddVarRW(bar, "Id", TW_TYPE_COLOR3F, terrain.Id.data(), NULL);
    TwAddVarRW(bar, "Is", TW_TYPE_COLOR3F, terrain.Is.data(), NULL);
    TwAddVarRW(bar, "LightDir", TW_TYPE_DIR3F, light_pos.data(), NULL);
    ///--- Material
    TwAddVarRW(bar, "p", TW_TYPE_FLOAT, &terrain.p, "min=.5 max=50 step=0.4");
    
    ///--- Callbacks
    ///--- https://github.com/davidcox/AntTweakBar/blob/master/examples/TwSimpleGLFW.c
    glfwSetMouseButtonCallback((GLFWmousebuttonfun)TwEventMouseButtonGLFW);
    glfwSetMouseButtonCallback((GLFWmousebuttonfun)TwEventMouseButtonGLFW);
    glfwSetMousePosCallback((GLFWmouseposfun)TwEventMousePosGLFW);
    glfwSetMouseWheelCallback((GLFWmousewheelfun)TwEventMouseWheelGLFW);
#endif
    
    optimode = OPTI_OFF;
    GLuint heightmap_tex = fb.init(true);   
    
	//noise for mountains heightmap
	fb.bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	quad.draw(mat4::Identity(), 13, 2.1f, 0.4f, 1/170.0f, 1.5);
	fb.unbind();
	
	std::vector<GLuint> tabMixingTex;
	tabMixingTex.push_back(fb.init()); 
	//noise for mixing grass and rock
	fb.bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	quad.draw(mat4::Identity(), 3, 2.0f, 0.6f, 1/100.0f, 1.5);
	fb.unbind();
	
	tabMixingTex.push_back(fb.init()); 
	//noise for mixing snow with grass
	fb.bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	quad.draw(mat4::Identity(), 13, 2.0f, 0.4f, 1/14.0f, 1.4);
	fb.unbind();

	terrain.init(heightmap_tex, tabMixingTex);
	
	//water with reflection, actually didn't work
	GLuint fb_tex = fb.init();
	water.init(heightmap_tex, fb_tex);	
	
	view_matrix = Eigen::lookAt(vec3(2.0f, 2.0f, 4.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	//view_matrix_mirrored = Eigen::lookAt(vec3(2.0f, 2.0f, 4.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	trackball_matrix = mat4::Identity();
	
	_moveL = false;
	_moveR = false;
	_moveU = false;
	_moveD = false;
	check_error_gl();
}



void display(){ 
    opengp::update_title_fps("FrameBuffer");   
    glViewport(0,0,width,height);
    
    ///--- Setup view-projection matrix
    float ratio = width / (float) height;

    ///--- Render to Window
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const float time = glfwGetTime();
	
	//used for reflection but actually didn't work
	//mat4 reflectionMatrix =  Eigen::Affine3f(Eigen::scale(vec3(1.0,-1.0,1.0)).matrix();
	//reflectionMatrix(2,2) = -1.f;
	//reflectionMatrix(2,3) = 0.9f;

	
	//mat4 VP_mirrored = projection * view_matrix_mirrored;

	mat4 quad_model_matrix = Eigen::Affine3f(Eigen::Translation3f(vec3(0.0f, -0.85f, 0.0f))).matrix();
	mat4 mat = view_matrix*trackball_matrix * quad_model_matrix;	
	vec4 vec = vec4(light_pos.x(), light_pos.y(), light_pos.z(), 1.0);
	vec4 vc2 = mat*vec;
	vec3 light = vec3(vc2.x(), vc2.y(), vc2.z());
	
	//get the terrain reflection
	/*b.bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		cube.draw(projection_matrix*view_matrix_mirrored, time);
	fb.unbind();*/
		
	//terrain.draw(trackball_matrix * quad_model_matrix, view_matrix, projection_matrix, time,light);
	terrain.draw(quad_model_matrix, view_matrix, projection_matrix, time, light);
	cube.draw(projection_matrix*view_matrix, time);
	water.draw(quad_model_matrix, view_matrix, projection_matrix, time, light);
	
	
	mat4 T = mat4::Identity();
	if(_moveL){
		T(0, 3) = 0.01f;
		view_matrix = T * view_matrix;
		//view_matrix_mirrored = T * view_matrix_mirrored;
	}
	if(_moveR){
		T(0, 3) = -0.01f;
		view_matrix = T * view_matrix;
		//view_matrix_mirrored = T * view_matrix_mirrored;
	}
	if(_moveU){
		T(2, 3) = 0.02f;
		view_matrix = T * view_matrix;
		//view_matrix_mirrored = T * view_matrix_mirrored;
	}
	if(_moveD){
		T(2, 3) = -0.02f;
		view_matrix = T * view_matrix;
		//view_matrix_mirrored = T * view_matrix_mirrored;
	}


#ifdef WITH_ANTTWEAKBAR
    TwDraw();
#endif
}

void cleanup(){
#ifdef WITH_ANTTWEAKBAR
    TwTerminate();
#endif
}


// Transforms glfw screen coordinates into normalized OpenGL coordinates.
vec2 transform_screen_coords(int x, int y) {
	return vec2(2.0f * (float)x / width - 1.0f,
		1.0f - 2.0f * (float)y / height);
}



void mouse_button(int button, int action) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		int x_i, y_i;
		glfwGetMousePos(&x_i, &y_i);
		vec2 p = transform_screen_coords(x_i, y_i);
		trackball.begin_drag(p.x(), p.y());
		old_trackball_matrix = trackball_matrix;  // Store the current state of the model matrix.
	}
}

void mouse_pos(int x, int y) {
	if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		vec2 p = transform_screen_coords(x, y);
		if (_oldXPos == 5 || _oldYPos == 5){
			_oldXPos = 1.0f - 2.0f * (float)x / width;
			_oldYPos = 1.0f - 2.0f * (float)y / height;			
		}
		float posX = 1.0f - 2.0f * (float)x / width;
		float posY = 1.0f - 2.0f * (float)y / height;
		float ratio = 0.05f;
		if (((-1 * (_oldYPos - posY)) >= 0.05) || ((-1 * (_oldYPos - posY)) <= -0.05)
			|| ((-1 * (_oldXPos - posX)) >= 0.05) || ((-1 * (_oldXPos - posX)) <= -0.05)){
			//do nothing
		}
		else {
			mat4 RX = Eigen::Affine3f(Eigen::AngleAxisf((_oldYPos - posY), vec3::UnitX())).matrix();
			mat4 RY = Eigen::Affine3f(Eigen::AngleAxisf((_oldXPos - posX), vec3::UnitY())).matrix();
			view_matrix = RX * view_matrix;
			view_matrix = RY * view_matrix;
			
			//view_matrix_mirrored = RX * view_matrix_mirrored;
			//view_matrix_mirrored = RY * view_matrix_mirrored;
		}
		_oldXPos = posX;
		_oldYPos = posY;
	}

	// Zoom
	/*
	if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		if (_oldYPos == 5){
			_oldYPos = 1.0f - 2.0f * (float)y / height;
		}
		float posY = 1.0f - 2.0f * (float)y / height;
		float ratio = 10.f;

		if (((-1 * (_oldYPos - posY)) >= 0.05) || ((-1 * (_oldYPos - posY)) <= -0.05)){
			//do nothing
		}
		else {
			mat4 T = mat4::Identity();
			T(2, 3) = -1 * (_oldYPos - posY) * ratio;
			view_matrix = T * view_matrix;
			//view_matrix_mirrored = T * view_matrix_mirrored;
		}
		_oldYPos = posY;

	}*/
}

void keyboard(int key, int action){
	
    if(action != GLFW_RELEASE){
		 switch(key){
			case 65: 
				_moveL = true;
				break;
			case 68: 
				_moveR = true;
				break;
			case 83: 
				_moveD = true;
				break;
			case 87: 
				_moveU = true;
				break;
			 default:
				break;
		}
	} else{
    switch(key){
        case '1':
	glfwSetMouseButtonCallback(mouse_button);
	glfwSetMousePosCallback(mouse_pos);
            std::cout << "navigation mode.\n" << std::flush;
            break;
        case '2':
            glfwSetMouseButtonCallback((GLFWmousebuttonfun)TwEventMouseButtonGLFW);
    glfwSetMouseButtonCallback((GLFWmousebuttonfun)TwEventMouseButtonGLFW);
    glfwSetMousePosCallback((GLFWmouseposfun)TwEventMousePosGLFW);
    glfwSetMouseWheelCallback((GLFWmousewheelfun)TwEventMouseWheelGLFW);
            std::cout << "set value mode.\n" << std::flush;
            break;
        case 65: 
			_moveL = false;
			break;
		case 68: 
			_moveR = false;
			break;
		case 83: 
			_moveD = false;
			break;
		case 87: 
			_moveU = false;
			break;		
        default:
            break;
    }
}
}

int main(int, char**){
    glfwInitWindowSize(width, height);
    glfwCreateWindow();
    glfwDisplayFunc(display);
    glfwSetKeyCallback(keyboard);
	glfwSetWindowSizeCallback(&resize_callback);
	glfwSetMouseButtonCallback(mouse_button);
	glfwSetMousePosCallback(mouse_pos);
    init();
    glfwSwapInterval(0); ///< disable VSYNC (allows framerate>30)
    glfwMainLoop();
    cleanup();
    return EXIT_SUCCESS;
}
