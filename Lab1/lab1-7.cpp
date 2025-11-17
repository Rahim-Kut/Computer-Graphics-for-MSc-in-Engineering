
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


// Read the code for the shader programs from file
std::string vertex_shader_str = readFile("../lab1-6_vs.glsl");
std::string fragment_shader_str = readFile("../lab1-6_fs.glsl");
GLuint shader_program;

float x_rotate_angle = 0.0f;
float y_rotate_angle = 0.0f;

glm::vec3 camPos(0.0f, 0.0f, 2.0f);
float camYaw = 0.0f; // Y axis
float camPitch = 0.0f; // X axis

// Helper function to forward shader compilation errors to terminal.
void checkShaderCompileError(GLint shaderID)
{
	GLint isCompiled = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_FALSE)
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
void loadShaders(std::string vertex_shader_str,
	std::string fragment_shader_str)
{
	const char* vertex_shader_src = vertex_shader_str.c_str();
	const char* fragment_shader_src = fragment_shader_str.c_str();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader_src, NULL);
	glCompileShader(vs);
	checkShaderCompileError(vs);

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader_src, NULL);
	glCompileShader(fs);
	checkShaderCompileError(fs);

	shader_program = glCreateProgram();
	glAttachShader(shader_program, fs);
	glAttachShader(shader_program, vs);
	glLinkProgram(shader_program);
	glDeleteShader(vs);
	glDeleteShader(fs);
	glUseProgram(shader_program);
}

// Write out error in terminal in case init failed.
static void error_callback(int error, const char* description)
{
	std::cerr << description;
}


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//-------------------------------------------------------------------------//
	// COPY YOUR CODE FROM BEFORE HERE
	// Update rotation angle here, for example
	if ((key == GLFW_KEY_RIGHT) && ((action == GLFW_PRESS) || action == GLFW_REPEAT)) {
		y_rotate_angle += 0.1f;
	}
	if ((key == GLFW_KEY_LEFT) && ((action == GLFW_PRESS) || action == GLFW_REPEAT)) {
		y_rotate_angle -= 0.1f;
	}
	if ((key == GLFW_KEY_UP) && ((action == GLFW_PRESS) || action == GLFW_REPEAT)) {
		x_rotate_angle += 0.1f;
	}
	if ((key == GLFW_KEY_DOWN) && ((action == GLFW_PRESS) || action == GLFW_REPEAT)) {
		x_rotate_angle -= 0.1f;
	}

	// Camera rotation (I/K pitch, J/L yaw)
	if ((key == GLFW_KEY_J) && ((action == GLFW_PRESS) || action == GLFW_REPEAT)) {
		camYaw -= 0.1;
	}
	if ((key == GLFW_KEY_L) && ((action == GLFW_PRESS) || action == GLFW_REPEAT)) {
		camYaw += 0.1;
	}
	if ((key == GLFW_KEY_I) && ((action == GLFW_PRESS) || action == GLFW_REPEAT)) {
		camPitch -= 0.1;
	}
	if ((key == GLFW_KEY_K) && ((action == GLFW_PRESS) || action == GLFW_REPEAT)) {
		camPitch += 0.1;
	}

	// camera translation (WASD RF)
	glm::vec3 forward = glm::vec3(sinf(camYaw), 0.0f, -cosf(camYaw));
	glm::vec3 right = glm::vec3(cosf(camYaw), 0.0f, sinf(camYaw));

	if ((key == GLFW_KEY_W) && ((action == GLFW_PRESS) || action == GLFW_REPEAT)) {
		camPos += 0.1f * forward;
	}
	if ((key == GLFW_KEY_S) && ((action == GLFW_PRESS) || action == GLFW_REPEAT)) {
		camPos -= 0.1f * forward;
	}
	if ((key == GLFW_KEY_A) && ((action == GLFW_PRESS) || action == GLFW_REPEAT)) {
		camPos -= 0.1f * right;
	}
	if ((key == GLFW_KEY_D) && ((action == GLFW_PRESS) || action == GLFW_REPEAT)) {
		camPos += 0.1f * right;
	}
	if ((key == GLFW_KEY_R) && ((action == GLFW_PRESS) || action == GLFW_REPEAT)) {
		camPos.y += 0.1f;
	}
	if ((key == GLFW_KEY_F) && ((action == GLFW_PRESS) || action == GLFW_REPEAT)) {
		camPos.y -= 0.1f;
	}

	//-------------------------------------------------------------------------//
}

static void scroll_callback(GLFWwindow* window, double scroll_v, double scroll_h)
{
};

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}




int main(int argc, char const* argv[])
{
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);

	int w_width = 800;
	int w_height = 600;
	GLFWwindow* window = glfwCreateWindow(w_width, w_height, "Hello Cube", NULL, NULL);
	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();

	loadShaders(vertex_shader_str, fragment_shader_str);
	GLint uMVP = glGetUniformLocation(shader_program, "uMVP");

	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

	//-------------------------------------------------------------------------//
	// COPY FROM lab1-5
	// Geometry, VBO, EBO, VAO
	float points[] = {
		-0.5, -0.5, -0.5,
		 0.5, -0.5, -0.5,
		 0.5,  0.5, -0.5,
		-0.5,  0.5, -0.5,
		-0.5, -0.5,  0.5,
		 0.5, -0.5,  0.5,
		 0.5,  0.5,  0.5,
		-0.5,  0.5,  0.5
	};

	// 12 triangular faces (vertexes that make up one triangle are listed in counter-clockwise order)
	unsigned short faces[] = {
		0, 3, 2,
		0, 2, 1,
		4, 6, 7,
		4, 5, 6,
		0, 7, 3,
		0, 4, 7,
		1, 6, 5,
		1, 2, 6,
		0, 5, 4,
		0, 1, 5,
		3, 6, 2,
		3, 7, 6
	};

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);


	GLuint ebo = 0;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);
	//-------------------------------------------------------------------------//


	while (!glfwWindowShouldClose(window))
	{
		// Update other events like keyboard/mouse input.
		glfwPollEvents();
		// Clear the drawing surface before drawing something new.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwGetFramebufferSize(window, &w_width, &w_height);

		//-----------------------------------------------------------------------//
		// YOUR CODE GOES HERE
		glm::mat4 Projection = glm::perspective(glm::radians(45.0f), float(w_width) / w_height, 1.0f, 100.0f);

		// Build View as inverse of camera transform


		glm::mat4 View(1.0f);
		View = glm::rotate(View, -camPitch, glm::vec3(1.0f, 0.0f, 0.0f));
		View = glm::rotate(View, -camYaw, glm::vec3(0.0f, 1.0f, 0.0f));
		View = glm::translate(View, -camPos);

		glm::vec3 rotate(x_rotate_angle, y_rotate_angle, 0.0f);
		glm::mat4 Model(1.0f);
		Model = glm::rotate(Model, rotate.x, glm::vec3(1.0f, 0.0f, 0.0f));
		Model = glm::rotate(Model, rotate.y, glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 MVP = Projection * View * Model;
		glUniformMatrix4fv(uMVP, 1, GL_FALSE, glm::value_ptr(MVP));
		// -----------------------------------------------------------------------//

		//-----------------------------------------------------------------------//
		// Call glDrawElements as before
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
		//-----------------------------------------------------------------------//

		glfwSwapBuffers(window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

