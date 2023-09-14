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
    const int M = 6, N = 6;     // кількість рядків та колонок поля
    const int halfM = 3;        // розділення поля на дві частини
    const int numOfShips = 3;   // кулькість кораблів
    const int numOfMark = 1;    // кількість маркерів

    // Основний клас гри, який представляє геометрію сцени,
    // контролює правила гри, розташування елементів,
    // а також реалізує обробку подій GLUT
    class Scene
    {
        std::vector<Shape*> shapes; // "гнучкий" масив указівників на елементи гри
        int button;           // кнопка миші (-1 - не натиснута, 0 - ліва, 2 - права)
        float angleX, angleY; // поточний кут повороту сцени 
        float mouseX, mouseY; // поточні координати
        float width, height;  // Розміри вікна
        float distZ;          // відстань по осі Z до сцени
        float xStep, zStep;   // відстань між окремими клітинами
        bool finish;          // ознака того, що гру завершено

        bool isMyRound;       // чия черга
        int botsHit;          // кількість влучань бота
        int myHit;            // кількість влучань гравця
        bool myWin;           // перемога гравця

        Ships* ships[numOfShips];      // масив указівників на кораблі
        Ships* marker[numOfMark];      // маркер
        Squares* squares[M];           // клітини поля
        Board* board;                  // дошка
        
        int fields1[M][N];      // масив, у якому відображається розміщення кораблів:
        // 0 - корабля немає, 1 - є корабель, 2 - влучили, 3 - не влучили
        int time;               // поточний час у секуднах
        int xFrom, zFrom;       // індекси клітини, з якої починається пересування
        int xTo, zTo;           // індекси клітини, якою закінчується пересування
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
