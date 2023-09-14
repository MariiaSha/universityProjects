//Ships.cpp
#include <gl/glut.h>
#include "utils.h"
#include "Ships.h"

namespace SeaBattle
{
    void Ships::draw()
    {
        // Визначення властивостей матеріалу:
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, getAmbiColor());
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, getDiffColor());
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, getSpecColor());
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, GraphUtils::shininess);
        // Запис поточної матриці в стек
        // (збереження вмісту поточної матриці для подальшого використання):
        glPushMatrix();
        glTranslatef(getXCenter(), getYCenter() + getYSize() / 5, getZCenter());
        // паралелепіпед повинен бути розташований у вертикальному напрямку:
        glRotatef(90, 1, 0, 0);
        GraphUtils::parallelepiped(getXSize(), getYSize(), getZSize());
        // Відновлення поточної матриці зі стека:
        glPopMatrix();
    }

}