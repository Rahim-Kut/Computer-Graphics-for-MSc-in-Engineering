#include <cmath>
#include <GL/glew.h>
#include <cmath>

#include <GLFW/glfw3.h>
#include <cstdlib>  
#include <iostream>
#include "readfile.hpp"
#include "lodepng.h"

// Include the GLM library for easier manipulation of transformations
#define GLM_FORCE_RADIANS
#include<glm/vec2.hpp> // glm::vec2
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> //glm::value_ptr
#include <glm/geometric.hpp> 



// Read the code for the shader programs from file
std::string vertex_shader_str =	readFile("../lab3-3_vs.glsl");
std::string fragment_shader_str =	readFile("../lab3-3_fs.glsl");
GLuint shader_program;

float g_rotation[2] = {0.0, 0.0};
float offset = 0.03;


// Helper function to forward shader compilation errors to terminal.
void checkShaderCompileError(GLint shaderID)
{
	GLint isCompiled = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
 
	if(isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::string errorLog;
		errorLog.resize(maxLength);
		glGetShaderInfoLog(shaderID, maxLength, &maxLength, &errorLog[0]);

		std::cout << "shader compilation failed:" << std::endl;
		std::cout << errorLog << std::endl;
		return;
	}
	else
		std::cout << "shader compilation success." << std::endl;
    
	return;
}


// Load vertex and fragment shader on demand, and recompile the shader program
void loadShaders( std::string vertex_shader_str,
                  std::string fragment_shader_str )
{
	const char *vertex_shader_src = vertex_shader_str.c_str();
	const char *fragment_shader_src = fragment_shader_str.c_str();

	GLuint vs = glCreateShader (GL_VERTEX_SHADER);
	glShaderSource (vs, 1, &vertex_shader_src, NULL);
	glCompileShader (vs);
	checkShaderCompileError(vs);
  
	GLuint fs = glCreateShader (GL_FRAGMENT_SHADER);
	glShaderSource (fs, 1, &fragment_shader_src, NULL);
	glCompileShader (fs);
	checkShaderCompileError(fs);

	shader_program = glCreateProgram ();
	glAttachShader (shader_program, fs);
	glAttachShader (shader_program, vs);
	glLinkProgram (shader_program);
	glDeleteShader(vs);
	glDeleteShader(fs);
	glUseProgram (shader_program);
}

// Write out error in terminal in case init failed.
static void error_callback(int error, const char* description)
{
	std::cerr << description;
}


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{	
	if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
    
	if ((key == GLFW_KEY_R) && action == GLFW_PRESS)
	{
		loadShaders( vertex_shader_str, fragment_shader_str );
	}
	//-------------------------------------------------------------------------//
	// COPY YOUR CODE FROM BEFORE HERE
	// Update rotation angle here, for example
	if ((key == GLFW_KEY_RIGHT) && ( (action == GLFW_PRESS) || action==GLFW_REPEAT) ){
		g_rotation[1] +=  offset;
	} 
	if ((key == GLFW_KEY_LEFT) && ( (action == GLFW_PRESS) || action==GLFW_REPEAT) ){
		g_rotation[1] -=  offset;
	}   
	if ((key == GLFW_KEY_UP) && ( (action == GLFW_PRESS) || action==GLFW_REPEAT) ){
		g_rotation[0] -= offset;
	}   
	if ((key == GLFW_KEY_DOWN) && ( (action == GLFW_PRESS) || action==GLFW_REPEAT) ) {
		g_rotation[0] += offset;
	}
	//-------------------------------------------------------------------------//
}

