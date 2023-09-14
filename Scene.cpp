// Scene.cpp
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h> 
#include <gl/glut.h>
#include <stdio.h>
#include "Scene.h"
#include "utils.h"
#include "Board.h"
#include "Squares.h"

namespace SeaBattle
{
    using namespace GraphUtils;

    // ��������� ������������ - ������� �� �������� ��������:
    Scene::Scene(float xStep, float zStep)
    {
        this->xStep = xStep;
        this->zStep = zStep;
        // ������ ����� 
        // ������ ��������� ���, ��� ��������� �� ������:
        shapes.push_back(board = new Board(0, 0.0, 0.0, N * xStep, 0.1, M * xStep, diffBlue, ambiBlue, specBlue));
        // ������ ������:
        for (int i = 0; i < M; i++)
        {
            for (int j = 0; j < N; j++)
            {
                shapes.push_back(squares[j] = new Squares(allocX(j), 0.2, allocZ(i), 0.05, 0.2, 0.05, diffLightBlue, ambiLightBlue, specLightBlue));
            }
        } 
        // ������ ������ �� ������:
        shapes.push_back(marker[0] = new Ships(allocX(0), 0.1, allocZ(4), 0.1, 0.1, 0.1, diffRed, ambiRed, specRed));
        shapes.push_back(ships[0] = new Ships(allocX(0), 0.1, allocZ(0), 0.1, 0.1, 0.1, diffYellow, ambiYellow, specYellow));
        shapes.push_back(ships[1] = new Ships(allocX(1), 0.1, allocZ(0), 0.1, 0.1, 0.1, diffYellow, ambiYellow, specYellow));
        shapes.push_back(ships[2] = new Ships(allocX(2), 0.1, allocZ(0), 0.1, 0.1, 0.1, diffYellow, ambiYellow, specYellow));

        // ��������� ����������� ��������� ����� ������ ����:
        initialize();
    }

    Scene::~Scene()
    {
        // ��������� �� ������:
        for (int i = 0; i < shapes.size(); i++)
        {
            delete shapes[i];
        }
    }

    void Scene::setEnemy() { // ������������ �� ������� ������� �������
        for (int i = 0; i < numOfShips; i++)
        {
            fields1[3][i] = 1;
        }
    }

    void Scene::setMyShips() { // ������������ �� ������� ����� �������
        for (int i = 0; i < numOfShips; i++)
        {
            fields1[0][i] = 1;
        }
    }

    // ����������� ������, � ����� ������������ ��������� ������� �� �������
    void Scene::resetArr()
    {
        for (int i = 0; i < M; i++)
        {
            for (int j = 0; j < N; j++)
            {
                shapes[(j + i * N) + 1]->setColors(diffLightBlue, ambiLightBlue, specLightBlue);
            }
        }
        // ������ ���� ������
        for (int i = 0; i < M; i++)
        {
            for (int j = 0; j < N; j++)
            {
                fields1[i][j] = 0;
            }
        }
        setEnemy();
        setMyShips();
    }

    // ������������� ������� ������ fields � ���������� x
    float Scene::allocX(int i)
    {
        return  xStep * i - (N - 1) * xStep / 2;
    }

    // ������������� ������� ������ fields � ���������� z
    float Scene::allocZ(int i)
    {
        return  zStep * i - (M - 1) * zStep / 2;
    }

    // ������������ ������� �������� �� ����� ������ fields
    void Scene::allocateShips()
    {
        int count = 0;
        for (int i = 0; i < halfM; i++)
        {
            for (int j = 0; j < N; j++)
            {
                if (fields1[i][j] > 0)
                {
                    ships[count]->setCoords(allocX(j), 0.1, allocZ(i));
                    count++;
                }
            }
        }
    }

    // ������������ �������
    void Scene::allocateMarker()
    {
        marker[0]->setCoords(allocX(0), 0.1, allocZ(5));
    }

    void Scene::upMarker(int x, int z)
    {
        if (x < 0 || z < 0 || x >= N || z >= M)
        {
            return;
        }
        marker[0]->setYCenter(0.3);
    }

    bool Scene::moveMarker(int xFrom, int zFrom, int xTo, int zTo)
    {
        // �������� ��������� ����������:
        if (xFrom < 0 || zFrom < halfM || xFrom >= N || zFrom >= M)
        {
            return false;
        }
        if (xTo < 0 || zTo < halfM || xTo >= N || zTo >= M)
        {
            return false;
        }
        if (fields1[zTo][xTo] == 2 || fields1[zTo][xTo] == 3)
        {
            return false;
        }
        if (xFrom == xTo && zFrom == zTo)
        {
            return false;
        }
        if (xFrom - xTo > 1 || xTo - xFrom > 1 || zFrom - zTo > 1 || zTo - zFrom > 1)
        {
            return false;
        }
        if (marker[fields1[zFrom][xFrom]]->getYCenter() < 0.2)
        {
            return false;
        }

        // ����������:
        marker[0]->setXCenter(allocX(xTo));
        marker[0]->setZCenter(allocZ(zTo));
        myRound(xTo, zTo);
        return true;
    }

