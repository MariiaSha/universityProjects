// Ships.h
#ifndef Ships_h
#define Ships_h

#include "Shape.h"

namespace SeaBattle
{
    // Клас, який відповідає за малювання корабля
    class Ships : public Shape
    {
    public:
        Ships(float xCenter, float yCenter, float zCenter,
            float xSize, float ySize, float zSize,
            float* diffColor, float* ambiColor, float* specColor)
            : Shape(xCenter, yCenter, zCenter, xSize, ySize, zSize,
                specColor, diffColor, ambiColor) {
        }
        virtual void draw();
    };
}
#endif