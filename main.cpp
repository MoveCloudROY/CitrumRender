#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
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


constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processExit(GLFWwindow *window);
void processInput(GLFWwindow *window, int key, std::function<void(void)> func);

std::vector<float> vertices {
//     ---- 位置 ----       ---- 颜色 ----     ---- 纹理坐标 ----                               //**** HomeWork 2 ****//
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   1.7f, 1.7f,// 右上
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   1.7f, 0.3f, // 右下
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   0.3f, 0.3f, // 左下
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,   0.3f, 1.7f// 左上
};


std::vector<unsigned int> indices {
    // 注意索引从0开始! 
    // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
    // 这样可以由下标代表顶点组合成矩形
    0, 1, 3,
    1, 2, 3,
};

float alpha = 0.0;

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0); // position
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float))); // color
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(6 * sizeof(float))); // texture_box
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(8 * sizeof(float))); // texture_smile
    // 以顶点属性位置值作为参数，启用顶点属性
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
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
    
    unsigned char * texture1_data = stbi_load("../assets/container.jpg", &width, &height, &nrChannels, 0);
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
    unsigned char * texture2_data = stbi_load("../assets/awesomeface.png", &width, &height, &nrChannels, 0);
    if (texture2_data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture2_data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(texture2_data);
    ourShader.use();
    ourShader.setInt("texture_box", 0);
    ourShader.setInt("texture_smile", 1);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        processExit(window);

        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 使用 ShaderProgram
        ourShader.use();
        // 绑定 VAO
        glBindVertexArray(VAO);
        // 绑定纹理
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        
        auto time_value = glfwGetTime();

        // 如果使用的是 glm 0.9.9 及以上版本，不能使用 
        //     glm::mat4 trans
        // 而应如下显式声明
        glm::mat4 trans{1.0f};
        trans = glm::translate(trans, glm::vec3(std::cos(time_value), std::sin(time_value), 0.0f));
        trans = glm::scale(trans, glm::vec3(0.6, 0.6, 0.6));
        trans = glm::rotate(trans, static_cast<float>(time_value), glm::vec3(0.0, 0.0, 1.0));

        unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

        //** HomeWork T4 **//
        processInput(window, GLFW_KEY_DOWN, [&]() {
            alpha-=0.005f;
            if (alpha <= 1e-7) alpha = 0.0f;
            ourShader.setFloat("alpha", alpha);
        });
        processInput(window, GLFW_KEY_UP, [&]() {
            alpha+=0.005f;
            if (alpha > 1.0f) alpha = 1.0f;
            ourShader.setFloat("alpha", alpha);
        });
        
        //=========== Just Use VBO ===========//
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        //============= Use EBO! =============//
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glm::mat4 trans2{1.0f};
        trans2 = glm::translate(trans2, glm::vec3(-0.5f, 0.5f, 0.0f));
        float scale = 0.5f * (std::sin(time_value) + 1.01f);
        trans2 = glm::scale(trans2, glm::vec3(scale, scale, scale));

        unsigned int transformLoc2 = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transformLoc2, 1, GL_FALSE, glm::value_ptr(trans2));
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

void processExit(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void processInput(GLFWwindow *window, int key, std::function<void(void)> func)
{
    if(glfwGetKey(window, key) == GLFW_PRESS)
        func();
}