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
#include "Material.h"
#include "LightDirectional.h"
#include "LightPoint.h"
#include "LightSpot.h"

using namespace std;
using namespace glm;

#pragma region Settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
const char* GLSL_VERSION = "#version 420";
const ImVec4 white = ImVec4(1.0, 1.0, 1.0, 1.0);
const ImVec4 black = ImVec4(0.0, 0.0, 0.0, 1.0);
const ImVec4 red = ImVec4(1.0, 0.0, 0.0, 1.0);
const ImVec4 green = ImVec4(0.0, 1.0, 0.0, 1.0);
const ImVec4 blue = ImVec4(0.0, 0.0, 1.0, 1.0);
//screen data
float screenVertices[] = {
	// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
};

//Camera Settings
vec3 cam_pos = vec3(0, 0, 3.0f);
vec3 world_up = vec3(0, 1.0f, 0);
float cam_pitch = radians(0.0f);
float cam_yaw = radians(180.0f);
Camera camera(cam_pos, cam_pitch, cam_yaw, world_up); 
#pragma endregion

#pragma region Lightings
ImVec4 ambient_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
float ambient_strength = 0.1f;
float shininess = 32.0f;
//directional light
ImVec4 light_dir_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
vec3 light_dir_dir = vec3(0);
LightDirectional lightDirectional(vec3(light_dir_color.x, light_dir_color.y, light_dir_color.z), radians(light_dir_dir));
//point light
ImVec4 light_point_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
vec3 light_point_pos = vec3(0, 3, 3);
LightPoint lightPoint(light_point_pos, vec3(light_point_color.x, light_point_color.y, light_point_color.z));
//spot light
ImVec4 light_spot_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
vec3 light_spot_pos = vec3(0, 0, 3);
vec3 light_spot_dir = vec3(0);
float light_spot_cosphy_in = 0.9f;
float light_spot_cosphy_out = light_spot_cosphy_in - 0.05f;
LightSpot lightSpot(light_spot_pos, radians(light_spot_dir), vec3(light_spot_color.x, light_spot_color.y, light_spot_color.z), light_spot_cosphy_in, light_spot_cosphy_out);
#pragma endregion

#pragma region Phong
//Cube
float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};
ImVec4 object_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
bool show_phong_settings = false;
#pragma endregion

#pragma region Functions
//键盘控制
float deltaX, deltaY;
bool firstMouse = true;
void processInput(GLFWwindow* window) {
	
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
		camera.speedY = -1.0f;
	}
	else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		camera.speedY = 1.0f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		deltaX = 5.0f;
		camera.ProcessMouseMovement(deltaX, deltaY);
	}
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		deltaX = -5.0f;
		camera.ProcessMouseMovement(deltaX, deltaY);
	}
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		deltaY = 5.0f;
		camera.ProcessMouseMovement(deltaX, deltaY);
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		deltaY = -5.0f;
		camera.ProcessMouseMovement(deltaX, deltaY);
	}
	else {
		camera.speedZ = 0.0f;
		camera.speedX = 0.0f;
		camera.speedY = 0.0f;
	}
}

//鼠标控制

