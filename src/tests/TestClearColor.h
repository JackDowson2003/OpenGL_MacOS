//
// Created by chenlong on 2026/1/7.
//
#include "Test.h"

namespace test
{

    class TestClearColor : public Test
    {
    private:
        //RGBA value what I want to clear
        float m_ClearColor[4]; //actual color RGBA
    public:

        TestClearColor();

        ~TestClearColor();

        virtual void OnUpdate(float deltaTime) override;

        virtual void OnRender() override;

        virtual void OnImGUIRender() override;


    };
}