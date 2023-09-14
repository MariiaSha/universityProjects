#include <gl/glut.h>
#include "Squares.h"
#include "utils.h"

namespace SeaBattle
{
    void Squares::draw()
    {
        // ���������� ������������ ��������:
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, getAmbiColor());
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, getDiffColor());
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, getSpecColor());
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, GraphUtils::shininess);
        // ����� ������� ������� � ����
        // (���������� ����� ������� ������� ��� ���������� ������������):
        glPushMatrix();
        glTranslatef(getXCenter(), getYCenter() / 3, getZCenter());
        // ������������ ������� ���� ������������ � ������������� ��������:
        glRotatef(90, 1, 0, 0);
        GraphUtils::parallelepiped(getXSize() * 3, getXSize() * 3, getZSize() / 5);
        // ³��������� ������� ������� � �����:
        glPopMatrix();
    }

}