    // ��������� �������
    void Scene::downMarker()
    {
        marker[0]->setYCenter(0.1);
    }

    // ����������� ����� (���������� ��������, � ���� � ������ ���������� ���):
    void Scene::initialize()
    {
        resetArr();      // ��������� ���������� ������ fields
        // "���������" ������. ������� ���������� ����� � C++ ���������� �� 
        // �����������, ��������� ���� ���������� �� ���� ������������ ������:
        GraphUtils::shuffle((int*)fields1, halfM * N);
        GraphUtils::shuffle((int*)fields1 + halfM * N, halfM * N);
        allocateShips(); // ������������ ������� �������� �� ������ fields
        allocateMarker();
        // ����������� �������� �����:
        distZ = -2;
        angleX = -10;
        angleY = 30;
        time = 0;
        isMyRound = true;
        botsHit = 0;
        myHit = 0;
        finish = false;
    }

    // ���� ������� ����� ����
    void Scene::changeStage(int x, int z) {
        if (fields1[z][x] == 3) { //������
            shapes[(x + z * N) + 1]->setColors(diffBlue, ambiBlue, specBlue);
        }
        if (fields1[z][x] == 2) { //�������
            shapes[(x + z * N) + 1]->setColors(diffWhite, ambiWhite, specWhite);
        }
    }

    void Scene::botRound() { // ��� ����
        int BotX = rand() % N;
        int BotZ = rand() % halfM;
        // �������� ���������� ���� BotX �� BotZ

        if (fields1[BotZ][BotX] == 1) //��� ������ �� ������ �������
        {
            fields1[BotZ][BotX] = 2;
            changeStage(BotX, BotZ);
            botsHit++;
            isMyRound = false;
        }
        else if (fields1[BotZ][BotX] == 0) { // ��� �� ������
            fields1[BotZ][BotX] = 3;
            changeStage(BotX, BotZ);
            isMyRound = true;
        }
    }

    void Scene::myRound(int x, int z) {
        if (fields1[z][x] == 1) // ������ �� �������� �������
        {
            fields1[z][x] = 2;
            changeStage(x, z);
            myHit++;
            isMyRound = true;
        }
        else if (fields1[z][x] == 0) // ������
        {
            fields1[z][x] = 3;
            changeStage(x, z);
            isMyRound = false;
        }
    }

