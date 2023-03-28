#include "neon_engine.h"

#include "user_interface.h"
#include "input.h"
#include "camera.h"
#include "rendering.h"

#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <mutex>
#include <iostream>

NeonEngine* NeonEngine::instance = nullptr;
std::mutex NeonEngine::neon_engine_mutex;

NeonEngine::NeonEngine() {
    window = nullptr;
    user_interface = nullptr;
    input = nullptr;
    rendering = nullptr;
    glfw_major_version = 3;
    glfw_minor_version = 3;
    glsl_version = "#version 330";
    window_title = "Neon Engine";
    window_width = 1920;
    window_height = 1080;
    clear_color = ImVec4(1.0f, 0.1f, 0.1f, 1.0f);
    
    deltaTime = 0.0f;
    lastFrame = 0.0f;
    firstMouse = true;
    lastX = 0.0f;
    lastY = 0.0f;
}

NeonEngine::~NeonEngine() {

}

void NeonEngine::initialize_all_components() {
    user_interface = UserInterface::get_instance();
    input = Input::get_instance();
    rendering = Rendering::get_instance();

    user_interface->initialize();
    input->initialize();
    rendering->initialize();
}

NeonEngine* NeonEngine::get_instance()
{
    std::lock_guard<std::mutex> lock(neon_engine_mutex);
    if (instance == nullptr) {
        instance = new NeonEngine();
    }
    return instance;
}

int NeonEngine::setup_glfw() {
    glfwSetErrorCallback(UserInterface::glfw_error_callback);
    if (!glfwInit())
        return 1;

    // GL 3.0 + GLSL 130
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glfw_major_version);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glfw_minor_version);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create window with graphics context
    window = glfwCreateWindow(window_width, window_height, window_title, NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //glfwSetFramebufferSizeCallback(window, Input::framebuffer_size_callback);

    // tell GLFW to capture our mouse
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSwapInterval(1); // Enable vsync
    return 0;
}

int NeonEngine::setup_glad() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    return 0;
}

void NeonEngine::clean_gflw() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

int NeonEngine::run() {
    initialize_all_components();
    if (setup_glfw() != 0) {
        return -1;
    }

    if (setup_glad() != 0) {
        return -1;
    }
    
    user_interface->setup_imgui();

    // Our state
    ImGuiIO& io = ImGui::GetIO();

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    rendering->set_opengl_state();

    rendering->set_viewport_shaders();

    rendering->set_viewport_models();

    rendering->initialize_game_objects();
    
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        user_interface->render_ui();

        // Rendering
        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
    }

    // Cleanup
    rendering->clean();
    user_interface->clean_imgui();

    clean_gflw();

    return 0;
}