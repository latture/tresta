#include "sphere.h"
#include "glassert.h"
#include <cmath>

namespace tresta {

    Sphere::Sphere() :
            Shape(),
            radius(0.05),
            dims(3) {
    }

    void Sphere::initialize() {
        const unsigned int sectors = 25;
        const unsigned int rings = 25;

        const float pi = 3.141592653589793238;

        const float r = 1.0f / ((float) rings - 1);
        const float s = 1.0f / ((float) sectors - 1);

        float x, y, z;
        unsigned int idx = 0;

        vertices.resize(rings * sectors * dims);
        normals.resize(rings * sectors * dims);
        indices.resize(rings * sectors * 2 * dims);

        for (unsigned int i = 0; i < rings; ++i) {
            for (unsigned int j = 0; j < sectors; ++j) {
                x = std::cos(2.0f * pi * j * s) * std::sin(pi * r * i);
                y = std::cos(pi * r * i);
                z = std::sin(2.0f * pi * j * s) * std::sin(pi * r * i);

                vertices[dims * idx] = radius * x;
                vertices[dims * idx + 1] = radius * y;
                vertices[dims * idx + 2] = radius * z;

                normals[dims * idx] = x;
                normals[dims * idx + 1] = y;
                normals[dims * idx + 2] = z;

                ++idx;
            }
        }

        idx = 0;
        for (unsigned int i = 0; i < rings - 1; ++i) {
            for (unsigned int j = 0; j < sectors - 1; ++j) {

                indices[2 * dims * idx] = i * sectors + j;
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
    }

} // namespace tresta
