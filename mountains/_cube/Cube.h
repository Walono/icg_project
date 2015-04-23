#include "icg_common.h"

namespace {
static const unsigned int nCubeVertices = 36;
static const vec3 cubeVertices[] =
{
    vec3(-0.5, -0.5, -0.5),
    vec3(-0.5, 0.5, -0.5),
    vec3(0.5, -0.5, -0.5),
    vec3(-0.5, 0.5, -0.5),
    vec3(0.5, -0.5, -0.5),
    vec3(0.5, 0.5, -0.5),
    vec3(0.5, 0.5, 0.5),
    vec3(0.5, -0.5, 0.5),
    vec3(0.5, 0.5, -0.5),
    vec3(0.5, -0.5, 0.5),
    vec3(0.5, 0.5, -0.5),
    vec3(0.5, -0.5, -0.5),
    vec3(0.5, 0.5, 0.5),
    vec3(-0.5, 0.5, 0.5),
    vec3(0.5, -0.5, 0.5),
    vec3(-0.5, 0.5, 0.5),
    vec3(0.5, -0.5, 0.5),
    vec3(-0.5, -0.5, 0.5),
    vec3(-0.5, -0.5, 0.5),
    vec3(-0.5, -0.5, -0.5),
    vec3(0.5, -0.5, 0.5),
    vec3(-0.5, -0.5, -0.5),
    vec3(0.5, -0.5, 0.5),
    vec3(0.5, -0.5, -0.5),
    vec3(-0.5, 0.5, -0.5),
    vec3(-0.5, -0.5, -0.5),
    vec3(-0.5, 0.5, 0.5),
    vec3(-0.5, -0.5, -0.5),
    vec3(-0.5, 0.5, 0.5),
    vec3(-0.5, -0.5, 0.5),
    vec3(0.5, 0.5, -0.5),
    vec3(-0.5, 0.5, -0.5),
    vec3(0.5, 0.5, 0.5),
    vec3(-0.5, 0.5, -0.5),
    vec3(0.5, 0.5, 0.5),
    vec3(-0.5, 0.5, 0.5)
};

static const unsigned int nCubeColors = 36;
static const vec3 cubeColors[] =
{
    vec3(0.583f,  0.771f,  0.014f),
    vec3(0.609f,  0.115f,  0.436f),
    vec3(0.327f,  0.483f,  0.844f),
    vec3(0.822f,  0.569f,  0.201f),
    vec3(0.435f,  0.602f,  0.223f),
    vec3(0.310f,  0.747f,  0.185f),
    vec3(0.597f,  0.770f,  0.761f),
    vec3(0.559f,  0.436f,  0.730f),
    vec3(0.359f,  0.583f,  0.152f),
    vec3(0.483f,  0.596f,  0.789f),
    vec3(0.559f,  0.861f,  0.639f),
    vec3(0.195f,  0.548f,  0.859f),
    vec3(0.014f,  0.184f,  0.576f),
    vec3(0.771f,  0.328f,  0.970f),
    vec3(0.406f,  0.615f,  0.116f),
    vec3(0.676f,  0.977f,  0.133f),
    vec3(0.971f,  0.572f,  0.833f),
    vec3(0.140f,  0.616f,  0.489f),
    vec3(0.997f,  0.513f,  0.064f),
    vec3(0.945f,  0.719f,  0.592f),
    vec3(0.543f,  0.021f,  0.978f),
    vec3(0.279f,  0.317f,  0.505f),
    vec3(0.167f,  0.620f,  0.077f),
    vec3(0.347f,  0.857f,  0.137f),
    vec3(0.055f,  0.953f,  0.042f),
    vec3(0.714f,  0.505f,  0.345f),
    vec3(0.783f,  0.290f,  0.734f),
    vec3(0.722f,  0.645f,  0.174f),
    vec3(0.302f,  0.455f,  0.848f),
    vec3(0.225f,  0.587f,  0.040f),
    vec3(0.517f,  0.713f,  0.338f),
    vec3(0.053f,  0.959f,  0.120f),
    vec3(0.393f,  0.621f,  0.362f),
    vec3(0.673f,  0.211f,  0.457f),
    vec3(0.820f,  0.883f,  0.371f),
    vec3(0.982f,  0.099f,  0.879f)
};

static const unsigned int nCubeUVs = 36;

static const vec2 cubeUVs[] =
{
    vec2(0.333, 0.75),
    vec2(0.666, 0.75),
    vec2(0.333, 0.5),
    vec2(0.666, 0.75),
    vec2(0.333, 0.5),
    vec2(0.666, 0.5),

    vec2(0.666, 0.25),
    vec2(0.333, 0.25),
    vec2(0.666, 0.5),
    vec2(0.333, 0.25),
    vec2(0.666, 0.5),
    vec2(0.333, 0.5),

    vec2(0.666, 0.25),
    vec2(0.666, 0.0),
    vec2(0.333, 0.25),
    vec2(0.666, 0.0),
    vec2(0.333, 0.25),
    vec2(0.333, 0.0),

    vec2(0.0, 0.75),
    vec2(0.333, 0.75),
    vec2(0.0, 0.5),
    vec2(0.333, 0.75),
    vec2(0.0, 0.5),
    vec2(0.333, 0.5),

    vec2(0.666, 0.75),
    vec2(0.333, 0.75),
    vec2(0.666, 1.0),
    vec2(0.333, 0.75),
    vec2(0.666, 1.0),
    vec2(0.333, 1.0),

    vec2(0.666, 0.5),
    vec2(0.666, 0.75),
    vec2(1.0, 0.5),
    vec2(0.666, 0.75),
    vec2(1.0, 0.5),
    vec2(1.0, 0.75)
};

}