//void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
//	float deltaX, deltaY;
//	if (firstMouse) {
//		lastX = xPos;
//		lastY = yPos;
//		firstMouse = false;
//	}
//	deltaX = lastX - xPos;
//	deltaY = lastY - yPos;
//	lastX = xPos;
//	lastY = yPos;
//	camera.ProcessMouseMovement(deltaX, deltaY);
//}

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

	//初始化GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		printf("Init GLEW Failed.");
		glfwTerminate();
		return -1;
	}

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
	Shaders shader("vertex.vert", "fragment.frag");
	Shaders lightShader("lightCube.vert", "lightCube.frag");
	Shaders screenShader("screen.vert", "screen.frag");
	#pragma endregion

	#pragma region Material
	Material phong;
	phong.create_phong(shader, vec3(ambient_color.x, ambient_color.y, ambient_color.z), vec3(ambient_color.x, ambient_color.y, ambient_color.z), vec3(ambient_color.x, ambient_color.y, ambient_color.z), shininess);
	#pragma endregion

	#pragma region Model
	string exePath = argv[0];  //exe文件的路径
	#pragma endregion

	#pragma region VAO,VBO,EBO
	unsigned int VBO, cubeVAO, lightPointVAO;
	
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	
	//glGenBuffers(1, &EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenVertexArrays(1, &lightPointVAO);
	glBindVertexArray(lightPointVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//use FBO
	unsigned int screenVAO, screenVBO;
	glGenVertexArrays(1, &screenVAO);
	glGenBuffers(1, &screenVBO);
	glBindVertexArray(screenVAO);
	glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), &screenVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(0);
	#pragma endregion

	#pragma region FBO
	screenShader.use();
	screenShader.setInt("screenTexture", 0);

	unsigned int mainFBO;
	glGenFramebuffers(1, &mainFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mainFBO);

	unsigned int render_texture;
	glGenTextures(1, &render_texture);
	glBindTexture(GL_TEXTURE_2D, render_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, render_texture, 0);

	unsigned int fbo_depth_stencil;
	glGenRenderbuffers(1, &fbo_depth_stencil);
	glBindRenderbuffer(GL_RENDERBUFFER, fbo_depth_stencil);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fbo_depth_stencil);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	#pragma endregion

	#pragma region MVP
	mat4 ModelMat, ViewMat, ProjectionMat;
	ProjectionMat = perspective(radians(45.0f), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);	
	ModelMat = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
	ModelMat = scale(ModelMat, glm::vec3(1.0f, 1.0f, 1.0f));	
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

		bool rendering_window = true;

		#pragma region show imgui window	
		//main window
		ImGui::Begin("Settings", NULL);
		
		ImGui::Text("Rendering Type");
		if (ImGui::Button("Default")) {
			shader.reloadShader(&shader.ID, "vertex.vert", "fragment.frag");
		}
		if (ImGui::Button("Phong Model")) {
			shader.reloadShader(&shader.ID, "Phong.vert", "Phong.frag");
		}
		ImGui::SameLine();
		ImGui::Checkbox("Phong Model Settings", &show_phong_settings);

		if (ImGui::CollapsingHeader("Default Settings")) {
			ImGui::ColorEdit3("clear color", (float*)&clear_color);
			ImGui::ColorEdit3("object color", (float*)&object_color);
		}	
		if (ImGui::CollapsingHeader("Directional Light Settings")) {
			ImGui::Checkbox("Directional Light", &lightDirectional.enable);
			if (lightDirectional.enable) {
				ImGui::ColorEdit3("Directional light color", (float*)&light_dir_color);
				ImGui::SliderFloat("Directional light direction x", &light_dir_dir.x, -180.0f, 180.0f);
				ImGui::SliderFloat("Directional light direction y", &light_dir_dir.y, -180.0f, 180.0f);
				ImGui::SliderFloat("Directional light direction z", &light_dir_dir.z, -180.0f, 180.0f);
			}			
		}
		if (ImGui::CollapsingHeader("Point Light Settings")) {
			ImGui::Checkbox("Point Light", &lightPoint.enable);
			if (lightPoint.enable) {
				ImGui::ColorEdit3("Point light color", (float*)&light_point_color);
				ImGui::SliderFloat("Point light position x", &light_point_pos.x, -5.0f, 5.0f);
				ImGui::SliderFloat("Point light position y", &light_point_pos.y, -5.0f, 5.0f);
				ImGui::SliderFloat("Point light position z", &light_point_pos.z, -5.0f, 5.0f);
			}			
		}
		if (ImGui::CollapsingHeader("Spot Light Settings")) {
			ImGui::Checkbox("Spot Light", &lightSpot.enable);
			if (lightSpot.enable) {
				ImGui::ColorEdit3("Spot light color", (float*)&light_spot_color);
				ImGui::SliderFloat("Spot light position x", &light_spot_pos.x, -5.0f, 5.0f);
				ImGui::SliderFloat("Spot light position y", &light_spot_pos.y, -5.0f, 5.0f);
				ImGui::SliderFloat("Spot light position z", &light_spot_pos.z, -5.0f, 5.0f);
				ImGui::SliderFloat("Spot light direction x", &light_spot_dir.x, -180.0f, 180.0f);
				ImGui::SliderFloat("Spot light direction y", &light_spot_dir.y, -180.0f, 180.0f);
				ImGui::SliderFloat("Spot light direction z", &light_spot_dir.z, -180.0f, 180.0f);
				ImGui::SliderFloat("Spot light angle", &light_spot_cosphy_in, 0.5f, 0.99f);
			}			
		}
		
		ImGui::End();

		//window
		if (show_phong_settings)
		{
			ImGui::Begin("Phong Model Settings", &show_phong_settings);
			ImGui::ColorEdit3("ambient color", (float*)&ambient_color);
			ImGui::SliderFloat("ambient strength", &ambient_strength, 0.0f, 1.0f);
			ImGui::SliderFloat("shininess", &shininess, 1.0f, 64.0f);
			ImGui::End();
		}

		//rendering window
		{
			ImGui::Begin("Rendering Window");
			ImVec2 wsize = ImGui::GetWindowSize();
			ImGui::Image((ImTextureID)render_texture, wsize, ImVec2(0, 1), ImVec2(1, 0));
			ImGui::End();
		}
		#pragma endregion

		#pragma region rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);

		glBindFramebuffer(GL_FRAMEBUFFER, mainFBO);
		glEnable(GL_DEPTH_TEST);

		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//set uniform and draw
		//main
		shader.use();
		ViewMat = camera.GetViewMatrix();
		shader.setMat4("ModelMat", ModelMat);
		shader.setMat4("ProjectionMat", ProjectionMat);
		shader.setMat4("ViewMat", ViewMat);
		shader.setVec3("objColor", object_color.x, object_color.y, object_color.z);
		shader.setVec3("ambientColor", ambient_color.x, ambient_color.y, ambient_color.z);
		shader.setFloat("ambientStrength", ambient_strength);
		shader.setVec3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);
		shader.setFloat("material.shininess", shininess);

		if (lightDirectional.enable) {
			lightDirectional.UpdateDirectionalLight(vec3(light_dir_color.x, light_dir_color.y, light_dir_color.z), radians(light_dir_dir));
			shader.setVec3("lightDir.color", lightDirectional.Color.x, lightDirectional.Color.y, lightDirectional.Color.z);
			shader.setVec3("lightDir.dir", lightDirectional.Direction.x, lightDirectional.Direction.y, lightDirectional.Direction.z);
		}
		else {
			light_dir_color = black;
			lightDirectional.UpdateDirectionalLight(vec3(light_dir_color.x, light_dir_color.y, light_dir_color.z), radians(light_dir_dir));
			shader.setVec3("lightDir.color", lightDirectional.Color.x, lightDirectional.Color.y, lightDirectional.Color.z);
			shader.setVec3("lightDir.dir", lightDirectional.Direction.x, lightDirectional.Direction.y, lightDirectional.Direction.z);
		}
		if (lightPoint.enable) {
			lightPoint.UpdateLightPoint(light_point_pos, vec3(light_point_color.x, light_point_color.y, light_point_color.z));
			shader.setVec3("lightPoint[0].pos", lightPoint.Position.x, lightPoint.Position.y, lightPoint.Position.z);
			shader.setVec3("lightPoint[0].color", lightPoint.Color.x, lightPoint.Color.y, lightPoint.Color.z);
			shader.setFloat("lightPoint[0].constant", lightPoint.constant);
			shader.setFloat("lightPoint[0].linear", lightPoint.linear);
			shader.setFloat("lightPoint[0].quadratic", lightPoint.quadratic);
		}
		else {
			light_point_color = black;
			lightPoint.UpdateLightPoint(light_point_pos, vec3(light_point_color.x, light_point_color.y, light_point_color.z));
			shader.setVec3("lightPoint[0].pos", lightPoint.Position.x, lightPoint.Position.y, lightPoint.Position.z);
			shader.setVec3("lightPoint[0].color", lightPoint.Color.x, lightPoint.Color.y, lightPoint.Color.z);
			shader.setFloat("lightPoint[0].constant", lightPoint.constant);
			shader.setFloat("lightPoint[0].linear", lightPoint.linear);
			shader.setFloat("lightPoint[0].quadratic", lightPoint.quadratic);
		}
		if (lightSpot.enable) {
			lightSpot.UpdateLightSpot(light_spot_pos, radians(light_spot_dir), vec3(light_spot_color.x, light_spot_color.y, light_spot_color.z), light_spot_cosphy_in, light_spot_cosphy_out);
			shader.setVec3("lightSpot.pos", lightSpot.Position.x, lightSpot.Position.y, lightSpot.Position.z);
			shader.setVec3("lightSpot.dir", lightSpot.Direction.x, lightSpot.Direction.y, lightSpot.Direction.z);
			shader.setVec3("lightSpot.color", lightSpot.Color.x, lightSpot.Color.y, lightSpot.Color.z);
			shader.setFloat("lightSpot.cosPhyIn", lightSpot.cosInnerPhy);
			shader.setFloat("lightSpot.cosPhyOut", lightSpot.cosOutterPhy);
		}
		else {
			light_spot_color = black;
			lightSpot.UpdateLightSpot(light_spot_pos, radians(light_spot_dir), vec3(light_spot_color.x, light_spot_color.y, light_spot_color.z), light_spot_cosphy_in, light_spot_cosphy_out);
			shader.setVec3("lightSpot.pos", lightSpot.Position.x, lightSpot.Position.y, lightSpot.Position.z);
			shader.setVec3("lightSpot.dir", lightSpot.Direction.x, lightSpot.Direction.y, lightSpot.Direction.z);
			shader.setVec3("lightSpot.color", lightSpot.Color.x, lightSpot.Color.y, lightSpot.Color.z);
			shader.setFloat("lightSpot.cosPhyIn", lightSpot.cosInnerPhy);
			shader.setFloat("lightSpot.cosPhyOut", lightSpot.cosOutterPhy);
		}

		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//point light
		if (lightPoint.enable) {
			lightShader.use();
			mat4 ModelMat_light_point = translate(mat4(1.0f), lightPoint.Position);
			ModelMat_light_point = scale(ModelMat_light_point, vec3(0.1f));
			lightShader.setMat4("ModelMat", ModelMat_light_point);
			lightShader.setMat4("ProjectionMat", ProjectionMat);
			lightShader.setMat4("ViewMat", ViewMat);
			lightShader.setVec3("lightColor", lightPoint.Color.x, lightPoint.Color.y, lightPoint.Color.z);
			glBindVertexArray(lightPointVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}		

		//spot light
		if (lightSpot.enable) {
			lightShader.use();
			mat4 ModelMat_light_spot = translate(mat4(1.0f), lightSpot.Position);
			ModelMat_light_spot = scale(ModelMat_light_spot, vec3(0.1f));
			lightShader.setMat4("ModelMat", ModelMat_light_spot);
			lightShader.setMat4("ProjectionMat", ProjectionMat);
			lightShader.setMat4("ViewMat", ViewMat);
			lightShader.setVec3("lightColor", lightSpot.Color.x, lightSpot.Color.y, lightSpot.Color.z);
			glBindVertexArray(lightPointVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}	

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

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