/*
* THINGS I FORGOT
*	- You need to use the shader program before the render loop using glUseProgram
*	- You can read files to a string easily using ifstream and sstream
*	
*	- You need to pass the texture data to glTexImage2D (you dummy)
*/

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Texture2D.h"

#pragma region FunctionDeclarations
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

//GLfloat vertices[] = {
//	// Positions (xyz)		// Colors (rgba)			// Texture Coords
//	-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f,		// bottom left
//	 0.0f,  0.5f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f,		// top
//	 0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f, 1.0f,		// bottom right
//};
//
//GLuint indices[] = {
//	0, 1, 2
//};

//GLfloat vertices[] = {
//	// Positions (xyz)		// Colors (rgba)			// Texture Coords
//	-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f,		0.0f, 0.0f,		// bottom left
//	-0.5f,  0.5f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f,		0.0f, 1.0f,		// top left
//	 0.5f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f, 1.0f,		1.0f, 1.0f,		// top right
//	 0.5f, -0.5f, 0.0f,		1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 0.0f		// bottom right
//};
//
//GLuint indices[] = {
//	0, 1, 3,
//	1, 2, 3
//};

glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

glm::vec3 pointLightColors[] = {
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f)
};

int main() {
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

	Shader shaderProgram{ "Shaders/default.vert", "Shaders/default.frag" };
	Shader lightShaderProgram{ "Shaders/light.vert", "Shaders/light.frag" };
	Shader gouraudShaderProgram{ "Shaders/gouraud.vert", "Shaders/gouraud.frag" };

	Texture2D containerDiffuseMap{ "Textures/container2.png", 0 };
	Texture2D containerSpecularMap{ "Textures/container2_specular.png", 1 };
	Texture2D containerEmissionMap{ "Textures/matrix.jpg", 2 };
	
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
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnable(GL_DEPTH_TEST);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//glm::vec3 objColor{ 1.0f, 0.5f, 0.2f };
	//glm::vec3 lightColor{ 1.0f, 1.0f, 1.0f };
	//glm::vec3 lightPos{ 1.2f, 1.0f, 2.0f };

	// Render loop
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

		shaderProgram.use();

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.id, "model"), 1, GL_FALSE, glm::value_ptr(objModel));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.id, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.id, "proj"), 1, GL_FALSE, glm::value_ptr(proj));

		glUniform1i(glGetUniformLocation(shaderProgram.id, "material.diffuse"), 0);
		glUniform1i(glGetUniformLocation(shaderProgram.id, "material.specular"), 1);
		//glUniform1i(glGetUniformLocation(shaderProgram.id, "material.emission"), 2);
		glUniform1f(glGetUniformLocation(shaderProgram.id, "material.shininess"), 32.0f);

		glm::vec3 dirLightDir = -glm::vec3(view * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
		glUniform3fv(glGetUniformLocation(shaderProgram.id, "dirLight.direction"), 1, glm::value_ptr(dirLightDir));
		glUniform3fv(glGetUniformLocation(shaderProgram.id, "dirLight.ambient"), 1, glm::value_ptr(glm::vec3(0.05f)));
		glUniform3fv(glGetUniformLocation(shaderProgram.id, "dirLight.diffuse"), 1, glm::value_ptr(glm::vec3(0.4f)));
		glUniform3fv(glGetUniformLocation(shaderProgram.id, "dirLight.specular"), 1, glm::value_ptr(glm::vec3(0.5f)));

		for (int i = 0; i < 4; ++i) {
			std::string index = std::to_string(i);

			glm::vec3 pointLightPos = glm::vec3(view * glm::vec4(pointLightPositions[i], 1.0f));
			glUniform3fv(glGetUniformLocation(shaderProgram.id, ("pointLights[" + index + "].position").c_str()), 1, 
						 glm::value_ptr(pointLightPos));
			glUniform1f(glGetUniformLocation(shaderProgram.id, ("pointLights[" + index + "].constant").c_str()), 1.0f);
			glUniform1f(glGetUniformLocation(shaderProgram.id, ("pointLights[" + index + "].linear").c_str()), 0.09f);
			glUniform1f(glGetUniformLocation(shaderProgram.id, ("pointLights[" + index + "].quadratic").c_str()), 0.032f);
			glUniform3fv(glGetUniformLocation(shaderProgram.id, ("pointLights[" + index + "].ambient").c_str()), 1, 
						 glm::value_ptr(glm::vec3(0.02f) * pointLightColors[i]));
			glUniform3fv(glGetUniformLocation(shaderProgram.id, ("pointLights[" + index + "].diffuse").c_str()), 1, 
						 glm::value_ptr(glm::vec3(0.8f) * pointLightColors[i]));
			glUniform3fv(glGetUniformLocation(shaderProgram.id, ("pointLights[" + index + "].specular").c_str()), 1, 
						 glm::value_ptr(glm::vec3(1.0f) * pointLightColors[i]));
		}

		glUniform1f(glGetUniformLocation(shaderProgram.id, "spotLight.innerCutoff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(shaderProgram.id, "spotLight.outerCutoff"), glm::cos(glm::radians(17.5f)));
		glUniform1f(glGetUniformLocation(shaderProgram.id, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(shaderProgram.id, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(shaderProgram.id, "spotLight.quadratic"), 0.032f);
		glUniform3fv(glGetUniformLocation(shaderProgram.id, "spotLight.ambient"), 1, glm::value_ptr(glm::vec3(0.03f)));
		glUniform3fv(glGetUniformLocation(shaderProgram.id, "spotLight.diffuse"), 1, glm::value_ptr(glm::vec3(1.0f)));
		glUniform3fv(glGetUniformLocation(shaderProgram.id, "spotLight.specular"), 1, glm::value_ptr(glm::vec3(1.0f)));

		containerDiffuseMap.bind();
		containerSpecularMap.bind();
		containerEmissionMap.bind();

		glBindVertexArray(objVAO);
		for (int i = 0; i < 10; ++i) {
			objModel = glm::mat4(1.0f);
			objModel = glm::translate(objModel, cubePositions[i]);
			float angle = 20.0f * i;
			objModel = glm::rotate(objModel, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram.id, "model"), 1, GL_FALSE, glm::value_ptr(objModel));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		lightShaderProgram.use();

		glUniformMatrix4fv(glGetUniformLocation(lightShaderProgram.id, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(lightShaderProgram.id, "proj"), 1, GL_FALSE, glm::value_ptr(proj));

		glBindVertexArray(lightVAO);
		for (int i = 0; i < 4; ++i) {
			glm::mat4 lightModel{ 1.0f };
			lightModel = glm::translate(lightModel, pointLightPositions[i]);
			lightModel = glm::scale(lightModel, glm::vec3(0.2f));
			glUniformMatrix4fv(glGetUniformLocation(lightShaderProgram.id, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
			glUniform3fv(glGetUniformLocation(lightShaderProgram.id, "lightColor"), 1, glm::value_ptr(pointLightColors[i]));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &objVAO);
	glDeleteBuffers(1, &vbo);
	glfwTerminate();

	return 0;
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		camera.applySpeedMultiplier();
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
		camera.removeSpeedMultiplier();
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.input(CAMERA_MOVEMENT::FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.input(CAMERA_MOVEMENT::BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.input(CAMERA_MOVEMENT::LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.input(CAMERA_MOVEMENT::RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		camera.input(CAMERA_MOVEMENT::UP, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		camera.input(CAMERA_MOVEMENT::DOWN, deltaTime);
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