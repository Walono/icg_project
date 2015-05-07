#pragma once
#include "icg_common.h"
#include "texture.h"
/// setup 1D color texture
#define NB_COLOR 6
GLfloat tex[NB_COLOR] = {0.0, 0.2, 0.4, 0.6, 0.8, 1.0};

#undef M_PI
#define M_PI 3.14159

typedef Eigen::Transform<float,3,Eigen::Affine> Transform;

struct Light{
    vec3 Ia = vec3(1.0f, 1.0f, 1.0f);
    vec3 Id = vec3(1.0f, 1.0f, 1.0f);
    vec3 Is = vec3(1.0f, 1.0f, 1.0f);

    vec3 light_pos =  vec3(0.0f, 0.0f, 0.01f);

    ///--- Pass light properties to the shader
    void setup(GLuint _pid){
        glUseProgram(_pid);                
        GLuint light_pos_id = glGetUniformLocation(_pid, "light_pos"); //Given in camera space
        GLuint Ia_id = glGetUniformLocation(_pid, "Ia");
        GLuint Id_id = glGetUniformLocation(_pid, "Id");
        GLuint Is_id = glGetUniformLocation(_pid, "Is");
        glUniform3fv(light_pos_id, ONE, light_pos.data());
        glUniform3fv(Ia_id, ONE, Ia.data());
        glUniform3fv(Id_id, ONE, Id.data());
        glUniform3fv(Is_id, ONE, Is.data());
    }

    vec3 get_spot_direction(float time) {
        return light_pos;
    }
};

struct Material{
    vec3 ka = vec3(0.18f, 0.1f, 0.1f);
    vec3 kd = vec3(0.9f, 0.5f, 0.5f);
    vec3 ks = vec3(0.8f, 0.8f, 0.8f);
    float p = 60.0f;

    ///--- Pass material properties to the shaders
    void setup(GLuint _pid){
        glUseProgram(_pid);        
        GLuint ka_id = glGetUniformLocation(_pid, "ka");
        GLuint kd_id = glGetUniformLocation(_pid, "kd");
        GLuint ks_id = glGetUniformLocation(_pid, "ks");
        GLuint p_id = glGetUniformLocation(_pid, "p");
        glUniform3fv(ka_id, ONE, ka.data());
        glUniform3fv(kd_id, ONE, kd.data());
        glUniform3fv(ks_id, ONE, ks.data());
        glUniform1f(p_id, p);
    }
};

class Mesh : public Material, public Light{
protected:
    GLuint _vao; ///< vertex array object

    GLuint _pid_gouraud;
    GLuint _pid_phong; ///< GLSL shader program ID
    GLuint _pid_toon;
    GLuint _pid_art;
    GLuint _pid_flat;
    GLuint _pid_phong_spot_light;

    GLuint _pid_active;

    opengp::Surface_mesh mesh;    
    GLuint _vpoint;    ///< memory buffer
    GLuint _vnormal;   ///< memory buffer
    GLuint _tex1D; ///< Texture ID
    GLuint _tex2D;

private:
    void bindShader(GLuint _pid) {
        glUseProgram(_pid);
        glBindVertexArray(_vao);
        check_error_gl();

        ///--- Vertex Attribute ID for Positions
        GLint vpoint_id = glGetAttribLocation(_pid, "vpoint");
        if (vpoint_id >= 0) {
            glEnableVertexAttribArray(vpoint_id);
            check_error_gl();
            glBindBuffer(GL_ARRAY_BUFFER, _vpoint);
            glVertexAttribPointer(vpoint_id, 3 /*vec3*/, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);
            check_error_gl();
        }

        ///--- Vertex Attribute ID for Normals
        GLint vnormal_id = glGetAttribLocation(_pid, "vnormal");
        if (vnormal_id >= 0) {
            glEnableVertexAttribArray(vnormal_id);
            glBindBuffer(GL_ARRAY_BUFFER, _vnormal);
            glVertexAttribPointer(vnormal_id, 3 /*vec3*/, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);
            check_error_gl();
        }

        ///--- Textures
        ///---- TODO ---
        /// TODO 2.1 Bind _tex1D for toon shading
        /// TODO 3.1 Bind _tex2D for artistic shading
        

        if(_pid == _pid_toon) {
			glBindTexture(GL_TEXTURE_1D, _tex1D);
			glActiveTexture(GL_TEXTURE0);
			GLuint id = glGetUniformLocation(_pid, "_tex1D");
			glUniform1i(id,0);
		}
        
        if(_pid == _pid_art) {
			glBindTexture(GL_TEXTURE_2D, _tex2D);
			glActiveTexture(GL_TEXTURE1);
			GLuint id = glGetUniformLocation(_pid, "_tex2D");
			glUniform1i(id,1);
		}
	}

    void unbindShader(GLuint _pid) {
        GLint vpoint_id = glGetAttribLocation(_pid, "vpoint");
        if (vpoint_id >= 0)
            glDisableVertexAttribArray(vpoint_id);
        GLint vnormal_id = glGetAttribLocation(_pid, "vnormal");
        if (vnormal_id >= 0)
            glDisableVertexAttribArray(vnormal_id);
        glUseProgram(0);
        glBindVertexArray(0);
    }

public:    
    void init(const std::string& filename){
        check_error_gl();
        assert(mesh.read(filename.c_str()));
        mesh.triangulate();
        mesh.update_vertex_normals();
        printf("Loaded mesh '%s' (#V=%d, #F=%d)\n", filename.c_str(), mesh.n_vertices(), mesh.n_faces());
        
        ///--- Vertex one vertex Array
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);
        check_error_gl();        
        
