#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Camera {
public:
	glm::vec3 cameraPosition;
	float yaw, pitch;
	float mouseSensitivity, scrollSensitivity;
	float speed, speedMultiplier;

	float fov;
	float pitchConstraintHi, pitchConstraintLo;
	float fovConstraintHi, fovConstraintLo;

	Camera(glm::vec3 pos, glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));

	void input(CameraMovement movementDirection, float deltaTime);
	void applySpeedMultiplier();
	void removeSpeedMultiplier();
	void processMouseMove(float xOffset, float yOffset, bool constrainPitch = true);
	void processMouseScroll(float yOffset);
	glm::mat4 getViewMatrix();

private:
	glm::vec3 cameraFront;
	glm::vec3 worldUp;
	float tempSpeed;
	glm::mat4 myLookAt(glm::vec3 pos, glm::vec3 target, glm::vec3 up);
};
