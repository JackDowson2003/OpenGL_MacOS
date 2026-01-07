#pragma once

namespace test
{

    class Test
    {
    public:
        Test() = default;

        virtual ~Test() = default;

        /// these functions are pure virtual func, those offer to their sons to implement.
        virtual void OnUpdate(float deltaTime) = 0;

        virtual void OnRender() = 0;

        virtual void OnImGUIRender() = 0;
    };
}