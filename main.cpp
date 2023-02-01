#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cmath>

#include <data.h>
#include <shader.h>
#include <stb_image.h>

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

std::vector<float> vertices {
//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
};


std::vector<unsigned int> indices {
    // 注意索引从0开始! 
    // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
    // 这样可以由下标代表顶点组合成矩形
    0, 1, 3,
    1, 2, 3,
};


int main(void)
{
    GLFWwindow* window;
    /* Initialize the library */
    if (!glfwInit())
        return EXIT_FAILURE;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
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

    // 创建 VAO
    unsigned int VAO = Utils::createVAO(vertices);
    // 创建 EBO
    Utils::createEBO(indices);

    // 如何从VBO解析顶点属性,并将状态保存到 VAO
    // '0' => Corresponding `location` in vertex shader Attribute value
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // position
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // color
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // texture
    // 以顶点属性位置值作为参数，启用顶点属性
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // 生成着色器程序对象
    Utils::Shader ourShader("../shaders/vs/shader.vs", "../shaders/fs/shader.fs");
    
    // 生成纹理
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // stb 加载图片
    int width, height, nrChannels;
    unsigned char * data = stbi_load("../assets/container.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 使用 ShaderProgram
        ourShader.use();
        // 绑定 VAO
        glBindVertexArray(VAO);
        // 绑定纹理
        glBindTexture(GL_TEXTURE_2D, texture);

        //** HomeWork T2 **//
        
        auto time_value = glfwGetTime();
        ourShader.setFloat("bias", [](double time)->float{
            constexpr double use_time = 2.0;
            // return time > use_time? 0.5 : 0.5 * time / use_time;
            return time;
        }(time_value));
        
        //=========== Just Use VBO ===========//
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        //============= Use EBO! =============//
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}