        ///--- Vertex Buffer
        Surface_mesh::Vertex_property<Point> vpoints = mesh.get_vertex_property<Point>("v:point");
        glGenBuffers(ONE, &_vpoint);
        glBindBuffer(GL_ARRAY_BUFFER, _vpoint);
        glBufferData(GL_ARRAY_BUFFER, mesh.n_vertices() * sizeof(vec3), vpoints.data(), GL_STATIC_DRAW);
        check_error_gl();        
    
        ///--- Normal Buffer
        Surface_mesh::Vertex_property<Normal> vnormals = mesh.get_vertex_property<Normal>("v:normal");
        glGenBuffers(ONE, &_vnormal);
        glBindBuffer(GL_ARRAY_BUFFER, _vnormal);
        glBufferData(GL_ARRAY_BUFFER, mesh.n_vertices() * sizeof(vec3), vnormals.data(), GL_STATIC_DRAW);
        check_error_gl();        
    
        ///--- Index Buffer
        std::vector<unsigned int> indices;
        for(Surface_mesh::Face_iterator fit = mesh.faces_begin(); fit != mesh.faces_end(); ++fit) {
            unsigned int n = mesh.valence(*fit);
            Surface_mesh::Vertex_around_face_circulator vit = mesh.vertices(*fit);
            for(unsigned int v = 0; v < n; ++v) {
                indices.push_back((*vit).idx());
                ++vit;
            }
        }

        GLuint _vbo_indices;
        glGenBuffers(ONE, &_vbo_indices);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo_indices);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        check_error_gl();        

        ///--- 1D texture for toon shader
        glGenTextures(1, &_tex1D);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_1D, _tex1D);
        glTexImage1D(GL_TEXTURE_1D, 0, GL_RED, NB_COLOR, 0, GL_RED, GL_FLOAT, tex);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        check_error_gl();

        ///--- 2D texture for artistic shader
        glGenTextures(1, &_tex2D);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, _tex2D);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TEX_WIDTH, TEX_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, TEX_PIXEL_DATA);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        check_error_gl();

        ///--- Unbind
        glBindVertexArray(0);

        ///--- Compile the shaders
        _pid_gouraud = opengp::load_shaders("gouraud_vshader.glsl", "gouraud_fshader.glsl");
        if(!_pid_gouraud) exit(EXIT_FAILURE);

        _pid_phong = opengp::load_shaders("phong_vshader.glsl", "phong_fshader.glsl");
        if(!_pid_phong) exit(EXIT_FAILURE);

        _pid_toon = opengp::load_shaders("toon_vshader.glsl", "toon_fshader.glsl");
        if(!_pid_toon) exit(EXIT_FAILURE);

        _pid_art = opengp::load_shaders("art_vshader.glsl", "art_fshader.glsl");
        if(!_pid_art) exit(EXIT_FAILURE);

        _pid_flat = opengp::load_shaders("flat_vshader.glsl", "flat_fshader.glsl");
        if(!_pid_flat) exit(EXIT_FAILURE);

        _pid_phong_spot_light = opengp::load_shaders("spot_vshader.glsl", "spot_fshader.glsl");
        if(!_pid_phong_spot_light) exit(EXIT_FAILURE);

        ///--- Initial shader.
        _pid_active = _pid_gouraud;
    }
           
    void cleanup(){
        /// TODO
    }
    
    void use_gouraud_shader() {
        _pid_active = _pid_gouraud;
    }

    void use_flat_shader() {
        _pid_active = _pid_flat;
    }

    void use_phong_shader() {
        _pid_active = _pid_phong;
    }

    void use_toon_shader() {
        _pid_active = _pid_toon;
    }

    void use_artistic_shader() {
        _pid_active = _pid_art;
    }

    void use_spot_light_phong_shader() {
        _pid_active = _pid_phong_spot_light;
    }

    void draw(const mat4& /*model*/, const mat4& view, const mat4& projection){

        float scale = 1.0;
        Transform _M = Transform::Identity();
        _M *= Eigen::Translation3f(0,0,.5);
        _M *= Eigen::AngleAxisf(glfwGetTime(), Eigen::Vector3f::UnitZ());
        _M *= Eigen::AlignedScaling3f(scale, scale, scale);
        mat4 model = _M.matrix();

        bindShader(_pid_active);

        Material::setup(_pid_active);
        Light::setup(_pid_active);

        // set up spot light if needed
        GLint spot_dir_id = glGetUniformLocation(_pid_active, "spot_dir");
        if (spot_dir_id >=0) {
            vec3 spot_dir = Light::get_spot_direction(glfwGetTime());
            glUniform3fv(spot_dir_id, ONE, spot_dir.data());
        }

        ///--- Setup matrix stack
        GLint model_id = glGetUniformLocation(_pid_active, "model");
        glUniformMatrix4fv(model_id, ONE, DONT_TRANSPOSE, model.data());
        GLint view_id = glGetUniformLocation(_pid_active, "view");
        glUniformMatrix4fv(view_id, ONE, DONT_TRANSPOSE, view.data());
        GLint projection_id = glGetUniformLocation(_pid_active, "projection");
        glUniformMatrix4fv(projection_id, ONE, DONT_TRANSPOSE, projection.data());
        glDrawElements(GL_TRIANGLES, /*#vertices*/ 3*mesh.n_faces(), GL_UNSIGNED_INT, ZERO_BUFFER_OFFSET);
        unbindShader(_pid_active);

        glBindVertexArray(0);
        glUseProgram(0);
    }
};