static void scroll_callback(GLFWwindow* window, double scroll_v, double scroll_h)
{
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


int main(int argc, char const *argv[])
{
  glfwSetErrorCallback(error_callback);
	if( !glfwInit() )
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1); 
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


  
	int w_width = 800;
	int w_height = 600;
	GLFWwindow* window = glfwCreateWindow (w_width, w_height, "Hello Cube", NULL, NULL);
	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent (window);
                                  
	glewExperimental = GL_TRUE;
	glewInit ();

	loadShaders( vertex_shader_str, fragment_shader_str );

	GLuint texture_handle;
  glGenTextures(1, &texture_handle);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_handle);

  glUniform1i(glGetUniformLocation(shader_program, "tex_sampler"), 0);

  unsigned char* image_data = nullptr;
  unsigned image_w = 0;
  unsigned image_h = 0;

  unsigned err_code = lodepng_decode32_file(
    &image_data,
    &image_w,
    &image_h,
    "../../common/data/numberline_hires.png"
  );

  if (!err_code){
    std::cout << "Read " << image_w << "x " << image_h << "image\n";

    glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_RGBA,
      image_w,
      image_h,
      0,
      GL_RGBA,
      GL_UNSIGNED_BYTE,
      image_data
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }
  else{
    std:: cerr << "Failed to read texture: " << lodepng_error_text(err_code) << std::endl;
  }
  free(image_data);

  GLuint env_texture;
  glGenTextures(1, &env_texture);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_CUBE_MAP, env_texture);
  glUniform1i(glGetUniformLocation(shader_program, "env_sampler"), 1);

  const char* cube_paths[6] = {
    "../../common/data/cube_right.png",
    "../../common/data/cube_left.png",
    "../../common/data/cube_up.png",
    "../../common/data/cube_down.png",
    "../../common/data/cube_back.png",
    "../../common/data/cube_front.png",
  };

  GLenum cube_targets[6] = {
    GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
  };

  for (int i = 0; i < 6; ++i){
    unsigned char* cube_data = nullptr;
    unsigned cube_w = 0;
    unsigned cube_h = 0;

    unsigned cube_err = lodepng_decode32_file(
      &cube_data,
      &cube_w,
      &cube_h,
      cube_paths[i]
    );

    if (!cube_err){
      std::cout << "Read cubee face " << cube_paths[i] << " " << cube_w << " x " << cube_h << "\n";

      glTexImage2D(
        cube_targets[i],
        0,
        GL_RGBA,
        cube_w,
        cube_h,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        cube_data
      );
    }
    else{
      std::cerr << "Failed to read cube face " << cube_paths[i] << ": " << lodepng_error_text(cube_err) << std::endl;
    }

    free(cube_data);
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  // Normal map texture
  GLuint normal_texture;
  glGenTextures(1, &normal_texture);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, normal_texture);
  glUniform1i(glGetUniformLocation(shader_program, "normal_sampler"), 2);

  unsigned char* normal_data = nullptr;
  unsigned normal_w = 0;
  unsigned normal_h = 0;
  unsigned normal_err = lodepng_decode32_file(
    &normal_data,
    &normal_w,
    &normal_h,
    "../../common/data/numberline_nmap_hires.png"
  );

  if(!normal_err){
    std::cout << "Read normal map" << normal_w << " x " << normal_h << "\n";

    glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_RGBA,
      normal_w,
      normal_h,
      0,
      GL_RGBA,
      GL_UNSIGNED_BYTE,
      normal_data
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }
  else{
    std::cerr << "Failed to read normal map : " << lodepng_error_text(normal_err) << std::endl;
  }
  free(normal_data);

	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"


  float t = (1.0f + sqrtf(5.0f))*0.25f;
  int num_faces = 20;
  int num_verts = 60;

  float points[] = {
    // An icosahedron without shared vertices has 20*3 vertices
    -0.5,  t,  0, 
    -0.5,  t,  0, 
    -0.5,  t,  0,
    -0.5,  t,  0,
    -0.5,  t,  0,
    
    0.5,  t,  0,
    0.5,  t,  0,
    0.5,  t,  0, 
    0.5,  t,  0, 
    0.5,  t,  0,
    
    -0.5, -t,  0, 
    -0.5, -t,  0, 
    -0.5, -t,  0,
    -0.5, -t,  0,
    -0.5, -t,  0,
    
    0.5, -t,  0,  
    0.5, -t,  0,  
    0.5, -t,  0, 
    0.5, -t,  0, 
    0.5, -t,  0,
    
    0, -0.5,  t,  
    0, -0.5,  t,  
    0, -0.5,  t, 
    0, -0.5,  t, 
    0, -0.5,  t,
    
    0,  0.5,  t,  
    0,  0.5,  t,  
    0,  0.5,  t, 
    0,  0.5,  t, 
    0,  0.5,  t,
    
    0, -0.5, -t,  
    0, -0.5, -t,  
    0, -0.5, -t, 
    0, -0.5, -t, 
    0, -0.5, -t,
    
    0,  0.5, -t,  
    0,  0.5, -t,  
    0,  0.5, -t, 
    0,  0.5, -t, 
    0,  0.5, -t,
    
    t,  0, -0.5,  
    t,  0, -0.5,  
    t,  0, -0.5, 
    t,  0, -0.5, 
    t,  0, -0.5,
    
    t,  0,  0.5,  
    t,  0,  0.5,  
    t,  0,  0.5, 
    t,  0,  0.5, 
    t,  0,  0.5,
    
    -t,  0, -0.5, 
    -t,  0, -0.5, 
    -t,  0, -0.5,
    -t,  0, -0.5,
    -t,  0, -0.5,
    
    -t,  0,  0.5, 
    -t,  0,  0.5, 
    -t,  0,  0.5,
    -t,  0,  0.5,
    -t,  0,  0.5
  };

  float uv_coords[]=
    {  
      0.072405, 0.144686,
      0.27133,  0.10033,
      0.481007, 0.129901,
      0.681948, 0.115116,
      0.928587, 0.107723,
      0.29888,  0.70651,
      0.449421, 0.736084,
      0.108695, 0.129901,
      0.847270, 0.699121,
      0.868776, 0.728691,
      0.576437, 0.721299,
      0.319044, 0.750869,
      0.548883, 0.736084,
      0.388265, 0.115116,
      0.397674, 0.713906,
      0.149690, 0.122508,
      0.347942, 0.115116,
      0.583157, 0.107723,
      0.780738, 0.100331,
      0.229663, 0.107723,
      0.33436,  0.68882,   
      0.181276, 0.721299,
      0.379528, 0.721299,
      0.190684, 0.122508,
      0.358023, 0.736084,
      0.103319, 0.713906,
      0.24512,  0.70651,
      0.083157, 0.699121,
      0.31006,  0.10251,
      0.222942, 0.721299,
      0.772001, 0.728691,
      0.618776, 0.721299,
      0.745120, 0.721299,
      0.433292, 0.115116,
      0.598614, 0.713906,
      0.520657, 0.721299,
      0.650362, 0.736084,
      0.700765, 0.728691,
      0.878184, 0.107723,
      0.671867, 0.743476,
      0.914475, 0.743476,
      0.819717, 0.736084, 
      0.201437, 0.721299,
      0.633561, 0.122508,
      0.796195, 0.743476,
      0.140953, 0.728691,
      0.122136, 0.706514,
      0.262593, 0.721299,
      0.157754, 0.728691,
      0.825765, 0.115116,
      0.723614, 0.728691,
      0.897001, 0.721299,
      0.499152, 0.736084,
      0.731007, 0.115116,
      0.469582, 0.721299,
      0.045523, 0.691729,
      0.967566, 0.721299,
      0.28341,  0.67157,
      0.532754, 0.122508,
      0.418507, 0.706514,
    };

  int num_indices = num_faces*3;
  unsigned short faces[]= {
    // ... and 20 triangular faces, 
    // defined by the following vertex indices:
    0,  55, 25,
    1,  26,  5,
    2,  6, 35,
    3,  36, 50,
    4,  51, 56,
    7,  27, 45,
    28, 57, 20,
    58, 52, 10,
    53, 37, 30,
    38,  8, 40,    
    15, 46, 21,
    16, 22, 11,
    17, 12, 31,
    18, 32, 41,
    19, 42, 47,
    23, 48, 29,
    13, 24, 59,
    33, 14, 54,
    43, 34, 39,
    49, 44, 9
  };

  float normals[60 * 3] = {0};
  float tangents[60 * 3] = {0};
  float bitangents[60 * 3] = {0};
  	for (int i = 0; i < num_indices; i += 3) {
      unsigned short i0 = faces[i];
      unsigned short i1 = faces[i + 1];
      unsigned short i2 = faces[i + 2];

      glm::vec3 p1(points[3 * i0], points[3 * i0 + 1], points[3 * i0 + 2]);
      glm::vec3 p2(points[3 * i1], points[3 * i1 + 1], points[3 * i1 + 2]);
      glm::vec3 p3(points[3 * i2], points[3 * i2 + 1], points[3 * i2 + 2]);

      glm::vec3 n = glm::normalize(glm::cross(p2 - p1, p3 - p1));

      glm::vec2 uv1(uv_coords[2 * i0], uv_coords[2 * i0 + 1]);
      glm::vec2 uv2(uv_coords[2 * i1], uv_coords[2 * i1 + 1]);
      glm::vec2 uv3(uv_coords[2 * i2], uv_coords[2 * i2 + 1]);

      glm::vec3 e1 = p2 - p1;
      glm::vec3 e2 = p3 - p1;
      glm::vec2 duv1 = uv2 - uv1;
      glm::vec2 duv2 = uv3 - uv1;

      float r = duv1.x * duv2.y - duv1.y * duv2.x;

      glm::vec3 tangent(1.0f, 0.0f, 0.0f);
      glm::vec3 bitangent(0.0f, 1.0f, 0.0f);

      if(std::abs(r) > 0.000001f){
        tangent = glm::normalize((duv2.y * e1 - duv1.y * e2) / r);
        bitangent = glm::normalize((duv1.x * e2 - duv2.x * e1) / r);
      }


      unsigned short tri[3] = { i0, i1, i2 };
      for (int j = 0; j < 3; ++j) {
        normals[3 * tri[j]] = n.x;
        normals[3 * tri[j] + 1] = n.y;
        normals[3 * tri[j] + 2] = n.z;
        
        tangents[3 * tri[j]] = tangent.x;
        tangents[3 * tri[j] + 1] = tangent.y;
        tangents[3 * tri[j] + 2] = tangent.z;

        bitangents[3 * tri[j]] = bitangent.x;
        bitangents[3 * tri[j] + 1] = bitangent.y;
        bitangents[3 * tri[j] + 2] = bitangent.z;
      }
  	}


	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray (0);

	GLuint normal_vbo;
  glGenBuffers(1, &normal_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, normal_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);

  GLuint uv_vbo;
  glGenBuffers(1, &uv_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, uv_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(uv_coords), uv_coords, GL_STATIC_DRAW);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(2);

  GLuint tangent_vbo;
  glGenBuffers(1, &tangent_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, tangent_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(tangents), tangents, GL_STATIC_DRAW);
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(3);

  GLuint bitangent_vbo;
  glGenBuffers(1, &bitangent_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, bitangent_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(bitangents), bitangents, GL_STATIC_DRAW);
  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(4);


	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);
	//-------------------------------------------------------------------------//
  

	while (!glfwWindowShouldClose (window)) {
    // Update other events like keyboard/mouse input.
		glfwPollEvents ();
		// Clear the drawing surface before drawing something new.
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwGetFramebufferSize(window, &w_width , &w_height );

		//-----------------------------------------------------------------------//
		// YOUR CODE GOES HERE
		//
		// Replace your hand-crafted matrices from lab1-5.cpp with ones
		// created by glm.
		//
		// Use glm::perspective to create a projection matrix
		//
		// Use glm::translate and glm::rotate to create the
		// model and view matrices.
		//
		// Multiply your matrices in the correct order to get a
		// modelViewProjection matrix and upload it to the appropriate
		// uniform variable in vertex shader.
		glm::mat4 Projection = glm::perspective(glm::radians(60.0f), float(w_width)/float(w_height), 1.0f, 100.0f);
		
		glm::vec3 rotate = glm::vec3(g_rotation[0], g_rotation[1], 0.0f);
		glm::vec3 translate_cam = glm::vec3(0.0f, 0.0f, -2.0f);

		glm::mat4 View(1.0f);
		View = glm::translate(View, translate_cam);

		glm::mat4 Model = glm::mat4(1.0f);
		Model = glm::rotate( Model, rotate.x, glm::vec3(1.0f, 0.0f, 0.0f));
		Model = glm::rotate( Model, rotate.y, glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 modelViewMatrix = View * Model;
		glm::mat4 modelViewProjectionMatrix = Projection * View * Model;

		GLint mvpLoc_mvp = glGetUniformLocation(shader_program, "modelViewProjectionMatrix");
		glUseProgram(shader_program);
		glUniformMatrix4fv(mvpLoc_mvp, 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix));

		GLint mvLoc_mv = glGetUniformLocation(shader_program, "modelViewMatrix");
  		glUniformMatrix4fv(mvLoc_mv, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
		// -----------------------------------------------------------------------//
	 
		//-----------------------------------------------------------------------//
		// Call glDrawElements as before
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT, 0);
		//-----------------------------------------------------------------------//

		glfwSwapBuffers (window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	exit(EXIT_SUCCESS);

}

