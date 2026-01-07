// 1️⃣ 先 STL
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"

//math libs
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

//imgui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

//struct ShaderProgramSource
//{
//    std::string VertexSource;
//    std::string FragmentSource;
//};

[[maybe_unused]] inline void glCout(const GLubyte *str)
{
    for (; *str != '\0'; str++)
    {
        std::cout.put(static_cast<char>(*str));
    }
    std::cout << std::endl;
}

/*static ShaderProgramSource ParseShader(const std::string &filePath)
{
    std::ifstream ifstream(filePath);
    std::vector<int> vec;

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2]; //vertex fragment
    ShaderType type = ShaderType::NONE;
    while (getline(ifstream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        } else
        {
            ss[(int) type] << line << '\n';
        }
    }
    return {ss[0].str(), ss[1].str()};
}*/

/**
 * 编译着色器函数
 * @param type 着色器类型（GL_VERTEX_SHADER 或 GL_FRAGMENT_SHADER）
 * @param source 着色器源代码字符串
 * @return 返回编译成功后的着色器ID，若编译失败返回0
 */
/*static unsigned int CompileShader(unsigned int type, const std::string &source)
{
    // 创建指定类型的着色器对象
    GLCall(unsigned int id = glCreateShader(type));
    // 获取源代码的C风格字符串指针
    const char *src = source.c_str();
    // 将源代码附加到着色器对象
    GLCall(glShaderSource(id, 1, &src, nullptr));
    // 编译着色器
    GLCall(glCompileShader(id));

    // 检查编译状态
    int result;
    // 查询着色器的编译状态
    // glGetShaderiv 返回的信息将存储在 result 中，用于判断着色器是否编译成功
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        // 如果编译失败，获取错误信息长度
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        // 使用alloca在栈上分配内存存储错误信息
        char *message = (char *) alloca(sizeof(char) * length);
        // 获取并输出错误信息
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Compiled " << ((type == GL_VERTEX_SHADER) ? "Vertex" : "Fragment") << " Shader Error"
                  << std::endl;
        std::cout << message << std::endl;
        // 删除编译失败的着色器
        GLCall(glDeleteShader(id));
        return 0; // 返回0表示编译失败
    }
    return id; // 返回编译成功的着色器ID
}*/

/*static unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader)
{
    GLCall(unsigned int program = glCreateProgram(););
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));
    return program;
}*/

/**
 * create VAO -> VBO (BIND ,SET DATA)
 * -> BUFFER (VERTEX DATA) -> CREATE SHADER -> BIND PROGRAM -> DRAW
 * */
