#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw_gl3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) {
        std::cout << "Failed to initialise GLFW" << std::endl;
        return -1;
    }

    #ifdef __APPLE__
    std::cout << "I'm apple machine" << std::endl;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #endif
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "GLFW CMake starter", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to open GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    glewExperimental=true; // Needed in core profile
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;;
        return -1;
    }
    {
        // An array of 3 vectors which represents 3 vertices
        static const GLfloat g_vertex_buffer_data[] = {
            000.0f, 300.0f, 0.0f, 0.0f,
            400.0f, 300.0f, 1.0f, 0.0f,
            900.0f, 400.0f, 1.0f, 1.0f,
            100.0f, 400.0f, 0.0f, 1.0f,
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexArray va;
        VertexBuffer vb(g_vertex_buffer_data, sizeof(g_vertex_buffer_data));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(200, 0, 0));

        // Use shader
        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        Texture texture("res/textures/ChernoLogo2.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        va.Unbind();
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

        Renderer renderer;

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
        ImGui_ImplGlfwGL3_Init(window, true);

        // Setup style
        ImGui::StyleColorsDark();

        float r = 0.0f;
        float increment = 0.05f;
        glm::vec3 translation(200, -200, 0);
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
        {
            /* Render here */
            renderer.Clear();
            ImGui_ImplGlfwGL3_NewFrame();

            glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
            glm::mat4 mvp = proj * view * model;
            
            GLCall(shader.Bind());
            GLCall(shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f));
           GLCall(shader.SetUniformMat4f("u_MVP", mvp));

            renderer.Draw(va, ib, shader);

            if (r > 1.0f) {
                increment = -0.05f;
            } else if (r < 0.0f) {
                increment = 0.05f;
            }
            r += increment;

            {
                static float f = 0.0f;
                static int counter = 0;
                ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }

            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
