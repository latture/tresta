#ifndef TRESTA_CYLINDER_H
#define TRESTA_CYLINDER_H

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include <vector>

namespace tresta {

    class Cylinder
    {
    public:
        Cylinder();

        void initialize();
        void prepareVertexBuffers();

        std::vector<float> vertices;
        std::vector<float> normals;
        std::vector<unsigned short> indices;

        QOpenGLVertexArrayObject mVAO;
        QOpenGLBuffer mVertexPositionBuffer;
        QOpenGLBuffer mVertexNormalBuffer;
        QOpenGLBuffer mIndexBuffer;

    private:
        const float radius;
        const unsigned int dims;
    };

} // namespace tresta

#endif // TRESTA_CYLINDER_H
