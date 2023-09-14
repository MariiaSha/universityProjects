#include <gl/glut.h>
#include "Squares.h"
#include "utils.h"

namespace SeaBattle
{
    void Squares::draw()
    {
        // Визначення властивостей матеріалу:
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, getAmbiColor());
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, getDiffColor());
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, getSpecColor());
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, GraphUtils::shininess);
        // Запис поточної матриці в стек
        // (збереження вмісту поточної матриці для подальшого використання):
        glPushMatrix();
        glTranslatef(getXCenter(), getYCenter() / 3, getZCenter());
        // Паралелепіпед повинен бути розташований у вертикальному напрямку:
        glRotatef(90, 1, 0, 0);
        GraphUtils::parallelepiped(getXSize() * 3, getXSize() * 3, getZSize() / 5);
        // Відновлення поточної матриці зі стека:
        glPopMatrix();
    }

}