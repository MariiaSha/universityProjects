//Ships.cpp
#include <gl/glut.h>
#include "utils.h"
#include "Ships.h"

namespace SeaBattle
{
    void Ships::draw()
    {
        // ���������� ������������ ��������:
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, getAmbiColor());
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, getDiffColor());
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, getSpecColor());
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, GraphUtils::shininess);
        // ����� ������� ������� � ����
        // (���������� ����� ������� ������� ��� ���������� ������������):
        glPushMatrix();
        glTranslatef(getXCenter(), getYCenter() + getYSize() / 5, getZCenter());
        // ������������ ������� ���� ������������ � ������������� ��������:
        glRotatef(90, 1, 0, 0);
        GraphUtils::parallelepiped(getXSize(), getYSize(), getZSize());
        // ³��������� ������� ������� � �����:
        glPopMatrix();
    }

}