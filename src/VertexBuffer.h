//
// Created by chenlong on 2025/12/31.
//

#pragma once

class VertexBuffer
{
private:
    unsigned int m_RendererID;
public:
    VertexBuffer(const void* data,unsigned int size);
    ~VertexBuffer();

    void Bind() const;
    void UnBind() const;
};