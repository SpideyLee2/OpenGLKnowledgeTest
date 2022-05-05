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
unsigned int loadCubemap(std::vector<const char*> dirs);
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
bool errorPrinted = false;

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

// Quad vertices in screen space. Covers the whole screen
float screenQuadVertices[] = {
	// Positions	// Tex Coords
	-1.0f, -1.0f,	0.0f, 0.0f,		// bottom left
	-1.0f,  1.0f,	0.0f, 1.0f,		// top left
	 1.0f,  1.0f,	1.0f, 1.0f,		// top right

	-1.0f, -1.0f,	0.0f, 0.0f,		// bottom left
	 1.0f, -1.0f,	1.0f, 0.0f,		// bottom right
	 1.0f,  1.0f,	1.0f, 1.0f		// top right
};

float mirrorQuadVertices[] = {
	// Positions		// Tex Coords
	-0.25f,  0.5f,		0.0f, 0.0f,		// bottom left
	-0.25f,  1.0f,		0.0f, 1.0f,		// top left
	 0.25f,  1.0f,		1.0f, 1.0f,		// top right

	-0.25f,  0.5f,		0.0f, 0.0f,		// bottom left
	 0.25f,  0.5f,		1.0f, 0.0f,		// bottom right
	 0.25f,  1.0f,		1.0f, 1.0f		// top right
};

float skyboxVertices[] = {
	// Positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

float reflectiveCubeVertices[] = {
	// Positions			// Normals
	-0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f
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
	Shader screenShader{ "Shaders/screen.vert", "Shaders/screen.frag" };
	Shader skyboxShader{ "Shaders/skybox.vert", "Shaders/skybox.frag" };
	Shader reflectShader{ "Shaders/reflect.vert", "Shaders/reflect.frag" };
	Shader refractShader{ "Shaders/refract.vert", "Shaders/refract.frag" };

	Texture2D containerTexture{ "Textures/container.jpg", 0, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };

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
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	#pragma endregion

	#pragma region Screen_Quad_VAO_Setup
	unsigned int screenQuadVAO, screenQuadVBO;
	glGenVertexArrays(1, &screenQuadVAO);
	glGenBuffers(1, &screenQuadVBO);

	glBindVertexArray(screenQuadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(screenQuadVertices), screenQuadVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(sizeof(float) * 2));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	#pragma endregion

	#pragma region Reflective_Cube_VAO_Setup
	unsigned int reflectiveCubeVAO, reflectiveCubeVBO;
	glGenVertexArrays(1, &reflectiveCubeVAO);
	glGenBuffers(1, &reflectiveCubeVBO);

	glBindVertexArray(reflectiveCubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, reflectiveCubeVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(reflectiveCubeVertices), reflectiveCubeVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float) * 3));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	#pragma endregion

	#pragma region Skybox_VAO_Setup
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);

	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	#pragma endregion

	std::vector<const char*> skyboxTextureDirs = {
		"Textures/skybox/right.jpg",	// Right face
		"Textures/skybox/left.jpg",		// Left face
		"Textures/skybox/top.jpg",		// Top face
		"Textures/skybox/bottom.jpg",	// Bottom face
		"Textures/skybox/front.jpg",	// Front face
		"Textures/skybox/back.jpg"		// Back face
	};
	unsigned int skybox = loadCubemap(skyboxTextureDirs);

	#pragma region Framebuffer_Setup
	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	unsigned int textureColorBuffer;
	glGenTextures(1, &textureColorBuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR::FRAMEBUFFER::Framebuffer is not complete!" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	#pragma endregion

	glEnable(GL_DEPTH_TEST);

	// Render Loop
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// INPUT
		// Processes user input
		processInput(window);

		// RENDERING
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

		// Reflective Cube
		/*
		reflectShader.use();
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox);

		glm::mat4 objModel{ 1.0f };

		glUniform1i(glGetUniformLocation(reflectShader.id, "skybox"), 0);
		glUniform3fv(glGetUniformLocation(reflectShader.id, "cameraPos"), 1, glm::value_ptr(camera.cameraPosition));
		glUniformMatrix4fv(glGetUniformLocation(reflectShader.id, "model"), 1, GL_FALSE, glm::value_ptr(objModel));
		glUniformMatrix4fv(glGetUniformLocation(reflectShader.id, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(reflectShader.id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(reflectiveCubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		*/

		// Refractive Cube
		refractShader.use();
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox);

		glm::mat4 objModel{ 1.0f };

		glUniform1i(glGetUniformLocation(refractShader.id, "skybox"), 0);
		glUniform3fv(glGetUniformLocation(refractShader.id, "cameraPos"), 1, glm::value_ptr(camera.cameraPosition));
		glUniform1f(glGetUniformLocation(refractShader.id, "initMat"), 1.00f);		// Air refractive index
		glUniform1f(glGetUniformLocation(refractShader.id, "refractMat"), 1.52f);	// Glass refractive index
		glUniformMatrix4fv(glGetUniformLocation(refractShader.id, "model"), 1, GL_FALSE, glm::value_ptr(objModel));
		glUniformMatrix4fv(glGetUniformLocation(refractShader.id, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(refractShader.id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(reflectiveCubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Skybox
		skyboxShader.use();
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox);

		glm::mat4 skyboxView = glm::mat4(glm::mat3(view));
		glUniform1i(glGetUniformLocation(skyboxShader.id, "skybox"), 0);
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.id, "view"), 1, GL_FALSE, glm::value_ptr(skyboxView));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glBindVertexArray(skyboxVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glDepthFunc(GL_LESS);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);

		// Off-screen rendered screen quad
		screenShader.use();
		glBindVertexArray(screenQuadVAO);
		glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();

		//printErrors();
	}

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
		errorPrinted = true;
		errorCode = glGetError();
	}
}

unsigned int loadCubemap(std::vector<const char*> dirs) {
	int dirsSize = dirs.size();
	if (dirsSize < 6) {
		std::cout << "ERROR: A vector of at least six directories must be provided to load a cubemap. Number provided: " 
				  << dirsSize << "\nReturning 0..." << std::endl;
		return 0;
	}
	
	unsigned int cubemap;
	glGenTextures(1, &cubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);

	int width, height, numChannels;
	stbi_set_flip_vertically_on_load(false);
	for (int i = 0; i < 6; ++i) {
		unsigned char* data = stbi_load(dirs[i], &width, &height, &numChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else {
			std::cout << "ERROR: Cannot load image at: " << dirs[i] << std::endl;
		}
		stbi_image_free(data);
	}
	stbi_set_flip_vertically_on_load(true);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return cubemap;
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