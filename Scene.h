// Scene.h
#ifndef Scene_h
#define Scene_h

#include "Shape.h"
#include "Ships.h"
#include "Squares.h"
#include "Board.h"
#include <vector>

namespace SeaBattle
{
    const int M = 6, N = 6;     // ������� ����� �� ������� ����
    const int halfM = 3;        // ��������� ���� �� �� �������
    const int numOfShips = 3;   // �������� �������
    const int numOfMark = 1;    // ������� �������

    // �������� ���� ���, ���� ����������� �������� �����,
    // ��������� ������� ���, ������������ ��������,
    // � ����� ������ ������� ���� GLUT
    class Scene
    {
        std::vector<Shape*> shapes; // "�������" ����� ��������� �� �������� ���
        int button;           // ������ ���� (-1 - �� ���������, 0 - ���, 2 - �����)
        float angleX, angleY; // �������� ��� �������� ����� 
        float mouseX, mouseY; // ������ ����������
        float width, height;  // ������ ����
        float distZ;          // ������� �� �� Z �� �����
        float xStep, zStep;   // ������� �� �������� ��������
        bool finish;          // ������ ����, �� ��� ���������

        bool isMyRound;       // ��� �����
        int botsHit;          // ������� ������� ����
        int myHit;            // ������� ������� ������
        bool myWin;           // �������� ������

        Ships* ships[numOfShips];      // ����� ��������� �� ������
        Ships* marker[numOfMark];      // ������
        Squares* squares[M];           // ������ ����
        Board* board;                  // �����
        
        int fields1[M][N];      // �����, � ����� ������������ ��������� �������:
        // 0 - ������� ����, 1 - � ��������, 2 - �������, 3 - �� �������
        int time;               // �������� ��� � ��������
        int xFrom, zFrom;       // ������� ������, � ��� ���������� �����������
        int xTo, zTo;           // ������� ������, ���� ���������� �����������
    public:
        Scene(float xStep, float zStep);
        ~Scene();
        void on_paint();
        void on_size(int width, int height);
        void on_mouse(int button, int state, int x, int y);
        void on_motion(int x, int y);
        void on_special(int key, int x, int y);
        void on_timer(int value);
    private:
        void initialize();
        void changeStage(int x, int z);
        void allocateShips();
        void allocateMarker();
        bool moveMarker(int xFrom, int zFrom, int xTo, int zTo);
        void upMarker(int x, int z);
        void downMarker();
        void botRound();
        void myRound(int x, int z);
        bool findNearest(int x, int z, int& x1, int& z1);
        void resetArr();
        void setEnemy();
        void setMyShips();
        float allocX(int i);
        float allocZ(int i);
    };
}

#endif
