#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>

#include "Shader.h"
#include "Camera.h"
#include "Texture2D.h"
#include "Model.h"

#pragma region Function_Declarations
void printErrors();
void drawModel(Model& obj, Shader& shader, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);
void processInput(GLFWwindow* window);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void cursorPosCallback(GLFWwindow* window, double xPos, double yPos);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
#pragma endregion

const int WIDTH = 800;
const int HEIGHT = 600;

double lastFrame = 0.0f;
float deltaTime = 0.0f;

bool firstMouseInput = true;
float lastXPos = WIDTH / 2;
float lastYPos = HEIGHT / 2;

bool flashlightIsActive = true;

Camera camera{ glm::vec3(0.0f, 0.0f, 3.0f) };

// Vertex data of a cube
float vertices[] = {
	// Positions			// Normals				// Texture Coords
	-0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,	0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,	1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,	1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,	1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,	0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,	0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,	1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,	0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,	1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,	1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,	1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,	0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,	0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,	0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,	1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,	1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,	1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,	0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,	0.0f, 1.0f
};

float quadVertices[] = {
	// Positions			// Tex Coords
	-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		// bottom left
	-0.5f,  0.5f, 0.0f,		0.0f, 1.0f,		// top left
	 0.5f,  0.5f, 0.0f,		1.0f, 1.0f,		// top right

	-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		// bottom left
	 0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		// bottom right
	 0.5f,  0.5f, 0.0f,		1.0f, 1.0f		// top right
};

// Positions of cube objects in world space
glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -5.0f)
};

glm::vec3 pointLightColors[] = {
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f)
};

std::vector<glm::vec3> grassPositions{
	glm::vec3{-1.5f,  0.0f, -0.48f},
	glm::vec3{ 1.5f,  0.0f,  0.51f},
	glm::vec3{ 0.0f,  0.0f,  0.7f},
	glm::vec3{-0.3f,  0.0f, -2.3f},
	glm::vec3{ 0.5f,  0.0f, -0.6f}
};

std::vector<glm::vec3> windowPositions {
	glm::vec3{-1.5f,  0.0f, -0.48f},
	glm::vec3{ 1.5f,  0.0f,  0.51f},
	glm::vec3{ 0.0f,  0.0f,  0.7f},
	glm::vec3{-0.3f,  0.0f, -2.3f},
	glm::vec3{ 0.5f,  0.0f, -0.6f}
};

