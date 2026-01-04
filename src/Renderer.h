//
// Created by chenlong on 2025/12/31.
//
#pragma once


// 前向声明（必须）
class VertexArray;

class IndexBuffer;

class Shader;

#if defined(_WIN32)
#include <intrin.h>
#define DEBUG_BREAK() __debugbreak()
#elif defined(__APPLE__)
#define DEBUG_BREAK() __builtin_debugtrap()
#else
#include <csignal>
#define DEBUG_BREAK() raise(SIGTRAP)
#endif

#define ASSERT(X) if (!(X)) DEBUG_BREAK()
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char *fuc, const char *file, int line);


class Renderer
{
private:
public:
    void Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const;

    void Clear() const;
};
