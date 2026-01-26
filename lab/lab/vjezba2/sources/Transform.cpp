#include <glm/ext/matrix_transform.hpp>

#include "Transform.h"

Transform::Transform()
    : right(1, 0, 0, 0),
    up(0, 1, 0, 0),
    front(0, 0, 1, 0),
    position(0, 0, 0, 1),
    scale(1, 1, 1, 0)
    {
}

void Transform::setPosition(const glm::vec3& pos) {
    position = glm::vec4(pos, 1.0f);
}

void Transform::setScale(const glm::vec3& scl) {
    scale = glm::vec4(scl, 0.0f);
}

void Transform::setOrientation(const glm::vec3& newRight, const glm::vec3& newUp, const glm::vec3& newFront) {
    right = glm::vec4(glm::normalize(newRight), 0.0f);
    up = glm::vec4(glm::normalize(newUp), 0.0f);
    front = glm::vec4(glm::normalize(newFront), 0.0f);
}


glm::mat4 Transform::getModelMatrix() const {
    glm::mat4 model(1.0f);
    model[0] = right * scale.x;
    model[1] = up * scale.y;
    model[2] = front * scale.z;
    model[3] = position;

    return model;
}

glm::mat4 Transform::getViewMatrix() const {
    glm::vec3 eye = glm::vec3(position.x, position.y, position.z);
    glm::vec3 center = eye + glm::vec3(front);
    glm::vec3 upVec = glm::vec3(up);

    return glm::lookAt(eye, center, upVec);
}


void Transform::globalMove(const glm::vec3& delta) {
    position += glm::vec4(delta, 0.0f);
}

void Transform::localMove(const glm::vec3& delta) {
    position += delta.x * right + delta.y * up + delta.z * front;
}


void Transform::rotate(glm::mat4 rot) {
    right = rot * right;
    up = rot * up;
    front = rot * front;
}

void Transform::rotateFPS(float xOffset, float yOffset, bool constrainPitch) {
    float sensitivity = 0.001f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    glm::mat4 yaw = glm::rotate(glm::mat4(1.0f), xOffset, glm::vec3(up));
    glm::mat4 pitch = glm::rotate(glm::mat4(1.0f), yOffset, glm::vec3(right));

    glm::mat4 rotation = yaw * pitch;
    rotate(rotation);
}

void Transform::scaleF(const glm::vec3& factor) {
    scale *= glm::vec4(factor, 0.0f);
}
