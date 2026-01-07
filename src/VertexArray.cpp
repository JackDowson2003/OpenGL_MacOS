//
// Created by chenlong on 2025/12/31.
//
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"

#include <iostream>

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
    const auto &elements = layout.GetElement();
    unsigned int offset = 0; ///每次的步长要跨越之前的节点后继续进行 只能放在外部
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto &element = elements[i];
        std::cout << "element type is '" << element.type << "'  element count is '" << element.count
                  << "'  now offset is '" << offset << "'  now stride is '" << layout.GetStride() <<"'"<< std::endl;
        GLCall(glEnableVertexAttribArray(i));
        //遵守连续分配的原则 （逻辑上）
        /***
         * layout(location = 0) in vec4 position;
           layout(location = 1) in vec2 texCoord;
           插槽值 0 1的作用如上
         */
        GLCall(glVertexAttribPointer(i, element.count, element.type,
                                     element.normalized, layout.GetStride(), reinterpret_cast<const void *>(offset)));
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type); ///计算从连续内存中初始位置开始偏移多少
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

