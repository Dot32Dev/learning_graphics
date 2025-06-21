#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <glad/glad.h>

int main(void) {
	if (!glfwInit()) printf("Fuck");
	GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
	if (!window) printf("Fuck");

	glfwMakeContextCurrent(window);
	gladLoadGLLoader(glfwGetProcAddress);

	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
}