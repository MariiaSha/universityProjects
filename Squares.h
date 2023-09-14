// Squares.h
#ifndef Squares_h
#define Squares_h

#include "shape.h"

namespace SeaBattle
{
    // Клас, який відповідає за малювання квадрату
    class Squares : public Shape
    {
    public:
        Squares(float xCenter, float yCenter, float zCenter,
            float xSize, float ySize, float zSize,
            float* diffColor, float* ambiColor, float* specColor)
            : Shape(xCenter, yCenter, zCenter,
                xSize, ySize, zSize,
                specColor, diffColor, ambiColor) { }
        virtual void draw();
    };
}
#endif