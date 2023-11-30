#include "utils/camera.h"

namespace Utils {

// 向量初始化
Camera::Camera(
    glm::vec3 position,
    glm::vec3 worldUp,
    float     yaw,
    float     pitch
)
    : m_front{0.f, 0.f, -1.0f}, m_movementSpeed{SPEED}, m_mouseSensitivity{SENSITIVITY}, m_zoom{ZOOM} {
    m_position = position;
    m_worldUp  = worldUp;
    m_yaw      = yaw;
    m_pitch    = pitch;
    updateCameraVectors();
}
// 单独变量初始化
Camera::Camera(
    float posX, float posY, float posZ,
    float worldUpX, float worldUpY, float worldUpZ,
    float yaw,
    float pitch
)
    : m_front{0.f, 0.f, -1.0f}, m_movementSpeed{SPEED}, m_mouseSensitivity{SENSITIVITY}, m_zoom{ZOOM} {
    m_position = {posX, posY, posZ};
    m_worldUp  = {worldUpX, worldUpY, worldUpZ};
    m_yaw      = yaw;
    m_pitch    = pitch;
    updateCameraVectors();
}
// 获取 LookAt (MVP - View) 矩阵
glm::mat4 Camera::GetViewMatrix() {

    /** 
        * What is LookAt? 

        // 设置摄像机位置
        glm::vec3 P = glm::vec3(0.f, 0.f, 3.f);
        // 设置摄像机方向
        glm::vec3 cameraTarget = glm::vec3(0.f, 0.f,0.f);
        glm::vec3 D = glm::normalize(P - cameraTarget);
        // 设置摄像机右轴
        glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
        glm::vec3 R = glm::normalize(glm::cross(up, D));
        // 设置摄像机上轴
        glm::vec3 U=glm::cross(D, R);
                    --             --    --             --
                    | R_x R_y R_z 0 |    |  1  0  0 -P_x |
        LookAt =    | U_x U_y U_z 0 |  * |  0  1  0 -P_y |
                    | D_x D_y D_z 0 |    |  0  0  1 -P_z |
                    |  0   0   0  0 |    |  0  0  0   1  |
                    --             --    --             --
    */
    // 创建摄像机 LookAt 变换矩阵 (MVP - View)
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

// 键盘控制摄像机
void Camera::ProcessKeyboard(Movement direction, float deltaTime) {
    if (!isMouseAttached)
        return;
    // 时间差越大，说明这帧渲染用时更多，需要增加速度来使用户体验相同
    float v = m_movementSpeed * deltaTime;

    switch (direction) {
    case Movement::FORWARD: {
        m_position += m_front * v;
        break;
    }
    case Movement::BACKWARD: {
        m_position -= m_front * v;
        break;
    }
    case Movement::LEFT: {
        m_position -= m_right * v;
        break;
    }
    case Movement::RIGHT: {
        m_position += m_right * v;
        break;
    }
    }
}

// 鼠标控制摄像机俯仰/偏航角
void Camera::ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch) {
    if (!isMouseAttached)
        return;
    xOffset *= m_mouseSensitivity;
    yOffset *= m_mouseSensitivity;

    m_yaw += xOffset;
    m_pitch += yOffset;
    if (constrainPitch)
        m_pitch = m_pitch > 89.0f ? 89.0f : m_pitch < -89.0f ? -89.0f
                                                             : m_pitch;
    updateCameraVectors();
}

// 鼠标滚轮控制摄像机镜头缩放
void Camera::ProcessMouseScroll(float yOffset) {
    if (!isMouseAttached)
        return;
    m_zoom -= yOffset;
    if (m_zoom < 1.0f)
        m_zoom = 1.0f;
    if (m_zoom > 45.0f)
        m_zoom = 45.0f;
}

// 启用鼠标绑定
void Camera::AttachMouse() {
    isMouseAttached = true;
}

// 取消鼠标和摄像机的绑定，并暂存鼠标位置
void Camera::DetachMouse() {
    isMouseAttached = false;
}

// 通过欧拉角计算相机朝向向量，同时更新 m_right 和 m_up
void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = std::cos(glm::radians(m_pitch)) * std::cos(glm::radians(m_yaw));
    front.y = std::sin(glm::radians(m_pitch));
    front.z = std::cos(glm::radians(m_pitch)) * std::sin(glm::radians(m_yaw));
    m_front = glm::normalize(front);
    // 更新 m_right 和 m_up
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up    = glm::normalize(glm::cross(m_right, m_front));
}

} // namespace Utils