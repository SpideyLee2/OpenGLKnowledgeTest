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
	// Positions			Normals
	// Front face
	-0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,

	// Back face
	-0.5f, -0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,

	// Left Face
	-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,

	// Right face
	 0.5f,  0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,

	 // Bottom face
	 -0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,
	  0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,
	  0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,
	  0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,
	 -0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,
	 -0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,

	 // Top face
	 -0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,
	  0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,
	  0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,
	  0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,
	 -0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,
	 -0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f
};


// Positions of cube objects in world space
//glm::vec3 cubePositions[] = {
//	glm::vec3(0.0f,  0.0f,  0.0f),
//	glm::vec3(2.0f,  5.0f, -15.0f),
//	glm::vec3(-1.5f, -2.2f, -2.5f),
//	glm::vec3(-3.8f, -2.0f, -12.3f),
//	glm::vec3(2.4f, -0.4f, -3.5f),
//	glm::vec3(-1.7f,  3.0f, -7.5f),
//	glm::vec3(1.3f, -2.0f, -2.5f),
//	glm::vec3(1.5f,  2.0f, -2.5f),
//	glm::vec3(1.5f,  0.2f, -1.5f),
//	glm::vec3(-1.3f,  1.0f, -1.5f)
//};

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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnable(GL_DEPTH_TEST);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glm::vec3 objColor{ 1.0f, 0.5f, 0.2f };
	glm::vec3 lightColor{ 1.0f, 1.0f, 1.0f };
	glm::vec3 lightPos{ 1.2f, 1.0f, 2.0f };

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
		glm::vec3 vLightPos = glm::vec3(view * glm::vec4(lightPos, 1.0));

		lightColor.x = sin(glfwGetTime() * 2.0f);
		lightColor.y = sin(glfwGetTime() * 0.7f);
		lightColor.z = sin(glfwGetTime() * 1.3f);

		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = lightColor * glm::vec3(0.2f);

		shaderProgram.use();

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.id, "model"), 1, GL_FALSE, glm::value_ptr(objModel));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.id, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.id, "proj"), 1, GL_FALSE, glm::value_ptr(proj));

		glUniform3fv(glGetUniformLocation(shaderProgram.id, "material.ambient"), 1, glm::value_ptr(glm::vec3(0.329412f, 0.223529f, 0.027451f)));
		glUniform3fv(glGetUniformLocation(shaderProgram.id, "material.diffuse"), 1, glm::value_ptr(glm::vec3(0.780392f, 0.568627f, 0.113725f)));
		glUniform3fv(glGetUniformLocation(shaderProgram.id, "material.specular"), 1, glm::value_ptr(glm::vec3(0.992157f, 0.941176f, 0.807843f)));
		glUniform1f(glGetUniformLocation(shaderProgram.id, "material.shininess"), 128.0f * 0.21794872f);

		glUniform3fv(glGetUniformLocation(shaderProgram.id, "light.position"), 1, glm::value_ptr(vLightPos));
		glUniform3fv(glGetUniformLocation(shaderProgram.id, "light.ambient"), 1, glm::value_ptr(ambientColor));
		glUniform3fv(glGetUniformLocation(shaderProgram.id, "light.diffuse"), 1, glm::value_ptr(diffuseColor));
		glUniform3fv(glGetUniformLocation(shaderProgram.id, "light.specular"), 1, glm::value_ptr(glm::vec3(1.0f)));

		//gouraudShaderProgram.use();

		//glUniformMatrix4fv(glGetUniformLocation(gouraudShaderProgram.id, "model"), 1, GL_FALSE, glm::value_ptr(objModel));
		//glUniformMatrix4fv(glGetUniformLocation(gouraudShaderProgram.id, "view"), 1, GL_FALSE, glm::value_ptr(view));
		//glUniformMatrix4fv(glGetUniformLocation(gouraudShaderProgram.id, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
		//glUniform3fv(glGetUniformLocation(gouraudShaderProgram.id, "objColor"), 1, glm::value_ptr(objColor));
		//glUniform3fv(glGetUniformLocation(gouraudShaderProgram.id, "lightColor"), 1, glm::value_ptr(lightColor));
		//glUniform3fv(glGetUniformLocation(gouraudShaderProgram.id, "lightPos"), 1, glm::value_ptr(vLightPos));

		glBindVertexArray(objVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		lightShaderProgram.use();

		glm::mat4 lightModel{ 1.0f };
		lightModel = glm::translate(lightModel, lightPos);
		lightModel = glm::scale(lightModel, glm::vec3(0.2f));		

		glUniformMatrix4fv(glGetUniformLocation(lightShaderProgram.id, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
		glUniformMatrix4fv(glGetUniformLocation(lightShaderProgram.id, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(lightShaderProgram.id, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
		glUniform3fv(glGetUniformLocation(lightShaderProgram.id, "lightColor"), 1, glm::value_ptr(lightColor));

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

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