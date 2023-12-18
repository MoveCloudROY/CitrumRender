#include <cstdint>
#include <glm/ext/vector_float3.hpp>
#include <glm/matrix.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <spdlog/spdlog.h>

#include <core/imgui/theme.h>

#include <core/terminal/terminal_commands.h>
#include <imterm/terminal.hpp>
#include <imterm/terminal_helpers.hpp>

#include <stb/stb_image.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <functional>
#include <cmath>
#include <utils/data.h>
#include <utils/shader.h>

#include <utils/camera.h>
#include <utils/texture.h>
#include <utils/cubemap.h>
#include <utils/vertexArray.h>
#include <utils/definedata.h>

#include <core/events/KeyEvents.h>

#include <model/model.h>


using namespace EG;

constexpr int AppWindowWidth   = 1920;
constexpr int AppWindowHeight  = 1080;
int           gameWindowWidth  = 1920;
int           gameWindowHeight = 1080;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void processExit(GLFWwindow* window);
void processInput(GLFWwindow* window, int key, std::function<void(void)> func);
void processCamera(GLFWwindow* window);
void renderMainImGui(GLFWwindow* window, auto texColorBuffer);

// clang-format off

glm::vec3 pointLightPositions[] = {
    glm::vec3( 0.7f,  0.2f,  2.0f),
    glm::vec3( 2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3( 0.0f,  0.0f, -3.0f)
};

// clang-format on

glm::vec3 lightPos   = {1.4f, 1.f, 1.2f};
glm::vec3 lightColor = {1.0f, 1.0f, 1.0f};

bool hiddenCursor = false;

float deltaTime     = 0.0f; // 当前帧和上一帧的时间差
float lastFrameTime = 0.0f; // 上一帧的时间

float lastX = (float)gameWindowWidth / 2.f, lastY = (float)gameWindowHeight / 2.f;

Utils::Camera camera{glm::vec3{0.f, 0.f, 3.f}};

EG::Core::custom_command_struct               cmdstruct;
ImTerm::terminal<EG::Core::terminal_commands> terminal_log(cmdstruct);

int main(void) {
    terminal_log.theme() = ImTerm::themes::purple;
    spdlog::set_level(spdlog::level::trace);
    spdlog::default_logger()->sinks().push_back(terminal_log.get_terminal_helper());

    spdlog::info("[Log] Initialized spdlog!");
    GLFWwindow* window;
    /* Initialize the library */
    if (!glfwInit())
        return EXIT_FAILURE;


        // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 430";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(AppWindowWidth, AppWindowHeight, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        spdlog::error("[UI] Failed to create GLFW window");
        glfwTerminate();
        return EXIT_FAILURE;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        spdlog::error("[UI] Failed to initialize GLEW");
        return EXIT_FAILURE;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking Windows
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi Viewports
    io.ConfigDockingWithShift = true;                     // Only enable docking when is pressing SHIFT


    // OpenGL 版本号输出
    spdlog::info("[OpenGL] Version {}", (char*)glGetString(GL_VERSION));
    // 设置视口大小
    glViewport(0, 0, gameWindowWidth, gameWindowHeight);
    // glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // 隐藏鼠标
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // 设置鼠标事件监听函数
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);


    float xscale, yscale;
    glfwGetWindowContentScale(window, &xscale, &yscale);
    auto meanscale = (xscale + yscale) / 2.f;
    if (meanscale <= 0.0F)
        meanscale = 1.0F;
    spdlog::info("[UI] Detect HiDPI scale: {}", meanscale);
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        // style.WindowRounding              = 0.0f;
        // style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        EG::Core::theme::SetTheme();
        style.ScaleAllSizes(meanscale);
    }
    // Basic Latin, Extended Latin
    io.Fonts->AddFontFromFileTTF("../src/assets/fonts/NotoSans-Medium.ttf", 16 * meanscale, nullptr, io.Fonts->GetGlyphRangesDefault());

    // Default + Selection of 2500 Ideographs used by Simplified Chinese
    io.Fonts->AddFontFromFileTTF("../src/assets/fonts/xihei-screen.ttf", 16 * meanscale, nullptr, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());

    // Default + Hiragana, Katakana, Half-Width, Selection of 1946 Ideographs
    // io.Fonts->AddFontFromFileTTF("font.ttf", 13, nullptr, io.Fonts->GetGlyphRangesJapanese());

    io.FontGlobalScale = meanscale;

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);


    runtime::Model nanosuit{"../src/assets/nanosuit/nanosuit.obj"};

    // 创建光源 VAO
    auto lightBlockSize = Utils::cubeVertices.size() * sizeof(Utils::cubeVertices[0]);

    auto lightBlockVertexBuf = std::make_shared<Utils::VertexBuffer>(nullptr, lightBlockSize);
    lightBlockVertexBuf->SetLayout({{"position", Utils::ShaderDataType::Vecf3}});
    lightBlockVertexBuf->SetData(Utils::cubeVertices).Unbind();
    auto lightBlockVA = std::make_shared<Utils::VertexArray>();
    lightBlockVA->SetVertexBuffer(lightBlockVertexBuf);
    lightBlockVA->Unbind();

    // 创建墙面 VAO
    auto wallVertexPosSize = Utils::wallVerticesPos.size() * sizeof(Utils::wallVerticesPos[0]);
    auto wallVertexUvSize  = Utils::wallVerticesUV.size() * sizeof(Utils::wallVerticesUV[0]);

    auto wallVertexBuf = std::make_shared<Utils::VertexBuffer>(wallVertexPosSize + wallVertexUvSize, true);
    wallVertexBuf->SetSubData({"position", Utils::ShaderDataType::Vecf3}, Utils::wallVerticesPos);
    wallVertexBuf->SetSubData({"uv", Utils::ShaderDataType::Vecf2}, Utils::wallVerticesUV);
    wallVertexBuf->Unbind();
    auto wallVA = std::make_shared<Utils::VertexArray>();
    wallVA->SetVertexBuffer(wallVertexBuf);
    wallVA->Unbind();


    // 生成纹理
    auto boxDiffTex = Utils::TextureBuilder(Utils::TextureType::TEXTURE_2D)
                          .SetProps()
                          .Attach("../src/assets/container2.png")
                          .Build();
    auto boxSpecTex = Utils::TextureBuilder(Utils::TextureType::TEXTURE_2D)
                          .SetProps()
                          .Attach("../src/assets/container2.png")
                          .Build();

    auto wallDiffTex = Utils::TextureBuilder(Utils::TextureType::TEXTURE_2D)
                           .SetProps()
                           .Attach("../src/assets/normalmap/brickwall.jpg")
                           .Build();
    auto wallNormMap = Utils::TextureBuilder(Utils::TextureType::TEXTURE_2D)
                           .SetProps()
                           .Attach("../src/assets/normalmap/brickwall_normal.jpg")
                           .Build();

    // 生成着色器程序对象
    Utils::Shader gameShader("../src/shaders/vs/shader.vs", "../src/shaders/fs/shader.fs");
    Utils::Shader applicationShader("../src/shaders/vs/application.vs", "../src/shaders/fs/application.fs");
    Utils::Shader lightShader("../src/shaders/vs/light.vs", "../src/shaders/fs/light.fs");
    Utils::Shader normMapShader("../src/shaders/vs/normmap.vs", "../src/shaders/fs/normmap.fs");
    // Shader 配置
    gameShader.use();
    // 材质，光照设置
    gameShader.setVec3f("lightColor", lightColor);

    // gameShader.setVec3f("material.ambient", glm::vec3{1.0f, 0.5f, 0.31f});
    // gameShader.setVec3f("material.diffuse", glm::vec3{1.0f, 0.5f, 0.31f});
    gameShader.setInt("material.diffuse", 0);
    gameShader.setInt("material.specular", 1);
    gameShader.setFloat("material.shininess", 5.0f);

    gameShader.setVec3f("spotLights[0].position", lightPos);
    // gameShader.setVec3f("spotLights[0].direction", glm::vec3{-0.2f, -1.0f, -0.3f});
    gameShader.setVec3f("spotLights[0].ambient", glm::vec3{0.2f, 0.2f, 0.2f});
    gameShader.setVec3f("spotLights[0].diffuse", glm::vec3{0.5f, 0.5f, 0.5f}); // 将光照调暗了一些以搭配场景
    gameShader.setVec3f("spotLights[0].specular", glm::vec3{1.0f, 1.0f, 1.0f});
    gameShader.setFloat("spotLights[0].constant", 1.0f);
    gameShader.setFloat("spotLights[0].linear", 0.07f);
    gameShader.setFloat("spotLights[0].quadratic", 0.017f);
    gameShader.setFloat("spotLights[0].cutOff", glm::cos(glm::radians(12.5f)));
    gameShader.setFloat("spotLights[0].outCutOff", glm::cos(glm::radians(17.5f)));
    for (auto&& i = 0; i < 4; ++i) {
        auto item = std::format("pointLights[{}]", i);
        gameShader.setVec3f(item + ".position", pointLightPositions[i]);
        gameShader.setVec3f(item + ".ambient", glm::vec3{0.2f, 0.2f, 0.2f});
        gameShader.setVec3f(item + ".diffuse", glm::vec3{0.5f, 0.5f, 0.5f}); // 将光照调暗了一些以搭配场景
        gameShader.setVec3f(item + ".specular", glm::vec3{1.0f, 1.0f, 1.0f});
        gameShader.setFloat(item + ".constant", 1.0f);
        gameShader.setFloat(item + ".linear", 0.09f);
        gameShader.setFloat(item + ".quadratic", 0.032f);
    }

    normMapShader.use();
    // 材质，光照设置
    normMapShader.setVec3f("lightColor", lightColor);

    // normMapShader.setVec3f("material.ambient", glm::vec3{1.0f, 0.5f, 0.31f});
    // normMapShader.setVec3f("material.diffuse", glm::vec3{1.0f, 0.5f, 0.31f});
    normMapShader.setInt("material.diffuse", 0);
    normMapShader.setInt("material.normal", 1);
    normMapShader.setFloat("material.shininess", 10.0f);

    normMapShader.setVec3f("spotLights[0].position", lightPos);
    // normMapShader.setVec3f("spotLights[0].direction", glm::vec3{-0.2f, -1.0f, -0.3f});
    normMapShader.setVec3f("spotLights[0].ambient", glm::vec3{0.2f, 0.2f, 0.2f});
    normMapShader.setVec3f("spotLights[0].diffuse", glm::vec3{0.3f, 0.3f, 0.3f}); // 将光照调暗了一些以搭配场景
    normMapShader.setVec3f("spotLights[0].specular", glm::vec3{1.0f, 1.0f, 1.0f});
    normMapShader.setFloat("spotLights[0].constant", 1.0f);
    normMapShader.setFloat("spotLights[0].linear", 0.07f);
    normMapShader.setFloat("spotLights[0].quadratic", 0.017f);
    normMapShader.setFloat("spotLights[0].cutOff", glm::cos(glm::radians(12.5f)));
    normMapShader.setFloat("spotLights[0].outCutOff", glm::cos(glm::radians(17.5f)));
    for (auto&& i = 0; i < 4; ++i) {
        auto item = std::format("pointLights[{}]", i);
        normMapShader.setVec3f(item + ".position", pointLightPositions[i]);
        normMapShader.setVec3f(item + ".ambient", glm::vec3{0.2f, 0.2f, 0.2f});
        normMapShader.setVec3f(item + ".diffuse", glm::vec3{0.3f, 0.3f, 0.3f}); // 将光照调暗了一些以搭配场景
        normMapShader.setVec3f(item + ".specular", glm::vec3{1.0f, 1.0f, 1.0f});
        normMapShader.setFloat(item + ".constant", 1.0f);
        normMapShader.setFloat(item + ".linear", 0.09f);
        normMapShader.setFloat(item + ".quadratic", 0.032f);
    }

    // 光照 Shader
    lightShader.use();
    lightShader.setVec3f("lightColor", lightColor);

    applicationShader.use();
    applicationShader.setInt("screenTexture", 0);

    std::map<runtime::Texture, int> a;

    // 创建帧缓冲
    uint32_t gameWindowFbo;
    glGenFramebuffers(1, &gameWindowFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, gameWindowFbo);

    // 添加纹理附件
    uint32_t texColorBuffer;
    glGenTextures(1, &texColorBuffer);
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gameWindowWidth, gameWindowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

    // 添加缓冲对象附件用于深度测试&模板测试
    uint32_t rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, gameWindowWidth, gameWindowHeight);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        spdlog::error("[OpenGL] ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Utils::CubeMap cubeMap{
        "../src/assets/skybox/right.jpg",
        "../src/assets/skybox/left.jpg",
        "../src/assets/skybox/top.jpg",
        "../src/assets/skybox/bottom.jpg",
        "../src/assets/skybox/front.jpg",
        "../src/assets/skybox/back.jpg",
    };

    // 启用深度测试
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT);

    // Main loop
