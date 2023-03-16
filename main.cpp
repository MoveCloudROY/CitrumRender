#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdlib>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <functional>
#include <cmath>

#include <data.h>
#include <shader.h>
#include <stb_image.h>
#include <todo.h>


constexpr int WIDTH  = 800;
constexpr int HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void processExit(GLFWwindow* window);
void processInput(GLFWwindow* window, int key, std::function<void(void)> func);

// clang-format off
std::vector<float> vertices = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

std::vector<unsigned int> indices {
    // 注意索引从0开始! 
    // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
    // 这样可以由下标代表顶点组合成矩形
    0, 1, 3,
    1, 2, 3,
};

// 立方体位置
std::vector<glm::vec3> cubePositions = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f), 
    glm::vec3(-3.8f, -2.0f, -12.3f), 
    glm::vec3( 2.4f, -0.4f, -3.5f), 
    glm::vec3(-1.7f,  3.0f, -7.5f), 
    glm::vec3( 1.3f, -2.0f, -2.5f), 
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

// clang-format on

float alpha = 0.0;

glm::vec3 cameraPos     = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront   = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp      = glm::vec3(0.0f, 1.0f, 0.0f);
float     deltaTime     = 0.0f; // 当前帧和上一帧的时间差
float     lastFrameTime = 0.0f; // 上一帧的时间

float lastX = (float)WIDTH / 2.f, lastY = (float)HEIGHT / 2.f;
float yaw = -80.f, pitch = 0.0f, cameraFov = 45.0f;

