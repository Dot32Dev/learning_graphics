#include <stdio.h>
#include <stdlib.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

int main(void) {
	if (!glfwInit()) return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "My thing", NULL, NULL);
	if (!window) return -2;

	glfwMakeContextCurrent(window);
	gladLoadGLLoader(glfwGetProcAddress);

	float xscale, yscale;
    glfwGetWindowContentScale(window, &xscale, &yscale);

	glViewport(0, 0, 800 * xscale, 600 * yscale);
	glClearColor(0.5, 0.5, 0.5, 1.0);

	// Get shaders
	FILE* frag_file = fopen("res/frag.glsl", "r");
	fseek(frag_file, 0L, SEEK_END);
	size_t frag_size = ftell(frag_file);
	rewind(frag_file);
	char* frag = (char*)malloc(frag_size + 1); // +1 for null terminator
	fread(frag, 1, frag_size, frag_file);
	frag[frag_size] = '\0';
	fclose(frag_file);

	FILE* vert_file = fopen("res/vert.glsl", "r");
	fseek(vert_file, 0L, SEEK_END);
	size_t vert_size = ftell(vert_file);
	rewind(vert_file);
	char* vert = (char*)malloc(vert_size + 1); // +1 for null terminator
	fread(vert, 1, vert_size, vert_file);
	vert[vert_size] = '\0';
	fclose(vert_file);

	// Vertices
	GLfloat vertices[] = {
		0.0, 0.5, 0.0,
		0.5, -0.5, 0.0,
		-0.5, -0.5, 0.0,
	};

	GLuint program = glCreateProgram();

	GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert_shader, 1, &vert, NULL);
	glCompileShader(vert_shader);
	glAttachShader(program, vert_shader);
	glDeleteShader(vert_shader);

	GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag_shader, 1, &frag, NULL);
	glCompileShader(frag_shader);
	glAttachShader(program, frag_shader);
	glDeleteShader(frag_shader);

	glLinkProgram(program);

	GLuint VBO; // Would normally be an array but we only have one so yeah
	GLuint VAO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO); // We only have one object
	
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // steam, static or dynamic

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(program);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(program);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}