#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = NULL;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!glfwWindowShouldClose(window))
#endif
    {
        /* Poll for and process events */
        glfwPollEvents();

        processExit(window);
        //** HomeWork T4 **//
        processCamera(window);
        // 渲染至gameWindowFbo帧缓冲
        glBindFramebuffer(GL_FRAMEBUFFER, gameWindowFbo); // 帧缓冲
        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // 清理颜色和深度缓冲位
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // 创建投影矩阵 (MVP - Projection)
        auto projection = glm::perspective(glm::radians(camera.m_zoom), (float)gameWindowWidth / (float)gameWindowHeight, 0.1f, 100.f);
        // 获得观察矩阵 (MVP - View)
        glm::mat4 view = camera.GetViewMatrix();


        // 绑定 VAO
        wallVA->Bind();

        // 绑定纹理
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, wallDiffTex);
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, wallNormMap);
        normMapShader.use();
        normMapShader.setMatrix4f("view", view);
        normMapShader.setMatrix4f("projection", projection);

        normMapShader.setVec3f("viewPos", camera.m_position);

        normMapShader.setVec3f("spotLights[0].position", camera.m_position);
        normMapShader.setVec3f("spotLights[0].direction", camera.m_front);


        // Draw Box
        // for (size_t i = 0; i < 10; ++i) {
        glm::mat4 model(1.0f);
        // model = glm::translate(model, glm::vec3(0.7f, 0.2f, 2.0f));
        normMapShader.setMatrix4f("model", model);
        normMapShader.setMatrix3f("normalMat", glm::transpose(glm::inverse(glm::mat3{model})));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // }
        wallVA->Unbind();

        gameShader.use();
        gameShader.setMatrix4f("view", view);
        gameShader.setMatrix4f("projection", projection);


        gameShader.setVec3f("viewPos", camera.m_position);

        gameShader.setVec3f("spotLights[0].position", camera.m_position);
        gameShader.setVec3f("spotLights[0].direction", camera.m_front);

        glm::mat4 model_nano = glm::mat4(1.0f);
        model_nano           = glm::translate(model_nano, glm::vec3(0.0f, -7.0f, -3.0f)); // translate it down so it's at the center of the scene
        model_nano           = glm::scale(model_nano, glm::vec3(0.7f));                   // it's a bit too big for our scene, so scale it down
        gameShader.setMatrix4f("model", model_nano);

        gameShader.setMatrix3f("normalMat", glm::transpose(glm::inverse(glm::mat3{model_nano})));
        nanosuit.Draw(gameShader);

        //=========== Draw Light ===========//
        // Light Shader Compose

        lightBlockVA->Bind();

        lightShader.use();
        lightShader.setMatrix4f("view", view);
        lightShader.setMatrix4f("projection", projection);
        lightShader.setVec3f("lightColor", lightColor);
        for (auto&& posVec : std::span{pointLightPositions}) {
            glm::mat4 mlight{1.0f};
            // 创建模型矩阵 (MVP - Model)
            mlight = glm::translate(mlight, posVec);
            mlight = glm::scale(mlight, glm::vec3{0.4});

            lightShader.setMatrix4f("model", mlight);
            // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        lightBlockVA->Unbind();


        // In order to make skybox look infinitely far away
        // SkyBox should not be moved due to Camera movement
        // So we need make the Shift part of the View matrix is 0
        cubeMap.Draw(glm::mat4(glm::mat3(view)), projection);

        //============= Use EBO! =============//
        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        // 返回默认帧缓冲
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        renderMainImGui(window, texColorBuffer);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        // 计算每帧用时和时间差
        float currentFrameTime = glfwGetTime();
        deltaTime              = currentFrameTime - lastFrameTime;
        lastFrameTime          = currentFrameTime;
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glDeleteFramebuffers(1, &gameWindowFbo);
    // glDeleteBuffers(1, &VBO); // TODO: TEMP

    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static bool first_in = true;

    if (first_in) {
        first_in = false;
        lastX    = xpos;
        lastY    = ypos;
    }
    float xOffset = xpos - lastX;
    float yOffset = lastY - ypos;
    lastX         = xpos;
    lastY         = ypos;

    camera.ProcessMouseMovement(xOffset, yOffset, true);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
    camera.ProcessMouseScroll(yOffset);
}

