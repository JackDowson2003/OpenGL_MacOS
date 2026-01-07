//
// Created by chenlong on 2026/1/7.
//

#include "TestClearColor.h"
#include <iostream>
#include <glad/glad.h>
#include "Renderer.h"
#include "imgui.h"
namespace test
{
    TestClearColor::TestClearColor()
    :m_ClearColor {0.2f,0.3f,0.8f,1.0f}
    {

    }

    TestClearColor::~TestClearColor()
    {
        std::cout << "Destroying the test" << std::endl;
    }

    void TestClearColor::OnUpdate(float deltaTime)
    {
        std::cout << "OnUpdate the test" << std::endl;

    }

    void TestClearColor::OnRender()
    {
        GLCall(glClearColor(m_ClearColor[0],m_ClearColor[1],
                            m_ClearColor[2],m_ClearColor[3]
                            )); //设置清除成相关的颜色 rgba
        GLCall(glClear(GL_COLOR_BUFFER_BIT)); // clear buffer of color data
    }

    void TestClearColor::OnImGUIRender()
    {
        ImGui::ColorEdit4("Clear Color",m_ClearColor);
    }
}