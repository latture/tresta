#include "cylinder.h"
#include <cmath>
#include "glassert.h"

namespace tresta {

    Cylinder::Cylinder() :
        vertices(0),
        normals(0),
        indices(0),
        mVertexPositionBuffer(QOpenGLBuffer::VertexBuffer),
        mVertexNormalBuffer(QOpenGLBuffer::VertexBuffer),
        mIndexBuffer(QOpenGLBuffer::IndexBuffer),
        radius(0.05),
        dims(3)
    {
    }

    void Cylinder::initialize() {

        const unsigned int sectors = 25;
        const unsigned int rings = 2;

        const float pi = 3.141592653589793238f;

        const float r = 1.0f / ((float) rings - 1);
        const float s = 1.0f / ((float) sectors - 1);

        float x, y, z;
        unsigned int idx = 0;

        vertices.resize(rings * sectors * dims);
        normals.resize(rings * sectors * dims);
        indices.resize(rings * sectors * 2 * dims);

        for (unsigned int i = 0; i < rings; ++i)
        {
            for (unsigned int j = 0; j < sectors; ++j)
            {
                x = std::cos(2.0f * pi * j * s);
                y = std::sin(2.0f * pi * j * s);
                z = r * i;

                vertices[dims * idx]     = radius * x;
                vertices[dims * idx + 1] = z;
                vertices[dims * idx + 2] = radius * y;

                normals[dims * idx]     = x;
                normals[dims * idx + 1] = z;
                normals[dims * idx + 2] = y;

                ++idx;
            }
        }

        idx = 0;
        for (unsigned int i = 0; i < rings - 1; ++i)
        {
            for (unsigned int j = 0; j < sectors - 1; ++j)
            {
                indices[2 * dims * idx]     = i * sectors + j;
                indices[2 * dims * idx + 1] = (i + 1) * sectors + j + 1;
                indices[2 * dims * idx + 2] = i * sectors + j + 1;

                indices[2 * dims * idx + 3] = i * sectors + j;
                indices[2 * dims * idx + 4] = (i + 1) * sectors + j;
                indices[2 * dims * idx + 5] = (i + 1) * sectors + j + 1;

                ++idx;
            }
        }

        // reorder vertices and normals to be CCW for correct back face culling when rendered
        std::reverse(vertices.begin(), vertices.end());
        std::reverse(normals.begin(), normals.end());

        // add disks onto ends of cylinder:
        // center point on bottom disk
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);

        normals.push_back(0.0f);
        normals.push_back(-1.0f);
        normals.push_back(0.0f);

        // center point on top disk
        vertices.push_back(0.0f);
        vertices.push_back(1.0f);
        vertices.push_back(0.0f);

        normals.push_back(0.0f);
        normals.push_back(1.0f);
        normals.push_back(0.0f);

        // disk ring vertices
        for (unsigned int i = 0; i < 2; ++i) {
            for (unsigned int j = 0; j < sectors; ++j)
            {
                x = std::cos(2.0f * pi * j * s);
                y = std::sin(2.0f * pi * j * s);
                z = static_cast<float>(i);

                vertices.push_back(radius * x);
                vertices.push_back(z);
                vertices.push_back(radius * y);

                normals.push_back(0.0f);
                normals.push_back(i == 0 ? -1.0f : 1.0f);
                normals.push_back(0.0f);
            }
        }

         // bottom disk indices
         for (unsigned int i = 0; i < sectors - 1; ++i){
             indices.push_back(rings * sectors);
             indices.push_back(rings * sectors + 3 + i);
             indices.push_back(rings * sectors + 2 + i);

         }

         // top disk indices
         for (unsigned int i = 0; i < sectors - 1; ++i){
             indices.push_back(rings * sectors + 1);
             indices.push_back(rings * sectors + sectors + 2 + i);
             indices.push_back(rings * sectors + sectors + 3 + i);

         }
    }

    void Cylinder::prepareVertexBuffers() {
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