void processExit(GLFWwindow* window) {
    // if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
    //     glfwSetWindowShouldClose(window, true);
}

void processCamera(GLFWwindow* window) {
    using Utils::Camera;
    // 摄像机移动控制
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(Camera::Movement::FORWARD, deltaTime);
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(Camera::Movement::LEFT, deltaTime);
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(Camera::Movement::BACKWARD, deltaTime);
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(Camera::Movement::RIGHT, deltaTime);
    } else if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS) {
        spdlog::info("[UI] Show Cursor due to press F5");
        hiddenCursor = false;
        camera.DetachMouse();
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}


void processInput(GLFWwindow* window, int key, std::function<void(void)> func) {
    using Utils::Camera;

    if (glfwGetKey(window, key) == GLFW_PRESS)
        func();
}

void renderMainImGui(GLFWwindow* window, auto colorBuffer) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    float       menuHeight   = 0;
    static bool showing_term = true, showing_demo = false;
    ImGui::BeginMainMenuBar();
    {
        if (ImGui::BeginMenu("File")) {
            ImGui::Button("Test");

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Windows")) {
            if (ImGui::Button("Logger")) {
                ImGui::Begin("Logger");
                showing_term = true;
                ImGui::End();
            }
            ImGui::Checkbox("Demo", &showing_demo);
            // auto showing_demo = ImGui::Selectable("Demo", true);

            ImGui::EndMenu();
        }

        menuHeight = ImGui::GetFrameHeight();
    }
    ImGui::EndMainMenuBar();
    ImGui::DockSpaceOverViewport();
    ImGui::Begin("Object Lists");
    {
        ImGui::Text("Object Occupy");
    }
    ImGui::End();

    ImGui::Begin("Game Window");
    {
        ImGui::SetNextWindowSize(ImVec2(400, 300));
        ImGui::BeginChild("GameRender");
        auto ret         = ImGui::GetWindowSize();
        gameWindowWidth  = ret.x;
        gameWindowHeight = ret.y;
        ImGui::ImageButton((ImTextureID)(unsigned long)colorBuffer, ret, ImVec2(0, 1), ImVec2(1, 0)); //自定义GUI内容
        bool isItemClicked = ImGui::IsItemClicked();
        if (hiddenCursor == false && isItemClicked) {
            spdlog::info("[UI] Hidden Cursor due to click window");
            camera.AttachMouse();
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            hiddenCursor = true;
        }
        ImGui::EndChild();
    }
    ImGui::End();

    ImGui::Begin("Assets");
    {
        ImGui::Text("Assets Occupy");
    }
    ImGui::End();

    ImGui::Begin("Config");
    {
        ImGui::Text("Config Occupy");
    }
    ImGui::End();

    if (showing_term) {
        showing_term = terminal_log.show();
    }
    if (showing_demo) {
        ImGui::ShowDemoWindow();
    }
    ImGui::Render();
}