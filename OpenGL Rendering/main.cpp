#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "Shaders.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Camera.h"
#include "Mesh.h"
#include "Model.h"

using namespace std;
using namespace glm;

#pragma region Settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
const char* GLSL_VERSION = "#version 420";

//Camera Settings
vec3 cam_pos = vec3(0, 0, 3.0f);
vec3 world_up = vec3(0, 1.0f, 0);
float cam_pitch = radians(-15.0f);
float cam_yaw = radians(180.0f);
Camera camera(cam_pos, cam_pitch, cam_yaw, world_up);
#pragma endregion

#pragma region Art Resources
float vertices[] = {
	// positions          // texture coords
	 0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
	 0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
	-0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
};
unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
};
#pragma endregion

#pragma region Functions
//键盘控制
bool mouse_enable = false;
void processInput(GLFWwindow* window) {
	//鼠标显示/隐藏
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		if (mouse_enable) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			mouse_enable = false;
		}			
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			mouse_enable = true;
		}
	}
	//camera控制
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.speedZ = -1.0f;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.speedZ = 1.0f;
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.speedX = -1.0f;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.speedX = 1.0f;
	}
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		camera.speedY = 1.0f;
	}
	else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		camera.speedY = -1.0f;
	}
	else {
		camera.speedZ = 0.0f;
		camera.speedX = 0.0f;
		camera.speedY = 0.0f;
	}
}

//鼠标控制
float lastX, lastY;
bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
	float deltaX, deltaY;
	if (firstMouse) {
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}
	deltaX = lastX - xPos;
	deltaY = lastY - yPos;
	lastX = xPos;
	lastY = yPos;
	camera.ProcessMouseMovement(deltaX, deltaY);
}

//读取纹理 internalFormat:显存中存储的格式 format:内存指针data中存储的格式 type:像素数据的bit depth
unsigned int LoadTextures(const char* texturePath, GLint internalFormat, GLenum format, int TextureSlot) {
	unsigned int TexBuffer;
	glGenTextures(1, &TexBuffer);
	glActiveTexture(GL_TEXTURE0 + TextureSlot);
	glBindTexture(GL_TEXTURE_2D, TexBuffer);

	int width, height, channel;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(texturePath, &width, &height, &channel, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "Load texture failed." << endl;
	}
	stbi_image_free(data);
	return TexBuffer;
}
#pragma endregion

int main(int argc, char* argv[]) {
	#pragma region Init
	//初始化GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//创建窗口
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Rendering", NULL, NULL);
	if (window == NULL) {
		printf("Open Window Failed.");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);

	//初始化GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		printf("Init GLEW Failed.");
		glfwTerminate();
		return -1;
	}
	glEnable(GL_DEPTH_TEST);

	//设置IMGUI
	//context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//style
	ImGui::StyleColorsDark();
	//Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(GLSL_VERSION);
	//state
	bool show_main_window = true;
	bool show_sub_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.6f, 1);

	//设置视口大小
	glViewport(0, 0, 1280, 720);
	#pragma endregion	

	#pragma region Shaders
	Shaders* shader = new Shaders("vertex.vert", "fragment.frag");
	#pragma endregion

	#pragma region Mesh and Model
	string exePath = argv[0];  //exe文件的路径
	#pragma endregion


	#pragma region VAO,VBO,EBO
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	#pragma endregion

	#pragma region MVP
	mat4 ModelMat, ViewMat, ProjectionMat;
	ProjectionMat = perspective(radians(45.0f), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);	
	ModelMat = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
	ModelMat = scale(ModelMat, glm::vec3(1.0f, 1.0f, 1.0f));
	shader->use();	
	shader->setMat4("ProjectionMat", ProjectionMat);
	#pragma endregion

	//rendering loop
	while (!glfwWindowShouldClose(window))
	{
		//input
		processInput(window);		

		//imgui start
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		#pragma region show imgui window
		static float f = 0.0f;
		static int counter = 0;
		
		ImGui::Begin("Settings");
		
		ImGui::Text("Rendering Settings");
		
		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
		ImGui::ColorEdit3("clear color", (float*)&clear_color);
		
		ImGui::Text("Load models");
		
		if (ImGui::Button("nanosuit")) {

		}		
		
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
		#pragma endregion

		#pragma region rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader->use();

		//update view matrix
		ViewMat = camera.GetViewMatrix();
		shader->setMat4("ModelViewMat", ViewMat * ModelMat);
		//draw
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		#pragma endregion	

		glfwSwapBuffers(window);
		glfwPollEvents();
		camera.UpdateCameraPosition();
	}

	//Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}