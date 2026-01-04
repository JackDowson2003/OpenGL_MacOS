//
// Created by chenlong on 2025/12/31.
//

#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray()
{

    GLCall(glGenVertexArrays(1, &m_RendererID));
//    GLCall(glBindVertexArray(m_RendererID));  //转移到Bind 方法了
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout)
{
    Bind();
    vb.Bind();
    const auto& elements = layout.GetElement();
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        unsigned int offset = 0;
        const auto& element = elements[i];
        GLCall(glEnableVertexAttribArray(i));
        //遵守连续分配的原则 （逻辑上）
        GLCall(glVertexAttribPointer(i, element.count, element.type,
                                     element.normalized, layout.GetStride(), reinterpret_cast<const void *>(offset)));
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}

void VertexArray::Bind() const
{
    GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::UnBind() const
{
    GLCall(glBindVertexArray(0));
}

