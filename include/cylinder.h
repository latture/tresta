#ifndef TRESTA_CYLINDER_H
#define TRESTA_CYLINDER_H

#include "shape.h"

namespace tresta {

    class Cylinder : public Shape
    {
    public:
        Cylinder();

        void initialize();

    private:
        const float radius;
        const unsigned int dims;
    };

} // namespace tresta

#endif // TRESTA_CYLINDER_H
