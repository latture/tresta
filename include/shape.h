#ifndef TRESTA_SHAPE_H
#define TRESTA_SHAPE_H

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include <vector>

namespace tresta {

    class Shape
    {
    public:
        Shape();
        virtual ~Shape() {};
        virtual void initialize() = 0;
        void prepareVertexBuffers();

        std::vector<float> vertices;
        std::vector<float> normals;
        std::vector<unsigned short> indices;

        QOpenGLVertexArrayObject mVAO;
        QOpenGLBuffer mVertexPositionBuffer;
        QOpenGLBuffer mVertexNormalBuffer;
        QOpenGLBuffer mIndexBuffer;
    };

} // namespace tresta

#endif // TRESTA_SHAPE_H
