#include "cylinder.h"
#include <cmath>
#include "glassert.h"

namespace tresta {

    Shape::Shape() :
        mVertexPositionBuffer(QOpenGLBuffer::VertexBuffer),
        mVertexNormalBuffer(QOpenGLBuffer::VertexBuffer),
        mIndexBuffer(QOpenGLBuffer::IndexBuffer)
    {
    }

    void Shape::prepareVertexBuffers() {
        mVAO.create();
        mVAO.bind();

        mVertexPositionBuffer.create();
        mVertexPositionBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
        mVertexPositionBuffer.bind();
        mVertexPositionBuffer.allocate(&vertices[0], vertices.size() * sizeof(float));

        mVertexNormalBuffer.create();
        mVertexNormalBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
        mVertexNormalBuffer.bind();
        mVertexNormalBuffer.allocate(&normals[0], normals.size() * sizeof(float));

        mIndexBuffer.create();
        mIndexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
        mIndexBuffer.bind();
        mIndexBuffer.allocate(&indices[0], indices.size() * sizeof(short));

        mVAO.release();
    }

} // namespace tresta
