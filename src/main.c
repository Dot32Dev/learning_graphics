#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

int main(void) {
	int grid[20][15];
	for (int x=0; x<20; x++) {
		for (int y=0; y<15; y++) {
			grid[x][y] = 0;
		}
	}

	// Graphics
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
	// GLfloat vertices[] = {
	// 	0.0, 150.0, 0.0,
	// 	150.0, -150.0, 0.0,
	// 	-150.0, -150.0, 0.0,
	// };
	GLfloat vertices[] = {
		// positions          // texture coords
		40.0f, -40.0f, 0.0f,     1.0f, 1.0f,   // top right
		40.0f,  0.0f,  0.0f,     1.0f, 0.0f,   // bottom right
		00.0f,  0.0f,  0.0f,     0.0f, 0.0f,   // bottom left

		00.0f,  0.0f,  0.0f,     0.0f, 0.0f,   // bottom left
		00.0f, -40.0f, 0.0f,     0.0f, 1.0f,    // top left
		40.0f, -40.0f, 0.0f,     1.0f, 1.0f,   // top right 
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);  

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	int scaleLocation = glGetUniformLocation(program, "scale");
	int positionLocation = glGetUniformLocation(program, "position");

	// Textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int imageWidth, imageHeight, nrChannels;
	unsigned char *data = stbi_load("res/tileset.png", &imageWidth, &imageHeight, &nrChannels, 4);

	unsigned int texture;
	glGenTextures(1, &texture);  
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	stbi_image_free(data);

	glUseProgram(program);
	glBindVertexArray(VAO);
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		int width, height;
		glfwGetWindowSize(window, &width, &height);
		glUniform2f(scaleLocation, width, height);

		for (int x=0; x<20; x++) {
			for (int y=0; y<15; y++) {
				if (grid[x][y] == 1) {
					glUniform2f(positionLocation, x*40, y*40);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
			}
		}

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		xpos = floor(xpos/40)*40;
		ypos = floor(ypos/40)*40;

		int mouse = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
		if (mouse == GLFW_PRESS) {
			grid[(int)xpos/40][(int)ypos/40] = 1;
		}

		glUniform2f(positionLocation, xpos, ypos);

		// glBindTexture(GL_TEXTURE_2D, texture);
		
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
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