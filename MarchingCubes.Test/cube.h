#pragma once

#include <vector>
#include <render_object.h>

class Cube : public RenderObject
{
private:

	float edgeLength;
	int cornersBitArray;
	int numberOfCorners;
	std::vector<float> meshData;

	GLuint edgeArrayId = -1;
	GLuint edgeBuffer = -1;
	GLint edgePositionAttribute = -1;

	GLuint triangleArrayId = -1;
	GLuint triangleBuffer = -1;

	GLuint cubeCornersArrayId = -1;
	GLuint cubeCornersBuffer = -1;
	GLint cubeCornersPositionAttribute = -1;

	GLuint meshArrayId = -1;
	GLuint meshBuffer = -1;
	GLint meshPositionAttribute = -1;


	glm::vec4* v4Edges;
	glm::vec4* v4Triangles;

	std::vector<float> generateVertices(float scale);

public:

	Cube(float, int, std::vector<float> &data);
	~Cube();

	void SetProjection(glm::mat4 p);
	virtual void UpdateModel(const glm::mat4& cam_view) override;
	virtual void Setup() override;
	virtual void Draw() override;
};