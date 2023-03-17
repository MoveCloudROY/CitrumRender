#ifndef EG_CAMERA_H__
#define EG_CAMERA_H__
#include <GL/glew.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <cmath>


namespace Utils {

class Camera {
public:
    // 摄像机移动方向枚举类
    enum class Movement;
    // 向量初始化
    Camera(
        glm::vec3 position = glm::vec3{0.f, 0.f, 0.f},
        glm::vec3 worldUp  = glm::vec3{0.f, 1.f, 0.f},
        float     yaw      = YAW,
        float     pitch    = PITCH
    );
    // 单独变量初始化
    Camera(
        float posX, float posY, float posZ,
        float worldUpX, float worldUpY, float worldUpZ,
        float yaw   = YAW,
        float pitch = PITCH
    );
    Camera(const Camera&)            = default;
    Camera(Camera&&)                 = default;
    Camera& operator=(const Camera&) = default;
    Camera& operator=(Camera&&)      = default;

    // 获取 LookAt (MVP - View) 矩阵
    glm::mat4 GetViewMatrix();
    // 键盘控制摄像机
    void ProcessKeyboard(Movement direction, float deltaTime);
    // 鼠标控制摄像机俯仰/偏航角
    void ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);
    // 鼠标滚轮控制摄像机镜头缩放
    void ProcessMouseScroll(float yOffset);

private:
    // 通过欧拉角计算相机朝向向量，同时更新 m_right 和 m_up
    void updateCameraVectors();

public:
    // 相机空间属性
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp;
    // 鼠标-欧拉角
    float m_yaw;
    float m_pitch;
    // 相机属性选项
    float m_movementSpeed;
    float m_mouseSensitivity;
    float m_zoom;

    enum class Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

private:
    // 默认相机参数
    static constexpr const float YAW         = -90.0f;
    static constexpr const float PITCH       = 0.0f;
    static constexpr const float SPEED       = 2.5f;
    static constexpr const float SENSITIVITY = 0.1f;
    static constexpr const float ZOOM        = 45.f;
};

} // namespace Utils
#endif /* EG_CAMERA_H__ */
