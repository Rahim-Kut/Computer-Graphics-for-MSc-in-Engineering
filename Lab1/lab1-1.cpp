
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdlib>  
#include <iostream>


// Write out error in terminal in case init failed.
static void error_callback(int error, const char* description)
{
	std::cerr << description;
}

// GLFW callback function to handle window closing.
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

// GLFW callback function to handle window resizing.
static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}





// Main comes here!
int main(int argc, char const *argv[])
{
	// First, start GL context and a window using the GLFW helper library.
	glfwSetErrorCallback(error_callback);
	if( !glfwInit() )
		exit(EXIT_FAILURE);
  
	GLFWwindow* window = glfwCreateWindow (640, 480,
	                                       "Hello Triangle", NULL, NULL);
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent (window);

	// OK, now we have a window we can draw in.  
	// Start GLEW, so that we can access all OpenGL functions.
	glewExperimental = GL_TRUE;
	glewInit ();
	
	// Shader code. Don't worry too much about this part until the next task.
	const char* vertex_shader =
		"#version 450\n"
		"layout(location=0) in vec3 vp;"
		"void main () {"
		"  gl_Position = vec4 (vp, 1.0);"
		"}";
  
	const char* fragment_shader =
		"#version 450\n"
		"out vec3 frag_colour;"
		"void main () {"
		"  frag_colour = vec3 (0.5, 0.0, 0.5);"
		"}";

	GLuint vs = glCreateShader (GL_VERTEX_SHADER);
	glShaderSource (vs, 1, &vertex_shader, NULL);
	glCompileShader (vs);
	
	GLuint fs = glCreateShader (GL_FRAGMENT_SHADER);
	glShaderSource (fs, 1, &fragment_shader, NULL);
	glCompileShader (fs);

	// Link the compiled shaders to crete the final shader program.
	GLuint shader_program = glCreateProgram ();
	glAttachShader (shader_program, fs);
	glAttachShader (shader_program, vs);
	glLinkProgram (shader_program);
	// (Now, the individual shaders can be deleted.)
	glDeleteShader(vs);
	glDeleteShader(fs);
	// Tell the GPU to use our newly compiled shader program.
	glUseProgram (shader_program);

	// Set a non-black background colour
	glClearColor(0.1, 0.2, 0.3, 0.0);


	//-------------------------------------------------------------------------//
	// YOUR CODE GOES HERE.
	// 1. Create geometry
	float points[] = {
		-0.6f, -0.6f, 0.0f,  // Vertex 1
		 0.6f, -0.6f, 0.0f,  // Vertex 2
		 0.0f,  0.6f, 0.0f   // Vertex 3
	};
	// 2-3. Create VAO and VBO
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// 4-6. Copy vertex data to VBO, configure the VAO
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,         
		0,                  
		(void*)0            
	);
	//-------------------------------------------------------------------------//

	
	// Loop until the window is closed.
	while (!glfwWindowShouldClose (window)) 
	{
		// Update other events like keyboard/mouse input.
		glfwPollEvents ();
		// Clear the drawing surface before drawing something new.
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
		//-----------------------------------------------------------------------//
		// YOUR CODE GOES HERE.
		// 7. Bind the VAO and issue an appropriate glDraw*() command.

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//-----------------------------------------------------------------------//

		//Refresh the displayed image.
		glfwSwapBuffers (window);
	}

	// Close GL context and any other GLFW resources.
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