class Cube{
protected:
    GLuint _vao; ///< vertex array object
    GLuint _pid; ///< GLSL shader program ID 
    GLuint _vbo; ///< memory buffer
    GLuint _tex; ///< Texture ID
    mat4   _M;   ///< model matrix
    
public:
    void init(){
        ///--- Compile the shaders
        _pid = opengp::load_shaders("_cube/cube_vshader.glsl", "_cube/cube_fshader.glsl");
        if(!_pid) exit(EXIT_FAILURE);       
        glUseProgram(_pid);
        
        ///--- Vertex one vertex Array
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);
     
        ///--- Vertex coordinates
        {    
            ///--- Buffer
            glGenBuffers(1, &_vbo);
            glBindBuffer(GL_ARRAY_BUFFER, _vbo);
            glBufferData(GL_ARRAY_BUFFER, nCubeVertices * sizeof(vec3), &cubeVertices[0], GL_STATIC_DRAW);
        
            ///--- Attribute
            GLuint vpoint_id = glGetAttribLocation(_pid, "vpoint");
            glEnableVertexAttribArray(vpoint_id);
            glVertexAttribPointer(vpoint_id, 3, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);
        }
        
        ///--- Texture coordinates
        {
            ///--- Buffer
            glGenBuffers(1, &_vbo);
            glBindBuffer(GL_ARRAY_BUFFER, _vbo);
            glBufferData(GL_ARRAY_BUFFER, nCubeUVs * sizeof(vec2), &cubeUVs[0], GL_STATIC_DRAW);
        
            ///--- Attribute
            GLuint vtexcoord_id = glGetAttribLocation(_pid, "vtexcoord");
            glEnableVertexAttribArray(vtexcoord_id);
            glVertexAttribPointer(vtexcoord_id, 2, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);
        }
        
        ///--- Load texture
        glGenTextures(1, &_tex);
        glBindTexture(GL_TEXTURE_2D, _tex);
        glfwLoadTexture2D("_cube/cube_texture.tga", 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        GLuint tex_id = glGetUniformLocation(_pid, "tex");
        glUniform1i(tex_id, 0 /*GL_TEXTURE0*/);

        ///--- to avoid the current object being polluted
        glBindVertexArray(0);
        glUseProgram(0);
        
        ///--- Create the model matrix (remember OpenGL is right handed)
        ///--- Accumulated transformation
        typedef Eigen::Transform<float,3,Eigen::Affine> Transform;
        Transform _M = Transform::Identity();
        _M *= Eigen::AngleAxisf(0, Eigen::Vector3f::UnitZ());
        _M *= Eigen::AlignedScaling3f(.5, .5, .5);
        _M *= Eigen::Translation3f(0,0,.5 + .1);
        this->_M = _M.matrix();
    }
    
    void cleanup(){
        /// TODO cleanup 
    }
    
    void draw(const mat4& VP, float time /*seconds*/){
        glUseProgram(_pid);
        glBindVertexArray(_vao);        
            ///--- Bind textures
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, _tex);
           
            ///--- Time
            glUniform1f(glGetUniformLocation(_pid, "time"), time);
            
            ///--- Setup MVP
            mat4 MVP = VP * _M;
            GLuint MVP_id = glGetUniformLocation(_pid, "MVP");
            glUniformMatrix4fv(MVP_id, 1, GL_FALSE, MVP.data());
            
            ///--- Draw
            glDrawArrays(GL_TRIANGLES,0,nCubeVertices);
        glBindVertexArray(0);        
        glUseProgram(0);
    }
};
