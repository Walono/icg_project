#include "icg_common.h"
#include "trackball.h"
#include "_grid/grid.h"

enum NAVIGATION_MODE {
	TRACKBALL,
	BEZIER
} navmode;

using namespace std;

Grid grid;

int WIDTH = 800;
int HEIGHT = 600;

mat4 projection_matrix;
mat4 view_matrix;
mat4 model;

///----------------- Trackball -----------------
mat4 trackball_matrix;
mat4 old_trackball_matrix;
Trackball trackball;

vec2 transform_screen_coords(int x, int y) {
	return vec2(2.0f * (float)x / WIDTH - 1.0f,
		1.0f - 2.0f * (float)y / HEIGHT);
}

void trackball_button(int button, int action) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		int x_i, y_i;
		glfwGetMousePos(&x_i, &y_i);
		vec2 p = transform_screen_coords(x_i, y_i);
		trackball.begin_drag(p.x(), p.y());
		old_trackball_matrix = trackball_matrix;  // Store the current state of the model matrix.
	}
}

void trackball_pos(int x, int y) {
	if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		vec2 p = transform_screen_coords(x, y);
		trackball_matrix = trackball.drag(p.x(), p.y()) * old_trackball_matrix;
	}
}
///---------------------------------------------------


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
    WIDTH = width;
    HEIGHT = height;

    std::cout << "Window has been resized to " << WIDTH << "x" << HEIGHT << "." << std::endl;
    glViewport(0, 0, WIDTH, HEIGHT);

    // TODO 1: Use a perspective projection instead;
    projection_matrix = PerspectiveProjection(45.0f, (GLfloat)WIDTH / HEIGHT, 0.1f, 100.0f);
    GLfloat top = 1.0f;
    GLfloat right = (GLfloat)WIDTH / HEIGHT * top;
}

void init(){
    // Sets background color.
    glClearColor(/*gray*/ .937,.937,.937, /*solid*/1.0);
    
    grid.init();

    // Enable depth test.
    glEnable(GL_DEPTH_TEST);


    view_matrix = Eigen::Affine3f(Eigen::Translation3f(0.0f, 0.0f, -4.0f)).matrix();

	model = mat4::Identity();
    trackball_matrix = mat4::Identity();
	navmode = TRACKBALL;
    check_error_gl();
}

// Gets called for every frame.
void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
    const float time = glfwGetTime();

    // Draw a quad on the ground.
    mat4 quad_model_matrix = Eigen::Affine3f(Eigen::Translation3f(vec3(0.0f, -0.25f, 0.0f))).matrix();
    grid.draw(trackball_matrix * quad_model_matrix, view_matrix, projection_matrix, time);



    check_error_gl();
}

void mouse_button(int button, int action) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		trackball_button(button, action);
    }
}

void mouse_pos(int x, int y) {
	trackball_pos(x, y);
}

int main(int, char**){
    glfwInitWindowSize(WIDTH, HEIGHT);
    glfwCreateWindow("Trackball");
    glfwDisplayFunc(display);
    glfwSetWindowSizeCallback(&resize_callback);
    glfwSetMouseButtonCallback(mouse_button);
    glfwSetMousePosCallback(mouse_pos);
    init();
    glfwMainLoop();
    return EXIT_SUCCESS;
}
