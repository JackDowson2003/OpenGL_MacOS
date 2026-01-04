#pragma once

#include <vector>
#include <glad/glad.h>
#include "Renderer.h"

/***
 * for implement example as
 * glEnableVertexAttribArray(0)
 * glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0)
 */
struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    bool normalized;

    static unsigned int GetSizeOfType(unsigned int type)
    {
        switch (type)
        {
            case GL_FLOAT:        return 4;
            case GL_UNSIGNED_INT: return 1;
            case GL_BYTE:         return 4;
            default:
                ASSERT(false);
                return 0;
        }
    }
};


class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;

public:
    VertexBufferLayout()
            : m_Stride(0)
    {};

    template<typename>
    inline static constexpr bool always_false = false;

    template<typename T>
    void Push(unsigned int count)
    {
        static_assert(always_false<T>, "Unsupported VertexBuffer type");
    }

    template<>
    inline void Push<float>(unsigned int count)
    {
        m_Elements.push_back({GL_FLOAT, count, GL_FALSE});
        this->m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
    }

    template<>
    inline void Push<unsigned int>(unsigned int count)
    {
        m_Elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
        this->m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
    }

    template<>
    inline void Push<unsigned char>(unsigned int count)
    {
        m_Elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
        this->m_Stride += count *  VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) ;
    }

    inline unsigned int GetStride() const
    {
        return m_Stride;
    }

    inline const std::vector<VertexBufferElement>& GetElement() const
    {
        return m_Elements;
    }
};
