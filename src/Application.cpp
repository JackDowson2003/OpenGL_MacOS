// 1️⃣ 先 STL
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"


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

    GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL Debug", nullptr, nullptr);
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
        float positions[8] = {
                .5f, -.5f,  // 0
                .5f, .5f,  // 1
                -.5f, -.5f,   // 2
                -.5f, .5f //3
        };

        //indices
        unsigned int indices[] = {
                2, 0, 1,
                2, 1, 3
        };

        // === 3. 生成并绑定 VAO (Vertex Array Object) ===
        ///顶点状态的集合
//        unsigned int vao; //负责记录信息（启用了什么顶点属性，定点属性的格式，ibo的绑定关系，）
//        GLCall(glGenVertexArrays(1, &vao));
//        GLCall(glBindVertexArray(vao));

        ////负责记录信息（启用了什么顶点属性，定点属性的格式，ibo的绑定关系，）
        VertexArray va;
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
        VertexBuffer vb(positions, 8 * sizeof(float));

        // ===  配置顶点属性指针 (这行代码现在会被记录在 VAO 中) ===
        //已经修改到 addBuffer中
//        GLCall(glEnableVertexAttribArray(0));
//        GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        //6个顶点
        IndexBuffer ib(indices, 6);

        // === 5. 解绑 VAO (良好的习惯) ===
        // 注意：解绑 VAO 前，必须确保 buffer 和属性指针已经设置好
//        GLCall(glBindVertexArray(0));
        va.UnBind();
        // === 着色器部分 (保持不变) ===
        ///Create Shader
//        ShaderProgramSource source = ParseShader("../res/shaders/Basic.shader");
//    std::cout << (source.VertexSource) << std::endl;
//    std::cout << (source.FragmentSource) << std::endl;
//        unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
        Shader shader("../res/shaders/Basic.shader");
        // === 6. 绘制前绑定 ibo ===
        // 必须在绘制前绑定，否则是黑屏
        //GLCall(glBindVertexArray(vao))
//        GLCall(glUseProgram(shader));

        va.Bind(); //因为之前解绑过
        shader.Bind(); //va绑定了 容器才可以记录到shader的存在
        shader.setUniform4f("u_Color", 0.8f, .3f, .8f, 1.0f);

        //set fragment shader color 必须先绑定再设置 不然获取不到uniform
//        int colorLocation = glGetUniformLocation(shader, "u_Color");
//        ASSERT(colorLocation != -1); //out of date

        va.UnBind();
        shader.UnBind();
        vb.UnBind();
        ib.UnBind();
//        GLCall(glBindVertexArray(0));
//        GLCall(glUseProgram(0));
//        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
//        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
        Renderer renderer;

        float r = 0.8f;
        float g = 0.3f;
        float b = 0.8f;
        float a = 1.0f;
        float increment = .05f;
        /* 渲染循环 */
        while (!glfwWindowShouldClose(window))
        {
//            GLCall(glClear(GL_COLOR_BUFFER_BIT));
            renderer.Clear();
            ///等待渲染的时候再绑定
            shader.Bind();
            shader.setUniform4f("u_Color", r, g, b, a); //需要shader绑定好菜可以设置
//            va.Bind();
//            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
//            GLCall(glUseProgram(shader));
//            GLCall(glUniform4f(colorLocation, r, g, b, a));
//            GLCall(glBindVertexArray(vao));
            renderer.Draw(va, ib, shader);
            // 现在使用 VAO 记录的状态来绘制

//        glDrawArrays(GL_TRIANGLES, 0, 3);
            GLClearError();
//        glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr);

            if (r > 1.0f)
                increment = -.5f;
            else if (r < 0.0f)
                increment = .5f;
            r += increment;


            GLCall(glfwSwapBuffers(window));
            GLCall(glfwPollEvents());
        }
        ///结束代码块后自动调用构析函数并释放vao,ib,va,vb 的内存和video mem
    }


    // 清理
//    GLCall(glDeleteVertexArrays(1, &vao)); // 别忘了删除 VAO
//    GLCall(glDeleteBuffers(1, &buffer));
//    GLCall(glDeleteBuffers(1, &ibo));
//    vb.UnBind();
//    ib.UnBind();
//    GLCall(glDeleteProgram(shader));
    GLCall(glfwTerminate());
    return 0;
}