int main()
{
    if (!glfwInit())
        return -1;

    // ---- macOS OpenGL 配置 ----
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
    GLFWwindow *window = glfwCreateWindow((int) (940 * main_scale), (int) (540 * main_scale), "OpenGL Debug", nullptr,
                                          nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        glfwTerminate();
        return -1;
    }

    // 打印版本
    glCout(glGetString(GL_VERSION));

    //这个域会自动销毁vf ib
    {
        // === 1. 定义顶点数据 ===
        float positions[16] = {
                -50.0f, 50.0f, 0.0f, 0.0f,// 2
                50.0f, 50.0f, 1.0f, 0.0f,   // 0
                50.0f, -50.0f, 1.0f, 1.0f,// 1
                -50.0f, -50.5f, 0.0f, 1.0f     //3
        }; /// 第三列 四列是我们的tex 坐标
        float positions2[16] = {
                100.0f, 50.0f, 0.0f, 0.0f,// 2
                200.0f, 50.0f, 1.0f, 0.0f,   // 0
                200.0f, -50.0f, 1.0f, 1.0f,// 1
                100.0f, -50.5f, 0.0f, 1.0f     //3
        }; /// 第三列 四列是我们的tex 坐标 一般不同的渲染需要不同

        //indices
        unsigned int indices[] = {
                0, 1, 2,
                2, 3, 0
        };

        ///渲染带透明度的纹理需要设置下面两行
        GLCall(glEnable(GL_BLEND));              // 1. 启用混合
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); // 2. 设置混合函数

        // === 3. 生成并绑定 VAO (Vertex Array Object) ===
        ///顶点状态的集合
/*//        unsigned int vao; //负责记录信息（启用了什么顶点属性，定点属性的格式，ibo的绑定关系，）
//        GLCall(glGenVertexArrays(1, &vao));
//        GLCall(glBindVertexArray(vao));*/
        ////负责记录信息（启用了什么顶点属性，定点属性的格式，ibo的绑定关系，）
        VertexArray va; //vao
        VertexArray va2; //vao
//        va.Bind(); //在我们addBuffer的时候绑定
        /*// === 2. 配置 buffer (Vertex Buffer Object) ===
        unsigned int buffer;
        GLCall(glGenBuffers(1, &buffer));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
        GLCall(glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), positions, GL_STATIC_DRAW));
        // ===  配置顶点属性指针 (这行代码现在会被记录在 VAO 中) ===
        GLCall(glEnableVertexAttribArray(0));
        GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));

        // 这是你需要的关键部分 index buffer obj
        unsigned int ibo;
        GLCall(glGenBuffers(1, &ibo));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));**/
        //instead of my def class
//        VertexBuffer vb(positions, 4*2 * sizeof(float));
        VertexBuffer vb(positions, 4 * 4 * sizeof(float)); //额外加上了纹理坐标 (从哪里开始采样) size是字节数
        VertexBuffer vb2(positions2, 4 * 4 * sizeof(float)); //额外加上了纹理坐标 (从哪里开始采样) size是字节数
/*        // ===  配置顶点属性指针 (这行代码现在会被记录在 VAO 中) ===
        //已经修改到 addBuffer中
//        GLCall(glEnableVertexAttribArray(0));
//        GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));*/
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);
        va2.AddBuffer(vb2, layout);
        //6个顶点
        IndexBuffer ib(indices, 6);
        // === 5. 解绑 VAO (良好的习惯) ===
/*        // 注意：解绑 VAO 前，必须确保 buffer 和属性指针已经设置好
//        GLCall(glBindVertexArray(0));*/
        va.UnBind();
        va2.UnBind();
        // === 着色器部分 (保持不变) ===
        ///Create Shader
/*//        ShaderProgramSource source = ParseShader("../res/shaders/Basic.shader");
//    std::cout << (source.VertexSource) << std::endl;
//    std::cout << (source.FragmentSource) << std::endl;
//        unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
        // === 6. 绘制前绑定 ibo ===
        // 必须在绘制前绑定，否则是黑屏
        //GLCall(glBindVertexArray(vao))
//        GLCall(glUseProgram(shader));*/

        /*
          -.5f, -.5f, 0.0f, 0.0f,// 2
          .5f, -.5f, 1.0f, 0.0f,   // 0
          .5f, .5f, 1.0f, 1.0f,// 1
          -.5f, .5f, 0.0f, 1.0f     //3
          100.0f, 100.0f, 0.0f, 0.0f,// 2
          200.5f, 100.0f, 1.0f, 0.0f,   // 0
          200.5f, 200.0f, 1.0f, 1.0f,// 1
          100.5f, 200.5f, 0.0f, 1.0f     //3
         * */
        ///创建一个正交投影矩阵 orthogonal projection matrix
        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); //单位矩阵


        va.Bind(); //因为之前解绑过
        Shader shader("../res/shaders/Basic.shader");
        shader.Bind(); //va绑定了 容器才可以记录到shader的存在
        shader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
//        shader.SetUniformMat4f("u_MVP", MVP); // u_MVP * position


        ///Texture
        Texture texture = Texture("../res/logo.png"); //复用
        texture.Bind(0);
        Texture flower = Texture("../res/flower.png"); //复用
        flower.Bind(1);
