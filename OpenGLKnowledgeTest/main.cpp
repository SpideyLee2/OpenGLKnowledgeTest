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

// Vertices of a cube
float vertices[] = {
//		Positions				Colors						Texture Coords
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,	1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,	1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,	1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,	1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,	1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,	1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,	1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,	1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,	1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,	1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,	1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,	1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,	1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,	1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,	1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,	1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,	1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f
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

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraRight = glm::normalize(glm::cross(cameraUp, cameraDirection));

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
	 
	// Configure VAO
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, (void*)(7 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	Shader shaderProgram{ "Shaders/default.vert", "Shaders/default.frag" };

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_set_flip_vertically_on_load(true);

	unsigned int texture1, texture2;
	int width, height, numChannels;
	glGenTextures(1, &texture1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	unsigned char* data = stbi_load("Textures/container.jpg", &width, &height, &numChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load container.jpg" << std::endl;
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &texture2);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	data = stbi_load("Textures/smiley.png", &width, &height, &numChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load smiley.png" << std::endl;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);

	shaderProgram.use();
	shaderProgram.setUniformInt("texture1", 0);
	shaderProgram.setUniformInt("texture2", 1);
	shaderProgram.setUniformFloat("mixRatio", 0.2f);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnable(GL_DEPTH_TEST);

	// Render loop
	while (!glfwWindowShouldClose(window)) {
		double currFrame = glfwGetTime();
		deltaTime = (float)(currFrame - lastFrame);
		lastFrame = currFrame;

		processInput(window);

		glClearColor(0.3f, 0.05f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glBindVertexArray(vao);

		glm::mat4 view = camera.getViewMatrix();

		glm::mat4 proj = glm::perspective(glm::radians(camera.fov), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.id, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.id, "proj"), 1, GL_FALSE, glm::value_ptr(proj));

		for (int i = 0; i < 10; ++i) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			if (i % 3 == 0) {
				model = glm::rotate(model, (float)glfwGetTime()*(i+1)/2.0f, glm::vec3(1.0f, 0.5f, 0.5f));
			}
			else {
				model = glm::rotate(model, i * 20.0f, glm::vec3(0.7f, 0.5f, 0.5f));
			}

			glUniformMatrix4fv(glGetUniformLocation(shaderProgram.id, "model"), 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices));
		}
		
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &vao);
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