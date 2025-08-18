#include <pch.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cube.h>
#include <shaders.h>

Cube::Cube(float length) : edgeLength(length)
{

}

Cube::~Cube()
{
    glDeleteBuffers(1, &triangleBuffer);
    glDeleteVertexArrays(1, &triangleArrayId);
    glDisableVertexAttribArray(edgePositionAttribute);

    glDeleteBuffers(1, &edgeBuffer);
    glDeleteVertexArrays(1, &edgeArrayId);
}

void Cube::Setup()
{
    program_id = LoadShaders(".\\cube.vert", ".\\cube.frag");

    float length = edgeLength / 2;

    const float zero[3] = { -length, -length, length };
    const float one[3] = { length, -length, length };
    const float two[3] = { -length, -length, -length };
    const float three[3] = { length, -length, -length };
    const float four[3] = { -length, length, length };
    const float five[3] = { length, length, length };
    const float six[3] = { -length, length, -length };
    const float seven[3] = { length, length, -length };

    GLfloat lines[] = {
                                zero[0], zero[1], zero[2], //40
                                four[0], four[1], four[2], 
                                zero[0], zero[1], zero[2], //10
                                one[0], one[1], one[2],
                                zero[0], zero[1], zero[2], //20
                                two[0], two[1], two[2],
                                two[0], two[1], two[2], //23
                                three[0], three[1], three[2],
                                two[0], two[1], two[2],//26
                                six[0], six[1], six[2],
                                one[0], one[1], one[2],//13
                                three[0], three[1], three[2],
                                one[0], one[1], one[2],//15
                                five[0], five[1], five[2],
                                three[0], three[1], three[2],//37
                                seven[0], seven[1], seven[2],
                                seven[0], seven[1], seven[2],//67
                                six[0], six[1], six[2],
                                four[0], four[1], four[2],//45
                                five[0], five[1], five[2],
                                seven[0], seven[1], seven[2],//57
                                five[0], five[1], five[2],
                                four[0], four[1], four[2],//46
                                six[0], six[1], six[2],
    };

    glGenVertexArrays(1, &edgeArrayId);
    glBindVertexArray(edgeArrayId);

    glGenBuffers(1, &edgeBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, edgeBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lines), lines, GL_STATIC_DRAW);
    edgePositionAttribute = glGetAttribLocation(program_id, "vPosition");
    glVertexAttribPointer(edgePositionAttribute, 3, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);
    glEnableVertexAttribArray(edgePositionAttribute);
    
    model_view = glGetUniformLocation(program_id, "model_view");
    projection = glGetUniformLocation(program_id, "projection");

    model_matrix = glm::mat4(1.0f);
}

void Cube::UpdateModel(const glm::mat4& cam_view)
{
    auto mm = glm::translate(model_matrix, glm::vec3(_X, _Y, _Z));

    auto shvec = glm::vec3(0.0f, 0.0f, 1.0f);
    auto dir = glm::vec3(1.0f, 0.0f, 0.0f);
    auto cross = glm::normalize(glm::cross(dir, shvec));
    float theta = glm::acos(glm::dot(dir, shvec));

    //mm = glm::rotate(mm, -theta, cross);
    model_view_matrix = cam_view* mm;
}

void Cube::SetProjection(glm::mat4 p)
{
    projection_matrix = p;
}

void Cube::Draw()
{
    glUniformMatrix4fv(model_view, 1, GL_FALSE, glm::value_ptr(model_view_matrix));
    glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(projection_matrix));

    glBindVertexArray(edgeArrayId);
    glDrawArrays(GL_LINES, 0, 24);
}