//        shader.SetUniform1i("u_Texture", 0); //匹配我们init传入的slot 值

/*//        set fragment shader color 必须先绑定再设置 不然获取不到uniform
//        int colorLocation = glGetUniformLocation(shader, "u_Color");
//        ASSERT(colorLocation != -1); //out of date*/
        va.UnBind();
        va2.UnBind();
        vb.UnBind();
        vb2.UnBind();
        ib.UnBind();
        shader.UnBind();
        texture.UnBind();
        flower.UnBind();
/*//        GLCall(glBindVertexArray(0));
//        GLCall(glUseProgram(0));
//        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
//        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));*/
        Renderer renderer;

        //imgui
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void) io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
//    ImGui::StyleColorsLight();

        // Setup scaling
        ImGuiStyle &style = ImGui::GetStyle();
        style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
        style.FontScaleDpi = main_scale;        // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        // Our state
        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        ///translation variable
        glm::vec3 translation(50.0f, 200.0f, 0);
        glm::vec3 translationB(50.0f, 200.0f, 0);
        // Main loop
        float r = 0.8f;
        float g = 0.3f;
        float b = 0.8f;
        float a = 1.0f;
        float increment = .05f;
        /* 渲染循环 */
        while (!glfwWindowShouldClose(window))
        {
// ===================== 渲染 Scene =====================

            if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
            {
                ImGui_ImplGlfw_Sleep(10);
                continue;
            }

            // ===================== ImGui New Frame =====================
            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            renderer.Clear();                 // ⭐ 一帧只 Clear 一次
            ImGui::NewFrame();

            ///等待渲染的时候再绑定
            shader.Bind();
            shader.SetUniform4f("u_Color", r, g, b, a);
            {
                texture.Bind(0);
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
                glm::mat4 MVP = proj * view * model;
                shader.SetUniform1i("u_Texture", 0); //匹配我们init传入的slot 值
                shader.SetUniformMat4f("u_MVP", MVP); // u_MVP * position

                renderer.Draw(va, ib, shader);
            }
            {
                flower.Bind(1);
                glm::mat4 modelB = glm::translate(glm::mat4(1.0f), translationB);
                glm::mat4 MVP_B = proj * view * modelB;
                shader.SetUniform1i("u_Texture", 1); //匹配我们init传入的slot 值
                shader.SetUniformMat4f("u_MVP", MVP_B); // u_MVP * position
                renderer.Draw(va2, ib, shader);
            }
            // 现在使用 VAO 记录的状态来绘制

            if (r > 1.0f)
                increment = -.5f;
            else if (r < 0.0f)
                increment = .5f;
            r += increment;

            // ===================== ImGui UI =====================
            // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()!)
            if (show_demo_window)
                ImGui::ShowDemoWindow(&show_demo_window);

            {
                ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
                ImGui::SliderFloat3("Translation x", &translation.x, 0.0f,
                                    960.0f);
                ImGui::SliderFloat3("Translation x B", &translationB.x, 0.0f,
                                    960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::End();
            }
            // 3. Show another simple window.
            if (show_another_window)
            {
                ImGui::Begin("Another Window", &show_another_window);
                ImGui::Text("Hello from another window!");
                if (ImGui::Button("Close Me"))
                    show_another_window = false;
                ImGui::End();
            }

            // ===================== ImGui Render =====================
            ImGui::Render();
            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);

            // ⚠️ 不再 glClear，这里是 ImGui 叠加
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            // ===================== Swap =====================
            glfwSwapBuffers(window);          // ⭐ 一帧只 Swap 一次
            // ===================== 事件 =====================
            glfwPollEvents();

        }

        ///结束代码块后自动调用构析函数并释放vao,ib,va,vb 的内存和video mem
    }
    GLCall(glfwTerminate());
    return 0;
}
