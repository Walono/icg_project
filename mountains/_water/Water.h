#pragma once
#include "icg_common.h"


class Water : public Material, public Light{
protected:
    GLuint _vao;          ///< vertex array object
    GLuint _vbo_position; ///< memory buffer for positions
    GLuint _vbo_index;    ///< memory buffer for indice
    GLuint _pid;          ///< GLSL shader program ID
    GLuint _heightmap;    ///< Texture ID
	GLuint _tex_mirror;	  ///< Texture ID
    GLuint _num_indices;  ///< number of vertices to render
    
public:    
	void init(GLuint tex_heightMap, GLuint tex_mirror){
        // Compile the shaders
        _pid = opengp::load_shaders("_water/water_vshader.glsl", "_water/water_fshader.glsl");
        if(!_pid) exit(EXIT_FAILURE);       
        glUseProgram(_pid);
        
        // Vertex one vertex Array
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);
     
        // Vertex coordinates and indices
        {
            std::vector<GLfloat> vertices;
            std::vector<GLuint> indices;

            int terrain_dim = 100;

			//Generate the mesh            
			for (int i = 0; i <= terrain_dim; i++){
				for (int j = 0; j <= terrain_dim; j++){
					vertices.push_back(-1.0f + i * 2 / (float)terrain_dim); vertices.push_back(-1.0f + j * 2 / (float)terrain_dim);
				}
			}

			int indice = 0;
			for (int row = 0; row<terrain_dim; row++){
				for (int col = 0; col<terrain_dim; col++){
					indices.push_back(indice);
					indices.push_back(indice + 1);
					indices.push_back(indice + terrain_dim + 2);
					indices.push_back(indice);
					indices.push_back(indice + terrain_dim + 1);
					indices.push_back(indice + terrain_dim + 2);
					indice++;
				}
				indice++;
			}
            _num_indices = indices.size();

            // position buffer
            glGenBuffers(1, &_vbo_position);
            glBindBuffer(GL_ARRAY_BUFFER, _vbo_position);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

            // vertex indices
            glGenBuffers(1, &_vbo_index);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo_index);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

            // position shader attribute
            GLuint loc_position = glGetAttribLocation(_pid, "position");
            glEnableVertexAttribArray(loc_position);
            glVertexAttribPointer(loc_position, 2, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);
        }

		///--- Load/Assign Hightmap texture
		this->_heightmap = tex_heightMap;
		glBindTexture(GL_TEXTURE_2D, _heightmap);
		GLuint heightmap_id = glGetUniformLocation(_pid, "heightmap");
		glUniform1i(heightmap_id, 0);

		//--- Texture uniforms
		this-> _tex_mirror = tex_mirror;
		GLuint tex_mirror_id = glGetUniformLocation(_pid, "tex_mirror");
		glUniform1i(tex_mirror_id, 1);
		
		
		
		Material::setup(_pid);
        Light::setup(_pid);
        
        // to avoid the current object being polluted
        glBindVertexArray(0);
		glUseProgram(0);
    }
           
    void cleanup(){
        glDeleteBuffers(1, &_vbo_position);
        glDeleteBuffers(1, &_vbo_index);
        glDeleteVertexArrays(1, &_vao);
        glDeleteProgram(_pid);
		glDeleteTextures(1, &_heightmap);
    }
    
    void draw(const mat4& model, const mat4& view, const mat4& projection, float time, vec3 light_pos){
        glUseProgram(_pid);
        glBindVertexArray(_vao);
        // Bind textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _heightmap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _tex_mirror);
		
	Light::set_spot_direction(light_pos);
		GLuint light_pos_id = glGetUniformLocation(_pid, "light_pos"); //Given in camera space
		glUniform3fv(light_pos_id, ONE, Light::get_spot_direction(time).data());

       // Setup MVP
        mat4 P = projection;
        mat4 MV = view*model;
        GLuint P_id = glGetUniformLocation(_pid, "P");
        glUniformMatrix4fv(P_id, 1, GL_FALSE, P.data());
        
        GLuint MV_id = glGetUniformLocation(_pid, "MV");
        glUniformMatrix4fv(MV_id, 1, GL_FALSE, MV.data());

        // Pass the current time stamp to the shader.
        glUniform1f(glGetUniformLocation(_pid, "time"), time);
        

		// Enable blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Draw
        // TODO 5: For debugging it can be helpful to draw only the wireframe.
        // You can do that by uncommenting the next line.
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        // TODO 5: Depending on how you set up your vertex index buffer, you
        // might have to change GL_TRIANGLE_STRIP to GL_TRIANGLES.
        glDrawElements(GL_TRIANGLES, _num_indices, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);        
        glUseProgram(0);
    }
};