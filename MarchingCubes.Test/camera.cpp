#include <pch.h>
#include <camera.h>

Camera::Camera(GLFWwindow* w, float speed) : speed(speed), window(w)
{
	g_position = glm::vec3(0, 0, 500.0f);
	g_initial_fov = glm::pi<float>() * 0.15f;
	g_direction = glm::vec3(0.0f, 0.0f, -1.0f);
	up = glm::vec3(0, 1, 0);
}

void Camera::rotateX(const float x)
{
	auto m = glm::rotate(glm::mat4(1.0f), x, glm::vec3(-1.0f, 0.0f, 0.0f));
	g_position = m * glm::vec4(g_position, 1.0f);
	g_direction = glm::vec3(0.0f, 0.0f, 0.0f) - g_position;
}

void Camera::rotateY(const float y)
{
	auto m = glm::rotate(glm::mat4(1.0f), y, glm::vec3(0.0f, -1.0f, 0.0f));
	g_position = m * glm::vec4(g_position, 1.0f);
	g_direction = glm::vec3(0.0f, 0.0f, 0.0f) - g_position;
}

void  Camera::rotateZ(const float z)
{
	auto m = glm::rotate(glm::mat4(1.0f), glm::pi<float>() * z / 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Camera::setOffsetX(const float& offset)
{
	g_position_offset_x = glm::vec3(offset, 0.0f, 0.0f);
}

void Camera::setOffsetY(const float& offset)
{
	g_position_offset_y = glm::vec3(0.0f, offset, 0.0f);
}


void Camera::computeViewProjectionMatrices(bool moveback, bool moveforward)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	if (width <= 0 || height <= 0)
		return;

	g_projection_matrix = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 1000.0f);

	//update the view matrix
	g_view_matrix = glm::lookAt(g_position + g_position_offset_x + g_position_offset_y, g_position + g_position_offset_x + g_position_offset_y + g_direction, up);
}

void Camera::computeViewProjectionMatrices(float fovyRadians)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	if (width <= 0 || height <= 0)
		return;

	g_projection_matrix = glm::perspective(fovyRadians, (float)(width ) / (float)height, 0.1f, 10000.0f);

	//update the view matrix
	g_view_matrix = glm::lookAt(g_position, g_position + g_direction, up);
}

void Camera::computeViewProjectionMatrices(float orthoLeft, float orthoRight, float orthoBottom, float orthoTop, float orthoNear, float orthoFar)
{
	//float cam_view_x = 20.0f;
	//float cam_view_y = 20.0f * 1.31912f;

	//g_projection_matrix = glm::ortho(cam_view_x, -cam_view_x, -cam_view_y, cam_view_y, 0.1f, 10000.0f);
	g_projection_matrix = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, orthoNear, orthoFar);

	g_view_matrix = glm::lookAt(g_position, g_position + g_direction, up);
}