int main() {
	#pragma region GLFW_Setup
	// Initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Create window object
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Test", NULL, NULL);

	if (!window) {
		std::cout << "Failed to initialize GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetScrollCallback(window, scrollCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glViewport(0, 0, WIDTH, HEIGHT);
	#pragma endregion

	stbi_set_flip_vertically_on_load(true);

	Shader cubeShader{ "Shaders/basic.vert", "Shaders/basic.frag" };
	Shader grassShader{ "Shaders/grass.vert", "Shaders/grass.frag" };
	Shader windowShader{ "Shaders/window.vert", "Shaders/window.frag" };

	//Texture2D grassTexture{ "Textures/grass.png", 0, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
	Texture2D windowTexture{ "Textures/window.png", 0, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };

	#pragma region Cube_VAO_Setup
	// Configure VAO
	unsigned int objVAO;
	glGenVertexArrays(1, &objVAO);
	glBindVertexArray(objVAO);

	//// Configure EBO
	//unsigned int ebo;
	//glGenBuffers(1, &ebo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Configure VBO
	unsigned int objVBO;
	glGenBuffers(1, &objVBO);
	glBindBuffer(GL_ARRAY_BUFFER, objVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	#pragma endregion

	#pragma region Quad_VAO_Setup
	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);

	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	#pragma endregion


	// Light cube object VAO setup
	/*
	unsigned int vbo;
	glGenBuffers(1, &vbo);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	*/

	glEnable(GL_DEPTH_TEST);

	// Render loop
	/*
	while (!glfwWindowShouldClose(window)) {
		double currFrame = glfwGetTime();
		deltaTime = (float)(currFrame - lastFrame);
		lastFrame = currFrame;

		processInput(window);

		glClearColor(0.3f, 0.05f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 objModel{ 1.0f };

		glm::mat4 view = camera.getViewMatrix();

		glm::mat4 proj = glm::perspective(glm::radians(camera.fov), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		
		//glm::vec3 lightPos{ cos(currFrame) * 2, sin(currFrame) * 2, cos(currFrame)};
		//glm::vec3 vLightPos = glm::vec3(view * glm::vec4(lightPos, 1.0));
		//glm::vec3 vLightDir = glm::vec3(view * glm::vec4(-lightPos, 0.0));
		//lightColor.x = sin(glfwGetTime() * 2.0f);
		//lightColor.y = sin(glfwGetTime() * 0.7f);
		//lightColor.z = sin(glfwGetTime() * 1.3f);

		//glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		//glm::vec3 ambientColor = lightColor * glm::vec3(0.05f);

		objectShader.use();

		glUniformMatrix4fv(glGetUniformLocation(objectShader.id, "model"), 1, GL_FALSE, glm::value_ptr(objModel));
		glUniformMatrix4fv(glGetUniformLocation(objectShader.id, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(objectShader.id, "proj"), 1, GL_FALSE, glm::value_ptr(proj));

		glUniform1i(glGetUniformLocation(objectShader.id, "material.diffuse"), 0);
		glUniform1i(glGetUniformLocation(objectShader.id, "material.specular"), 1);
		//glUniform1i(glGetUniformLocation(objectShader.id, "material.emission"), 2);
		glUniform1f(glGetUniformLocation(objectShader.id, "material.shininess"), 32.0f);

		glm::vec3 dirLightDir = -glm::vec3(view * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
		glUniform3fv(glGetUniformLocation(objectShader.id, "dirLight.direction"), 1, glm::value_ptr(dirLightDir));
		glUniform3fv(glGetUniformLocation(objectShader.id, "dirLight.ambient"), 1, glm::value_ptr(glm::vec3(0.05f)));
		glUniform3fv(glGetUniformLocation(objectShader.id, "dirLight.diffuse"), 1, glm::value_ptr(glm::vec3(0.4f)));
		glUniform3fv(glGetUniformLocation(objectShader.id, "dirLight.specular"), 1, glm::value_ptr(glm::vec3(0.5f)));

		for (int i = 0; i < 4; ++i) {
			std::string index = std::to_string(i);

			glm::vec3 pointLightPos = glm::vec3(view * glm::vec4(pointLightPositions[i], 1.0f));
			glUniform3fv(glGetUniformLocation(objectShader.id, ("pointLights[" + index + "].position").c_str()), 1, 
						 glm::value_ptr(pointLightPos));
			glUniform1f(glGetUniformLocation(objectShader.id, ("pointLights[" + index + "].constant").c_str()), 1.0f);
			glUniform1f(glGetUniformLocation(objectShader.id, ("pointLights[" + index + "].linear").c_str()), 0.09f);
			glUniform1f(glGetUniformLocation(objectShader.id, ("pointLights[" + index + "].quadratic").c_str()), 0.032f);
			glUniform3fv(glGetUniformLocation(objectShader.id, ("pointLights[" + index + "].ambient").c_str()), 1, 
						 glm::value_ptr(glm::vec3(0.02f) * pointLightColors[i]));
			glUniform3fv(glGetUniformLocation(objectShader.id, ("pointLights[" + index + "].diffuse").c_str()), 1, 
						 glm::value_ptr(glm::vec3(0.8f) * pointLightColors[i]));
			glUniform3fv(glGetUniformLocation(objectShader.id, ("pointLights[" + index + "].specular").c_str()), 1, 
						 glm::value_ptr(glm::vec3(1.0f) * pointLightColors[i]));
		}

		glUniform1f(glGetUniformLocation(objectShader.id, "spotLight.innerCutoff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(objectShader.id, "spotLight.outerCutoff"), glm::cos(glm::radians(17.5f)));
		glUniform1f(glGetUniformLocation(objectShader.id, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(objectShader.id, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(objectShader.id, "spotLight.quadratic"), 0.032f);
		glUniform3fv(glGetUniformLocation(objectShader.id, "spotLight.ambient"), 1, glm::value_ptr(glm::vec3(0.03f)));
		glUniform3fv(glGetUniformLocation(objectShader.id, "spotLight.diffuse"), 1, glm::value_ptr(glm::vec3(1.0f)));
		glUniform3fv(glGetUniformLocation(objectShader.id, "spotLight.specular"), 1, glm::value_ptr(glm::vec3(1.0f)));

		containerDiffuseMap.bind();
		containerSpecularMap.bind();
		containerEmissionMap.bind();

		glBindVertexArray(objVAO);
		for (int i = 0; i < 10; ++i) {
			objModel = glm::mat4(1.0f);
			objModel = glm::translate(objModel, cubePositions[i]);
			float angle = 20.0f * i;
			objModel = glm::rotate(objModel, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(glGetUniformLocation(objectShader.id, "model"), 1, GL_FALSE, glm::value_ptr(objModel));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		lightShader.use();

		glUniformMatrix4fv(glGetUniformLocation(lightShader.id, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(lightShader.id, "proj"), 1, GL_FALSE, glm::value_ptr(proj));

		glBindVertexArray(lightVAO);
		for (int i = 0; i < 4; ++i) {
			glm::mat4 lightModel{ 1.0f };
			lightModel = glm::translate(lightModel, pointLightPositions[i]);
			lightModel = glm::scale(lightModel, glm::vec3(0.2f));
			glUniformMatrix4fv(glGetUniformLocation(lightShader.id, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
			glUniform3fv(glGetUniformLocation(lightShader.id, "lightColor"), 1, glm::value_ptr(pointLightColors[i]));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	*/
	
	// Model Render Loop
	/*
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// INPUT
		// Processes user input
		processInput(window);

		// RENDERING
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);

		// Changes the background color after clearing it and clears the depth buffer bit
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		objectShader.use();

		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		glm::mat4 objModel{ 1.0f };
		objModel = translate(objModel, glm::vec3(0.0f, 0.0f, 0.0f));
		objModel = scale(objModel, glm::vec3(1.0f, 1.0f, 1.0f));

		drawModel(backpackModel, objectShader, objModel, view, projection);

		glDisable(GL_DEPTH_TEST);
		glStencilMask(0x00);

		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);

		outlineShader.use();

		//objModel = glm::scale(objModel, glm::vec3(1.1f, 1.1f, 1.1f));

		glUniformMatrix4fv(glGetUniformLocation(outlineShader.id, "model"), 1, GL_FALSE, glm::value_ptr(objModel));
		glUniformMatrix4fv(glGetUniformLocation(outlineShader.id, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(outlineShader.id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniform3fv(glGetUniformLocation(outlineShader.id, "outlineColor"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 0.0f)));

		backpackModel2.draw(outlineShader);

		glEnable(GL_DEPTH_TEST);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		glStencilMask(0xFF);

		//lightShader.use();

		//glUniformMatrix4fv(glGetUniformLocation(lightShader.id, "view"), 1, GL_FALSE, glm::value_ptr(view));
		//glUniformMatrix4fv(glGetUniformLocation(lightShader.id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		//glBindVertexArray(lightVAO);
		//for (int i = 0; i < 4; ++i) {
		//	glm::mat4 lightModel{ 1.0f };
		//	lightModel = glm::translate(lightModel, pointLightPositions[i]);
		//	lightModel = glm::scale(lightModel, glm::vec3(0.2f));
		//	glUniformMatrix4fv(glGetUniformLocation(lightShader.id, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
		//	glUniform3fv(glGetUniformLocation(lightShader.id, "lightColor"), 1, glm::value_ptr(pointLightColors[i]));

		//	glDrawArrays(GL_TRIANGLES, 0, 36);
		//}

		glfwSwapBuffers(window);
		glfwPollEvents();

		//printErrors();
	}
	*/	
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Transparent objects render loop
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// INPUT
		// Processes user input
		processInput(window);

		// RENDERING
		// Changes the background color after clearing it and clears the depth buffer bit
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		
		cubeShader.use();

		glm::mat4 objModel{ 1.0f };

		glUniformMatrix4fv(glGetUniformLocation(cubeShader.id, "model"), 1, GL_FALSE, glm::value_ptr(objModel));
		glUniformMatrix4fv(glGetUniformLocation(cubeShader.id, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(cubeShader.id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(objVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		// Grass Drawing
		/*
		grassShader.use();
		grassTexture.bind();

		glUniformMatrix4fv(glGetUniformLocation(grassShader.id, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(grassShader.id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniform1i(glGetUniformLocation(grassShader.id, "tex"), 0);

		glBindVertexArray(quadVAO);
		for (int i = 0; i < grassPositions.size(); ++i) {
			glm::mat4 grassModel{ 1.0f };
			grassModel = glm::translate(grassModel, grassPositions[i]);

			glUniformMatrix4fv(glGetUniformLocation(grassShader.id, "model"), 1, GL_FALSE, glm::value_ptr(grassModel));

			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		*/
		
		// Window Drawing
		windowShader.use();
		windowTexture.bind();

		glUniformMatrix4fv(glGetUniformLocation(windowShader.id, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(windowShader.id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniform1i(glGetUniformLocation(windowShader.id, "tex"), 0);

		std::map<float, glm::vec3> sortedWindows;
		for (int i = 0; i < windowPositions.size(); ++i) {
			float distance = length(camera.cameraPosition - windowPositions[i]);
			sortedWindows[distance] = windowPositions[i];
		}

		glBindVertexArray(quadVAO);
		for (std::map<float, glm::vec3>::reverse_iterator revItr = sortedWindows.rbegin(); revItr != sortedWindows.rend(); ++revItr) {
			glm::mat4 windowModel{ 1.0f };
			windowModel = glm::translate(windowModel, revItr->second);

			glUniformMatrix4fv(glGetUniformLocation(windowShader.id, "model"), 1, GL_FALSE, glm::value_ptr(windowModel));

			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();

		printErrors();
	}

	//glDeleteVertexArrays(1, &objVAO);
	//glDeleteBuffers(1, &vbo);
	glfwTerminate();

	return 0;
}

// Used to print every error that has occurred in a frame
void printErrors() {
	int errorCode = glGetError();
	while (errorCode) {
		std::string error;
		switch (errorCode) {
			case GL_INVALID_ENUM:
				error = "Invalid enum.";
				break;
			case GL_INVALID_VALUE:
				error = "Value out of range.";
				break;
			case GL_INVALID_OPERATION:
				error = "Operation not allowed in current state.";
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				error = "The framebuffer object is not complete.";
				break;
			case GL_OUT_OF_MEMORY:
				error = "There is not enough memory left to execute the command.";
				break;
			default:
				error = "How am I even here to begin with?";
		}
		std::cout << "Error " << errorCode << ": " << error << std::endl;
		errorCode = glGetError();
	}
}

void drawModel(Model& obj, Shader& shader, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) {
	glUniformMatrix4fv(glGetUniformLocation(shader.id, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader.id, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader.id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glm::vec3 dirLightDir = -glm::vec3(view * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	glUniform3fv(glGetUniformLocation(shader.id, "dirLight.direction"), 1, glm::value_ptr(dirLightDir));
	glUniform3fv(glGetUniformLocation(shader.id, "dirLight.ambient"), 1, glm::value_ptr(glm::vec3(0.1f)));
	glUniform3fv(glGetUniformLocation(shader.id, "dirLight.diffuse"), 1, glm::value_ptr(glm::vec3(0.4f)));
	glUniform3fv(glGetUniformLocation(shader.id, "dirLight.specular"), 1, glm::value_ptr(glm::vec3(0.5f)));

	for (int i = 0; i < 4; ++i) {
		std::string index = std::to_string(i);

		glm::vec3 pointLightPos = glm::vec3(view * glm::vec4(pointLightPositions[i], 1.0f));
		glUniform3fv(glGetUniformLocation(shader.id, ("pointLights[" + index + "].position").c_str()), 1,
					 glm::value_ptr(pointLightPos));
		glUniform1f(glGetUniformLocation(shader.id, ("pointLights[" + index + "].constant").c_str()), 1.0f);
		glUniform1f(glGetUniformLocation(shader.id, ("pointLights[" + index + "].linear").c_str()), 0.09f);
		glUniform1f(glGetUniformLocation(shader.id, ("pointLights[" + index + "].quadratic").c_str()), 0.032f);
		glUniform3fv(glGetUniformLocation(shader.id, ("pointLights[" + index + "].ambient").c_str()), 1,
					 glm::value_ptr(glm::vec3(0.02f) * pointLightColors[i]));
		glUniform3fv(glGetUniformLocation(shader.id, ("pointLights[" + index + "].diffuse").c_str()), 1,
					 glm::value_ptr(glm::vec3(0.8f) * pointLightColors[i]));
		glUniform3fv(glGetUniformLocation(shader.id, ("pointLights[" + index + "].specular").c_str()), 1,
					 glm::value_ptr(glm::vec3(1.0f) * pointLightColors[i]));
	}

	glUniform1i(glGetUniformLocation(shader.id, "spotLight.isActive"), flashlightIsActive);
	glUniform1f(glGetUniformLocation(shader.id, "spotLight.innerCutoff"), glm::cos(glm::radians(12.5f)));
	glUniform1f(glGetUniformLocation(shader.id, "spotLight.outerCutoff"), glm::cos(glm::radians(17.5f)));
	glUniform1f(glGetUniformLocation(shader.id, "spotLight.constant"), 1.0f);
	glUniform1f(glGetUniformLocation(shader.id, "spotLight.linear"), 0.09f);
	glUniform1f(glGetUniformLocation(shader.id, "spotLight.quadratic"), 0.032f);
	glUniform3fv(glGetUniformLocation(shader.id, "spotLight.ambient"), 1, glm::value_ptr(glm::vec3(0.03f)));
	glUniform3fv(glGetUniformLocation(shader.id, "spotLight.diffuse"), 1, glm::value_ptr(glm::vec3(1.0f)));
	glUniform3fv(glGetUniformLocation(shader.id, "spotLight.specular"), 1, glm::value_ptr(glm::vec3(1.0f)));

	obj.draw(shader);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_BACKSLASH) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else if (glfwGetKey(window, GLFW_KEY_BACKSLASH) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		flashlightIsActive = false;
	}
	else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		flashlightIsActive = true;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		camera.applySpeedMultiplier();
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
		camera.removeSpeedMultiplier();
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.input(CameraMovement::FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.input(CameraMovement::BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.input(CameraMovement::LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.input(CameraMovement::RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		camera.input(CameraMovement::UP, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		camera.input(CameraMovement::DOWN, deltaTime);
	}
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void cursorPosCallback(GLFWwindow* window, double xPosIn, double yPosIn) {
	float xPos = static_cast<float>(xPosIn);
	float yPos = static_cast<float>(yPosIn);

	if (firstMouseInput) {
		lastXPos = xPos;
		lastYPos = yPos;
		firstMouseInput = false;
	}

	float xOffset = xPos - lastXPos;
	float yOffset = yPos - lastYPos;
	lastXPos = xPos;
	lastYPos = yPos;

	camera.processMouseMove(xOffset, yOffset);
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
	camera.processMouseScroll(static_cast<float>(yOffset));
}