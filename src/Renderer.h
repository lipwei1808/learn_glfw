#pragma once

#include <GL/glew.h>
#include "VertexArray.h"
#include "Shader.h"
#include "IndexBuffer.h"

#define ASSERT(x) if (!(x)) __builtin_trap();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

class Renderer {
public:
    Renderer();
    ~Renderer();
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
private:
};