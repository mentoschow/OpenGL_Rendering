#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

void processInput(GLFWwindow* window) {
	//退出程序
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int main() {
	//初始化GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//创建窗口
	GLFWwindow* window = glfwCreateWindow(1280, 720, "OpenGL Rendering", NULL, NULL);
	if (window == NULL) {
		printf("Open Window Failed.");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//初始化GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		printf("Init GLEW Failed.");
		glfwTerminate();
		return -1;
	}
	//设置视口大小
	glViewport(0, 0, 1280, 720);

	//窗口循环
	while (!glfwWindowShouldClose(window))
	{
		//输出指令
		processInput(window);

		//渲染指令
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}