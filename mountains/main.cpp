#include "icg_common.h"
#include "FrameBuffer.h"
#include "_cube/Cube.h"
#include "_quad/Quad.h"
#include "_screenquad/ScreenQuad.h"
#include "_terrain/Terrain.h"
#include "trackball.h"

enum OPTIMIZATION_MODE {
    OPTI_ON,
    OPTI_OFF
} optimode;

int width=720, height=720;

float _oldYPos = 5;

Cube cube;
Quad quad;

FrameBuffer fb(width, height);
Terrain squad;

mat4 projection_matrix;
mat4 view_matrix;
mat4 trackball_matrix;

Trackball trackball;

mat4 PerspectiveProjection(float fovy, float aspect, float near, float far){
	// TODO 1: Create a perspective projection matrix given the field of view,
	// aspect ratio, and near and far plane distances.
	mat4 projection = mat4::Zero();
	double f = 1 / tan(fovy / 2);

	projection(0, 0) = f / aspect;
	projection(1, 1) = f;
	projection(2, 2) = (far + near) / (near - far);
	projection(2, 3) = (2 * far*near) / (near - far);
	projection(3, 2) = -1.0f;
	return projection;
}

mat4 LookAt(vec3 eye, vec3 center, vec3 up) {
	// We need a function that converts from world coordinates into camera coordiantes.
	//
	// Cam coords to world coords is given by:
	// X_world = R * X_cam + eye
	//
	// inverting it leads to:
	//
	// X_cam = R^T * X_world - R^T * eye
	//
	// Or as a homogeneous matrix:
	// [ r_00 r_10 r_20 -r_0*eye
	//   r_01 r_11 r_21 -r_1*eye
	//   r_02 r_12 r_22 -r_2*eye
	//      0    0    0        1 ]

	vec3 z_cam = (eye - center).normalized();
	vec3 x_cam = up.cross(z_cam).normalized();
	vec3 y_cam = z_cam.cross(x_cam);

	mat3 R;
	R.col(0) = x_cam;
	R.col(1) = y_cam;
	R.col(2) = z_cam;

	mat4 look_at = mat4::Zero();
	look_at.block(0, 0, 3, 3) = R.transpose();
	look_at(3, 3) = 1.0f;
	look_at.block(0, 3, 3, 1) = -R.transpose() * (eye);
	return look_at;
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
    glEnable(GL_DEPTH_TEST);
    cube.init();
    quad.init();
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

	squad.init(heightmap_tex, tabMixingTex);
	view_matrix = LookAt(vec3(2.0f, 2.0f, 4.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	view_matrix = Eigen::Affine3f(Eigen::Translation3f(0.0f, 0.0f, -4.0f)).matrix();
	trackball_matrix = mat4::Identity();
	check_error_gl();
}

void display(){ 
    opengp::update_title_fps("FrameBuffer");   
    glViewport(0,0,width,height);
    
    ///--- Setup view-projection matrix
    float ratio = width / (float) height;
    static mat4 projection = Eigen::perspective(45.0f, ratio, 0.1f, 10.0f);
    vec3 cam_pos(2.0f, 2.0f, 2.0f);
    vec3 cam_look(0.0f, 0.0f, 0.0f);
    vec3 cam_up(0.0f, 0.0f, 1.0f);
    mat4 view = Eigen::lookAt(cam_pos, cam_look, cam_up);
    mat4 VP = projection * view;

    ///--- Render to Window
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const float time = glfwGetTime();

	mat4 quad_model_matrix = Eigen::Affine3f(Eigen::Translation3f(vec3(0.0f, -0.25f, 0.0f))).matrix();
	squad.draw(trackball_matrix * quad_model_matrix, view_matrix, projection_matrix, time);
	cube.draw(projection_matrix*view_matrix*trackball_matrix , time);

}

/*void keyboard(int key, int action){
    if(action != GLFW_RELEASE) return; ///< only act on release
    switch(key){
        case '1':
            optimode = OPTI_OFF;
            std::cout << "optimization off.\n" << std::flush;
            break;
        case '2':
            optimode = OPTI_ON;
            std::cout << "optimization on.\n" << std::flush;
            break;
        default:
            break;
    }
}*/

// Transforms glfw screen coordinates into normalized OpenGL coordinates.
vec2 transform_screen_coords(int x, int y) {
	return vec2(2.0f * (float)x / width - 1.0f,
		1.0f - 2.0f * (float)y / height);
}

mat4 old_trackball_matrix;

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
		// TODO 3: Cacuclate 'model_matrix' given the return value of
		// trackball.drag(...) and the value stored in 'old_model_matrix'.
		// See also the mouse_button(...) function.
		//model_matrix = ...
		trackball_matrix = trackball.drag(p(0), p(1)) * old_trackball_matrix;

	}

	// Zoom
	if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		// TODO 4: Implement zooming. When the right mouse button is pressed,
		// moving the mouse cursor up and down (along the screen's y axis)
		// should zoom out and it. For that you have to update the current
		// 'view_matrix' with a translation along the z axis.
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
		}
		_oldYPos = posY;

	}
}

int main(int, char**){
    glfwInitWindowSize(width, height);
    glfwCreateWindow();
    glfwDisplayFunc(display);
	glfwSetWindowSizeCallback(&resize_callback);
	glfwSetMouseButtonCallback(mouse_button);
	glfwSetMousePosCallback(mouse_pos);
    init();
    glfwSwapInterval(0); ///< disable VSYNC (allows framerate>30)
    glfwMainLoop();
    return EXIT_SUCCESS;
}