    // ����� ������, ��������� �� ������� ������� ����:
    bool Scene::findNearest(int x, int y, int& x1, int& z1)
    {
        int viewport[4];
        int iMin = -1, jMin = -1;
        double mvMatrix[16], projMatrix[16];
        double minDist = 2000;

        for (int i = 0; i < M; i++)
        {
            for (int j = 0; j < N; j++)
            {

                // ����� x, y, z ��������� �������:
                double wx = allocX(j);
                double wy = 0.1;
                double wz = allocZ(i);

                // ���������� ����� viewport �������� ������� ���������:
                glGetIntegerv(GL_VIEWPORT, viewport);

                // ���������� ������ ��������� ���������:
                glGetDoublev(GL_MODELVIEW_MATRIX, mvMatrix);
                glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);

                // ����� x, y, z ����������, �� �����������:
                double dx, dy, dz;

                // �������� ���������� �����, �� ��� ������������ �������� ��������:
                gluProject(wx, wy, wz, mvMatrix, projMatrix, viewport, &dx, &dy, &dz);
                dy = viewport[3] - dy - 1; // dy ��������� ������������
                double d = (x - dx) * (x - dx) + (y - dy) * (y - dy); // ������� ������
                if (d < minDist) // ������� ������� ��������
                {
                    minDist = d;
                    iMin = i;
                    jMin = j;
                }
            }
        }
        if (minDist < 1000) // ������� ���������� ��������
        {
            x1 = jMin;
            z1 = iMin;
            return true;
        }
        else
        {
            return false;
        }
    }

    // ���������� ��䳿, ���'����� � �������������� ����
    void Scene::on_paint()
    {
        char text[128]; // ����� �������, 
        // ���������� ������ ������� �������� �� ����� ���:
        if (finish)
        {
            if (myWin) {
                sprintf(text, "Game over. Player's win. Time: %d sec.   F2 - Restart game   Esc - Exit", time);
            }
            else {
                sprintf(text, "Game over. Computer's win. Time: %d sec.   F2 - Restart game   Esc - Exit", time); 
            }
            
        }
        else
        {
            sprintf(text, "F2 - Restart game   Esc - Exit              Time: %d sec.", time);
        }
        // ������������ ������� ��������� ����, ��� ���� �������� ��� ����:
        glViewport(0, 0, width, height);

        // ���������� ��������� �������� � ������� �����:
        float lightAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // ���� �������� ��������� 
        float lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // ���� ��������� ��������� 
        float lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };// ���� ������������ �����������
        float lightPosition[] = { 1.0f, 1.0f, 1.0f, 0.0f };// ������������ ������� �����

        // ������������ ��������� ������� �����:
        glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

        // ��������� ��������� ���� ��� ��������:
        if (finish)
        {
            glClearColor(0, 0.7, 0.7, 0);
        }
        else
        {
            glClearColor(0, 0.5, 0.5, 0);
        }

        // ������� ������:
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPushMatrix();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        // ��� ����������� ������, ����� ��������������� ������������ ��������:
        glOrtho(0, 1, 0, 1, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glColor3f(1, 1, 0); // ������ �����
        drawString(GLUT_BITMAP_TIMES_ROMAN_24, text, 0.01, 0.95);
        glPopMatrix();

        // �������� ����� ������ � �������� ��������:
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        // ������ ������� ����� �������� � ����������� ������ ���������, 
        // 60 - ��� �������� � �������� �� �� �,
        // width/height - ��� �������� ������ �� x,
        // 1 � 100 - ������� �� ����������� �� ������ �������� �� ������:
        gluPerspective(60, width / height, 1, 100);

        // �������� ����� ������ � ������� ��������:
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0, 0, distZ);	// ������ � ������� ��������� ��������� �� distZ, 

        glRotatef(angleX, 0.0f, 1.0f, 0.0f);  // ���� ���������� �� �� Oy
        glRotatef(angleY, 1.0f, 0.0f, 0.0f);  // ���� ���������� �� �� Ox
        glEnable(GL_DEPTH_TEST);	// �������� ����� ������� (��� �������� ��������� ������ ����������)

        // �������� ����� ��� ��������� ���������:
        glEnable(GL_LIGHTING);

        // ������ ������� ����� � 0 (�� ���� ���� �� 8), ����� ���� ������ � "����":
        glEnable(GL_LIGHT0);

        // ������� �� ������:
        for (int i = 0; i < shapes.size(); i++)
        {
            shapes[i]->draw();
        }

        // �������� ���, �� ��������:
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glFlush();
        // �������� ����:
        glutSwapBuffers(); // ����������� ������
    }

    // ���������� ��䳿, ���'����� � ����� ������ ���� 
    void Scene::on_size(int width, int height)
    {
        this->width = width;
        if (height == 0)
            height = 1;
        this->height = height;
    }

    // ���������� ����, ���'������ � ����������� ������ ����
    void Scene::on_mouse(int button, int state, int x, int y)
    {
        // �������� ������ ���������� ����:
        mouseX = x;
        mouseY = y;
        if ((state == GLUT_UP)) // ������ ������
        {
            downMarker();
            
            // �������� ��������� ���:
            if (botsHit == numOfShips) // �������� 
            {
                myWin = false;
                finish = true;
            }
            if (myHit == numOfShips) // ��������
            {
                myWin = true;
                finish = true;
            }
            this->button = -1;  // ���� ������ �� ���������
            return;
        }
        this->button = button;  // �������� ���������� ��� ������
        if (finish)
        {
            return;
        }
        // �������� ���� ��� �����������:
        if (button == 0 && findNearest(x, y, xFrom, zFrom) && isMyRound == true)
        {
            upMarker(xFrom, zFrom);
        }
        else if (button == 0 && isMyRound == false) {
            botRound();
        }

    }

    // ���������� ����, ���'������ � ������������ ���� � ���������� �������
    void Scene::on_motion(int x, int y)
    {
        switch (button)
        {
        case 0: // ��� ������ - ����������� �������
            if (finish)
                break;
            if (findNearest(x, y, xTo, zTo) && isMyRound == true)
            {
                moveMarker(xFrom, zFrom, xTo, zTo);
                xFrom = xTo;
                zFrom = zTo;
            }
            break;
        case 2: // ����� ������ - ��������� �����
            angleX += x - mouseX;
            angleY += y - mouseY;
            mouseX = x;
            mouseY = y;
            break;
        }
    }

    // ���������� ����, ���'������ � ����������� �������������� ����� � ������ 
    void Scene::on_special(int key, int x, int y)
    {
        switch (key) {
        case GLUT_KEY_UP:   // ����������
            if (distZ > -1.7)
            {
                break;
            }
            distZ += 0.1;
            break;
        case GLUT_KEY_DOWN: // ���������
            distZ -= 0.1;
            break;
        case GLUT_KEY_F2:   // ���� ���
            initialize();
            break;
        }
    }

    int tick = 0; // ��������, �������� ����� ��������� ���� 25 ��

    // ���������� ��䳿 �� �������
    void Scene::on_timer(int value)
    {
        tick++;
        if (tick >= 40) // ���������� �������� �������
        {
            if (!finish)// ������� �����������, ���� ��� �� ��������
            {
                time++;
            }
            tick = 0;   // ������� ��������
        }
        on_paint();     // ��������� ������������� ����
    }
}