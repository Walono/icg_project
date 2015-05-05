#pragma once
#include "icg_common.h"

struct Light{
	vec3 Ia = vec3(1.0f, 1.0f, 1.0f);
	vec3 Id = vec3(1.0f, 1.0f, 1.0f);
	vec3 Is = vec3(1.0f, 1.0f, 1.0f);

	vec3 light_pos = vec3(1.0f, 1.0f, 1.0f);

	///--- Pass light properties to the shader
	void setup(GLuint _pid){
		glUseProgram(_pid);
		
		GLuint light_pos_id = glGetUniformLocation(_pid, "light_pos"); //Given in camera space
		glUniform3fv(light_pos_id, ONE, light_pos.data());
		GLuint Ia_id = glGetUniformLocation(_pid, "Ia");
		glUniform3fv(Ia_id, ONE, Ia.data());
		GLuint Id_id = glGetUniformLocation(_pid, "Id");
		glUniform3fv(Id_id, ONE, Id.data());
		GLuint Is_id = glGetUniformLocation(_pid, "Is");
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

class Terrain : public Material, public Light{
protected:
    GLuint _vao;          ///< vertex array object
    GLuint _vbo_position; ///< memory buffer for positions
    GLuint _vbo_index;    ///< memory buffer for indice
    GLuint _pid;          ///< GLSL shader program ID
    GLuint _heightmap;    ///< Texture ID
    vector<GLuint> _tabMixingTex;
	GLuint _tex1;		  //<- Texure
	GLuint _tex2;		  //<- Texure
	GLuint _tex3;		  //<- Texure
	GLuint _tex4;
    GLuint _num_indices;  ///< number of vertices to render
    Light light;
    
public:    
	void init(GLuint heightmapTex, vector<GLuint> tabMixingTex){
        // Compile the shaders
        _pid = opengp::load_shaders("_terrain/terrain_vshader.glsl", "_terrain/terrain_fshader.glsl");
        if(!_pid) exit(EXIT_FAILURE);       
        glUseProgram(_pid);
        
        // Vertex one vertex Array
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);
        
        // Vertex coordinates and indices
        {
            std::vector<GLfloat> vertices;
            std::vector<GLuint> indices;

            int terrain_dim = 1024;

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
		this->_heightmap = heightmapTex;
		glBindTexture(GL_TEXTURE_2D, _heightmap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		GLuint heightmap_id = glGetUniformLocation(_pid, "heightmap");
		glUniform1i(heightmap_id, 0);

		// Load texture
		glGenTextures(1, &_tex1);		
		glBindTexture(GL_TEXTURE_2D, _tex1);
		glfwLoadTexture2D("_terrain/texture-herbe.tga", 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		GLuint grass_id = glGetUniformLocation(_pid, "grass");
		glUniform1i(grass_id, 1 /*GL_TEXTURE1*/);

		glGenTextures(1, &_tex2);
		glBindTexture(GL_TEXTURE_2D, _tex2);
		glfwLoadTexture2D("_terrain/snow.tga", 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		GLuint snow_id = glGetUniformLocation(_pid, "snow");
		glUniform1i(snow_id, 2 /*GL_TEXTURE2*/);

		glGenTextures(1, &_tex3);
		glBindTexture(GL_TEXTURE_2D, _tex3);
		glfwLoadTexture2D("_terrain/rock2.tga", 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		GLuint rock_id = glGetUniformLocation(_pid, "rock");
		glUniform1i(rock_id, 3 /*GL_TEXTURE3*/);
		
		///bind texture for mix
		this ->_tabMixingTex = tabMixingTex;
		glActiveTexture(GL_TEXTURE4);
		//glGenTextures(1, &_tabMixingTex.at(0));
		glBindTexture(GL_TEXTURE_2D, _tabMixingTex.at(0));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		GLuint mixTexTerrain_id = glGetUniformLocation(_pid, "mixTexTerrain");
		glUniform1i(mixTexTerrain_id, 4);
		
		glActiveTexture(GL_TEXTURE6);
		//glGenTextures(1, &_tabMixingTex.at(0));
		glBindTexture(GL_TEXTURE_2D, _tabMixingTex.at(1));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		GLuint mixSnowGrass_id = glGetUniformLocation(_pid, "mixSnowGrass");
		glUniform1i(mixSnowGrass_id, 6);
		
		glGenTextures(1, &_tex4);
		glBindTexture(GL_TEXTURE_2D, _tex4);
		glfwLoadTexture2D("_terrain/montagneNeige2.tga", 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		GLuint grassSnow_id = glGetUniformLocation(_pid, "grassSnow");
		glUniform1i(grassSnow_id, 5 /*GL_TEXTURE5*/);
		

        
        
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
    
    void draw(const mat4& model, const mat4& view, const mat4& projection, float time){
        glUseProgram(_pid);
        glBindVertexArray(_vao);
        // Bind textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _heightmap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _tex1);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, _tex2);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, _tex3);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D,_tabMixingTex.at(0));
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D,_tex4);
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D,_tabMixingTex.at(1));
		
		Material::setup(_pid);
        Light::setup(_pid);

        // Setup MVP
        mat4 P = projection;
        mat4 MV = view*model;
        GLuint P_id = glGetUniformLocation(_pid, "P");
        glUniformMatrix4fv(P_id, 1, GL_FALSE, P.data());
        
        GLuint MV_id = glGetUniformLocation(_pid, "MV");
        glUniformMatrix4fv(MV_id, 1, GL_FALSE, MV.data());

        // Pass the current time stamp to the shader.
        glUniform1f(glGetUniformLocation(_pid, "time"), time);
        

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
