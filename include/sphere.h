#ifndef TRESTA_SPHERE_H
#define TRESTA_SPHERE_H

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

namespace tresta {

    class Sphere
    {
    public:
        Sphere();

        void initialize();
        void prepareVertexBuffers();

        std::vector<float> vertices;
        std::vector<float> normals;
        std::vector<float> offsets;
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

#endif // TRESTA_SPHERE_H
