//
// Created by chenlong on 2025/12/31.
//
#include "Renderer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include <iostream>
#include <glad/glad.h>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char *fuc, const char *file, int line)
{
    if (GLenum error = glGetError())
    {
        std::cerr << "[OpenGL Error]: (" << error << ") function:\t"
                  << fuc << " file:\t" << file << ":\t" << line
                  << std::endl;
        return false;
    }
    return true;
}

void Renderer::Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const
{
//    shader.Bind();
    va.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}
