#include "MyGLM.h"
#include <glm/glm.hpp>  
#include <glm/gtc/matrix_transform.hpp>
#include <glm/geometric.hpp>

glm::mat4 MyGLM::translate3D(glm::vec3 translateVector) {
    glm::mat4 mat(1.0f);
    mat[3] = glm::vec4(translateVector, 1.0f);
    return mat;
}

glm::mat4 MyGLM::scale3D(glm::vec3 scaleVector) {
    glm::mat4 mat(1.0f);
    mat[0][0] = scaleVector.x;
    mat[1][1] = scaleVector.y;
    mat[2][2] = scaleVector.z;
    return mat;
}

glm::mat4 MyGLM::rotate3D(glm::vec3 axis, float angle) {
    axis = glm::normalize(axis);
    float c = cos(angle);
    float s = sin(angle);
    float oneMinusC = 1.0f - c;

    glm::mat4 mat(1.0f);

    mat[0][0] = c + axis.x * axis.x * oneMinusC;
    mat[0][1] = axis.x * axis.y * oneMinusC - axis.z * s;
    mat[0][2] = axis.x * axis.z * oneMinusC + axis.y * s;

    mat[1][0] = axis.y * axis.x * oneMinusC + axis.z * s;
    mat[1][1] = c + axis.y * axis.y * oneMinusC;
    mat[1][2] = axis.y * axis.z * oneMinusC - axis.x * s;

    mat[2][0] = axis.z * axis.x * oneMinusC - axis.y * s;
    mat[2][1] = axis.z * axis.y * oneMinusC + axis.x * s;
    mat[2][2] = c + axis.z * axis.z * oneMinusC;

    return mat;
}

glm::mat4 MyGLM::lookAtMatrix(glm::vec3 eye, glm::vec3 center, glm::vec3 viewUp) {
    glm::vec3 forward = glm::normalize(center - eye);            
    glm::vec3 right = glm::normalize(glm::cross(forward, viewUp)); 
    glm::vec3 up = glm::cross(right, forward);

    glm::mat4 rotation(1.0f);
    rotation[0][0] = right.x;   rotation[1][0] = right.y;   rotation[2][0] = right.z;
    rotation[0][1] = up.x;      rotation[1][1] = up.y;      rotation[2][1] = up.z;
    rotation[0][2] = -forward.x; rotation[1][2] = -forward.y; rotation[2][2] = -forward.z;

    glm::mat4 translation = translate3D(-eye);

    return rotation * translation;
}