int main(void) {
    GLFWwindow* window;
    /* Initialize the library */
    if (!glfwInit())
        return EXIT_FAILURE;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    // OpenGL 版本号输出
    std::cout << glGetString(GL_VERSION) << std::endl;
    // 设置视口大小
    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // 创建 VAO
    unsigned int VAO = Utils::createVAO(vertices);
    // 创建 EBO
    Utils::createEBO(indices);

    // 如何从VBO解析顶点属性,并将状态保存到 VAO
    // '0' => Corresponding `location` in vertex shader Attribute value
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);                   // position
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); // texture_box
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(5 * sizeof(float))); // texture_smile

    // 以顶点属性位置值作为参数，启用顶点属性
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // 生成着色器程序对象
    Utils::Shader ourShader("../shaders/vs/shader.vs", "../shaders/fs/shader.fs");

    // 生成纹理
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glGenTextures(1, &texture2);
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // stb 加载图片

    unsigned char* texture1_data =
        stbi_load("../assets/container.jpg", &width, &height, &nrChannels, 0);
    if (texture1_data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture1_data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(texture1_data);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    // GL_REPEAT            重复纹理图像
    // GL_MIRRORED_REPEAT   和GL_REPEAT一样，但每次重复图片是镜像放置的
    // GL_CLAMP_TO_EDGE     纹理坐标会被约束在0到1之间，超出的部分会重复纹理坐标的边缘，产生一种边缘被拉伸的效果
    // GL_CLAMP_TO_BORDER   超出的坐标为用户指定的边缘颜色。
    //
    // # WARNING #
    //    根据 https://stackoverflow.com/questions/26589683/access-violation-when-calling-gltextureparameteri-with-opengl-and-devil
    //    这里原先使用 glTextureParameteri(), 为 OpenGL 4.5 中的新入口点，
    //    这个想法是您可以在不绑定对象的情况下修改对象的状态。这可以通过减少 API 调用次数和驱动程序开销来提高效率。
    //    与相应的旧入口点相比，这些新入口点将对象名称作为第一个参数，而旧入口点将目标作为第一个参数。
    //    原先第一个参数为目标
    //       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //    新的则可以使用对象名称
    //       glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // stb 加载图片
    unsigned char* texture2_data =
        stbi_load("../assets/awesomeface.png", &width, &height, &nrChannels, 0);
    if (texture2_data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture2_data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(texture2_data);

    // 启用 Shader
    ourShader.use();
    // 设置纹理坐标
    ourShader.setInt("texture_box", 0);
    ourShader.setInt("texture_smile", 1);


    // 启用深度测试
    glEnable(GL_DEPTH_TEST);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        processExit(window);
        //** HomeWork T4 **//
        processInput(window, GLFW_KEY_DOWN, [&]() {
            alpha -= 0.005f;
            if (alpha <= 1e-7)
                alpha = 0.0f;
            ourShader.setFloat("alpha", alpha);
        });
        processInput(window, GLFW_KEY_UP, [&]() {
            alpha += 0.005f;
            if (alpha > 1.0f)
                alpha = 1.0f;
            ourShader.setFloat("alpha", alpha);
        });
        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // 清理颜色和深度缓冲位
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        int screenW, screenH;
        glfwGetFramebufferSize(window, &screenW, &screenH);

        // 创建投影矩阵 (MVP - Projection)
        auto projection = glm::perspective(glm::radians(cameraFov), (float)screenW / (float)screenH, 0.1f, 100.f);

        // 使用 ShaderProgram
        ourShader.use();
        // 绑定 VAO
        glBindVertexArray(VAO);
        // 绑定纹理
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);


        float radius = 10.0f;
        float camX   = sin(glfwGetTime()) * radius;
        float camZ   = cos(glfwGetTime()) * radius;
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
            LookAt = | U_x U_y U_z 0 |  * |  0  1  0 -P_y |
                     | D_x D_y D_z 0 |    |  0  0  1 -P_z |
                     |  0   0   0  0 |    |  0  0  0   1  |
                     --             --    --             --
        */
        // 创建摄像机 LookAt 变换矩阵 (MVP - View)
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        ourShader.setMatrix4f("view", view);
        ourShader.setMatrix4f("projection", projection);

        for (std::size_t i = 0; i < 10; ++i) {
            // 创建模型矩阵 (MVP - Model)
            glm::mat4 model{1.0f};
            model = glm::translate(model, cubePositions[i]);
            float angle;
            if (i == 0 || i % 3 == 0)
                angle = 20.f * (i + 1);
            else
                angle = 0;
            model = glm::rotate(model, glm::radians(angle) * (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMatrix4f("model", model);

            // //=========== Just Use VBO ===========//
            glDrawArrays(GL_TRIANGLES, 0, 36);
            //============= Use EBO! =============//
            // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        // 计算每帧用时和时间差
        float currentFrameTime = glfwGetTime();
        deltaTime              = currentFrameTime - lastFrameTime;
        lastFrameTime          = currentFrameTime;
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static bool first_in = true;
    constexpr float sensitibity = 0.05f;
    
    if (first_in) {
        first_in = false;
        lastX    = xpos;
        lastY    = ypos;
    }
    float xOffset = xpos - lastX;
    float yOffset = lastY - ypos;
    lastX         = xpos;
    lastY         = ypos;
    
    xOffset *= sensitibity;
    yOffset *= sensitibity;

    yaw += xOffset;
    pitch += yOffset;

    pitch = pitch > 89.0f ? 89.0f : pitch < -89.0f ? -89.0f
                                                   : pitch;
    glm::vec3 front;
    front.x     = std::cos(glm::radians(pitch)) * std::cos(glm::radians(yaw));
    front.y     = std::sin(glm::radians(pitch));
    front.z     = std::cos(glm::radians(pitch)) * std::sin(glm::radians(yaw));
    cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
    // constexpr float sensitibity = 0.05f;
    if (cameraFov >= 1.0f && cameraFov <= 45.0f)
        cameraFov -= yOffset;
    if (cameraFov <= 1.0f)
        cameraFov = 1.0f;
    if (cameraFov >= 45.0f)
        cameraFov = 45.0f;
}

void processExit(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void processInput(GLFWwindow* window, int key, std::function<void(void)> func) {
    // 时间差越大，说明这帧渲染用时更多，需要增加速度来使用户体验相同
    float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, key) == GLFW_PRESS)
        func();

    // 摄像机移动控制
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += cameraSpeed * cameraFront;
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        // 正则化使左右移动速度相等
        cameraPos -= cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= cameraSpeed * cameraFront;
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos += cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
    }
}
