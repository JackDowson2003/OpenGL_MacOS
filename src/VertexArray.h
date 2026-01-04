//
// Created by chenlong on 2025/12/31.
//
#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
private:
    unsigned int m_RendererID;
public:

    VertexArray();
    ~VertexArray();

    void AddBuffer(const VertexBuffer& vb,const VertexBufferLayout& layout);

    void Bind() const;

    void UnBind() const;

};
