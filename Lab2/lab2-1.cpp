// Replace with your own code, starting from what you had in Lab 1,
// but use the geometry defined below.

#include <GL/glew.h>
#include <cmath>

#include <GLFW/glfw3.h>
#include <cstdlib>  
#include <iostream>
#include "readfile.hpp"

// Include the GLM library for easier manipulation of transformations
#define GLM_FORCE_RADIANS
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> //glm::value_ptr
#include <glm/geometric.hpp> 


// Read the code for the shader programs from file
std::string vertex_shader_str =	readFile("../lab2-1_vs.glsl");
std::string fragment_shader_str =	readFile("../lab2-1_fs.glsl");
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
	
	glEnable (GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"

	float points[] = {
		// A cube has 8 vertices, but now we have three copies of each one:
		-0.5, -0.5, -0.5, 1, //0 0
		-0.5, -0.5, -0.5, 1, //0 1
		-0.5, -0.5, -0.5, 1, //0 2
			//
		-0.5, -0.5,  0.5, 1, //1 3
		-0.5, -0.5,  0.5, 1, //1 4
		-0.5, -0.5,  0.5, 1, //1 5
			//
		-0.5,  0.5, -0.5, 1, //2 6
		-0.5,  0.5, -0.5, 1, //2 7
		-0.5,  0.5, -0.5, 1, //2 8
			//
		-0.5,  0.5,  0.5, 1, //3 9
		-0.5,  0.5,  0.5, 1, //3 10
		-0.5,  0.5,  0.5, 1, //3 11
			//
		0.5, -0.5, -0.5, 1, //4 12
		0.5, -0.5, -0.5, 1, //4 13
		0.5, -0.5, -0.5, 1, //4 14
			//
		0.5, -0.5,  0.5, 1, //5 15
		0.5, -0.5,  0.5, 1, //5 16
		0.5, -0.5,  0.5, 1, //5 17
			//
		0.5,  0.5, -0.5, 1, //6 18
		0.5,  0.5, -0.5, 1, //6 19
		0.5,  0.5, -0.5, 1, //6 20
			//
		0.5,  0.5,  0.5, 1, //7 21
		0.5,  0.5,  0.5, 1, //7 22
		0.5,  0.5,  0.5, 1, //7 23
	};
	

	unsigned short faces[]= {
		// ... and 12 triangular faces, 
		// defined by the following vertex indices:
		0, 9, 6, // 0 3 2
		0, 3, 9, // 0 1 3
		//
		1, 7, 18, // 0 2 6
		1, 18, 12, // 0 6 4
		//
		13, 19, 15, // 4 6 5
		15, 19, 21, // 5 6 7
		//
		16, 22, 10, // 5 7 3
		16, 10, 4, // 5 3 1
		//
		8, 11, 23, // 2 3 7
		8, 23, 20, // 2 7 6
		//
		2, 14, 5, // 0 4 1
		5, 14, 17 // 1 4 5
	};

	float normals[24 * 4] = {0};
  	for (int i = 0; i < 36; i += 3) {
		unsigned short i0 = faces[i];
      	unsigned short i1 = faces[i + 1];
      	unsigned short i2 = faces[i + 2];

      	glm::vec3 p1(points[4 * i0], points[4 * i0 + 1], points[4 * i0 + 2]);
      	glm::vec3 p2(points[4 * i1], points[4 * i1 + 1], points[4 * i1 + 2]);
      	glm::vec3 p3(points[4 * i2], points[4 * i2 + 1], points[4 * i2 + 2]);

      	glm::vec3 n = glm::normalize(glm::cross(p2 - p1, p3 - p1));

      	unsigned short tri[3] = { i0, i1, i2 };
		for (int j = 0; j < 3; ++j) {
			normals[4 * tri[j]] = n.x;
			normals[4 * tri[j] + 1] = n.y;
			normals[4 * tri[j] + 2] = n.z;
			normals[4 * tri[j] + 3] = 0.0f;
		}
  	}


	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray (0);

	GLuint normal_vbo;
    glGenBuffers(1, &normal_vbo);
  	glBindBuffer(GL_ARRAY_BUFFER, normal_vbo);
  	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
  	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  	glEnableVertexAttribArray(1);

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);
	//-------------------------------------------------------------------------//
  

	while (!glfwWindowShouldClose (window)) 
	{
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
		View = translate(View, translate_cam);

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
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
		//-----------------------------------------------------------------------//

		glfwSwapBuffers (window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	exit(EXIT_SUCCESS);
}