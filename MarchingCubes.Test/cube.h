#pragma once

#include <render_object.h>

class Cube : public RenderObject
{
private:

	float edgeLength;

	GLuint edgeArrayId = -1;
	GLuint edgeBuffer = -1;
	GLint edgePositionAttribute = -1;

	GLuint triangleArrayId = -1;
	GLuint triangleBuffer = -1;

	glm::vec4* v4Edges;
	glm::vec4* v4Triangles;

public:

	Cube(float);
	~Cube();

	void SetModelView(glm::mat4 mv);
	void SetProjection(glm::mat4 p);
	virtual void UpdateModel(const glm::mat4& cam_view);
	virtual void Setup();
	virtual void Draw();
};