#ifndef TRESTA_SPHERE_H
#define TRESTA_SPHERE_H

#include "shape.h"

namespace tresta {

    class Sphere : public Shape
    {
    public:
        Sphere();

        void initialize();

    private:
        const float radius;
        const unsigned int dims;
    };

} // namespace tresta

#endif // TRESTA_SPHERE_H
