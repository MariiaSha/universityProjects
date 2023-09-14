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

    // Параметри конструктора - відстань між окремими клітинами:
    Scene::Scene(float xStep, float zStep)
    {
        this->xStep = xStep;
        this->zStep = zStep;
        // Додаємо дошку 
        // Розміри визначаємо так, щоб поміщалися всі клітини:
        shapes.push_back(board = new Board(0, 0.0, 0.0, N * xStep, 0.1, M * xStep, diffBlue, ambiBlue, specBlue));
        // Додаємо клітини:
        for (int i = 0; i < M; i++)
        {
            for (int j = 0; j < N; j++)
            {
                shapes.push_back(squares[j] = new Squares(allocX(j), 0.2, allocZ(i), 0.05, 0.2, 0.05, diffLightBlue, ambiLightBlue, specLightBlue));
            }
        } 
        // Додаємо маркер та кораблі:
        shapes.push_back(marker[0] = new Ships(allocX(0), 0.1, allocZ(4), 0.1, 0.1, 0.1, diffRed, ambiRed, specRed));
        shapes.push_back(ships[0] = new Ships(allocX(0), 0.1, allocZ(0), 0.1, 0.1, 0.1, diffYellow, ambiYellow, specYellow));
        shapes.push_back(ships[1] = new Ships(allocX(1), 0.1, allocZ(0), 0.1, 0.1, 0.1, diffYellow, ambiYellow, specYellow));
        shapes.push_back(ships[2] = new Ships(allocX(2), 0.1, allocZ(0), 0.1, 0.1, 0.1, diffYellow, ambiYellow, specYellow));

        // Здійснюємо ініціалізацію параметрів перед першою грою:
        initialize();
    }

    Scene::~Scene()
    {
        // Видаляємо всі фігури:
        for (int i = 0; i < shapes.size(); i++)
        {
            delete shapes[i];
        }
    }

    void Scene::setEnemy() { // розташування та кількість ворожих кораблів
        for (int i = 0; i < numOfShips; i++)
        {
            fields1[3][i] = 1;
        }
    }

    void Scene::setMyShips() { // розташування та кількість наших кораблів
        for (int i = 0; i < numOfShips; i++)
        {
            fields1[0][i] = 1;
        }
    }

    // Ініціалізація масиву, в якому відображається розміщення кораблв та маркеру
    void Scene::resetArr()
    {
        for (int i = 0; i < M; i++)
        {
            for (int j = 0; j < N; j++)
            {
                shapes[(j + i * N) + 1]->setColors(diffLightBlue, ambiLightBlue, specLightBlue);
            }
        }
        // клітини поки порожні
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

    // Перерахування індексу масиву fields в координату x
    float Scene::allocX(int i)
    {
        return  xStep * i - (N - 1) * xStep / 2;
    }

    // Перерахування індексу масиву fields в координату z
    float Scene::allocZ(int i)
    {
        return  zStep * i - (M - 1) * zStep / 2;
    }

    // Розташування кораблів відповідно до вмісту масиву fields
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

    // Розташування маркеру
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
        // Перевірка можливості переміщення:
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

        // Переміщення:
        marker[0]->setXCenter(allocX(xTo));
        marker[0]->setZCenter(allocZ(zTo));
        myRound(xTo, zTo);
        return true;
    }

    // Опускання маркеру
    void Scene::downMarker()
    {
        marker[0]->setYCenter(0.1);
    }

    // Ініціалізація даних (виконується спочатку, а потім з кожним оновленням гри):
    void Scene::initialize()
    {
        resetArr();      // початкове заповнення масиву fields
        // "Тасування" масиву. Оскільки двовимірний масив у C++ зберігається як 
        // одновимірний, здійснюємо його приведення до типу одновимірного масиву:
        GraphUtils::shuffle((int*)fields1, halfM * N);
        GraphUtils::shuffle((int*)fields1 + halfM * N, halfM * N);
        allocateShips(); // розташування кораблів відповідно до масиву fields
        allocateMarker();
        // Ініціалізація елементів даних:
        distZ = -2;
        angleX = -10;
        angleY = 30;
        time = 0;
        isMyRound = true;
        botsHit = 0;
        myHit = 0;
        finish = false;
    }

    // зміна кольору клітин поля
    void Scene::changeStage(int x, int z) {
        if (fields1[z][x] == 3) { //промах
            shapes[(x + z * N) + 1]->setColors(diffBlue, ambiBlue, specBlue);
        }
        if (fields1[z][x] == 2) { //влучили
            shapes[(x + z * N) + 1]->setColors(diffWhite, ambiWhite, specWhite);
        }
    }

    void Scene::botRound() { // хід бота
        int BotX = rand() % N;
        int BotZ = rand() % halfM;
        // отримали координати бота BotX та BotZ

        if (fields1[BotZ][BotX] == 1) //Бот влучив по нашому кораблю
        {
            fields1[BotZ][BotX] = 2;
            changeStage(BotX, BotZ);
            botsHit++;
            isMyRound = false;
        }
        else if (fields1[BotZ][BotX] == 0) { // бот не влучив
            fields1[BotZ][BotX] = 3;
            changeStage(BotX, BotZ);
            isMyRound = true;
        }
    }

    void Scene::myRound(int x, int z) {
        if (fields1[z][x] == 1) // попали по ворожому кораблю
        {
            fields1[z][x] = 2;
            changeStage(x, z);
            myHit++;
            isMyRound = true;
        }
        else if (fields1[z][x] == 0) // промах
        {
            fields1[z][x] = 3;
            changeStage(x, z);
            isMyRound = false;
        }
    }

    // Пошук клітини, найближчої до позиції курсору миші:
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

                // Світові x, y, z поточного стрижня:
                double wx = allocX(j);
                double wy = 0.1;
                double wz = allocZ(i);

                // Заповнюємо масив viewport поточною областю перегляду:
                glGetIntegerv(GL_VIEWPORT, viewport);

                // Заповнюємо масиви поточними матрицями:
                glGetDoublev(GL_MODELVIEW_MATRIX, mvMatrix);
                glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);

                // Світові x, y, z координати, що обертаються:
                double dx, dy, dz;

                // Отримуємо координати точки, на яку спроектовано поточний стрижень:
                gluProject(wx, wy, wz, mvMatrix, projMatrix, viewport, &dx, &dy, &dz);
                dy = viewport[3] - dy - 1; // dy необхідно перерахувати
                double d = (x - dx) * (x - dx) + (y - dy) * (y - dy); // квадрат відстані
                if (d < minDist) // знайшли ближчий стрижень
                {
                    minDist = d;
                    iMin = i;
                    jMin = j;
                }
            }
        }
        if (minDist < 1000) // знайшли найближчий стрижень
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

    // Оброблювач події, пов'язаної з перемалюванням вікна
    void Scene::on_paint()
    {
        char text[128]; // Масив символів, 
        // Заповнення масиву символів відповідно до стану гри:
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
        // Встановлюємо область перегляду таку, щоб вона вміщувала все вікно:
        glViewport(0, 0, width, height);

        // Ініціалізуємо параметри матеріалів і джерела світла:
        float lightAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // колір фонового освітлення 
        float lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // колір дифузного освітлення 
        float lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };// колір дзеркального відображення
        float lightPosition[] = { 1.0f, 1.0f, 1.0f, 0.0f };// розташування джерела світла

        // Встановлюємо параметри джерела світла:
        glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

        // Визначаємо блакитний колір для очищення:
        if (finish)
        {
            glClearColor(0, 0.7, 0.7, 0);
        }
        else
        {
            glClearColor(0, 0.5, 0.5, 0);
        }

        // Очищуємо буфери:
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPushMatrix();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        // Для відображення тексту, краще використовувати ортографічну проекцію:
        glOrtho(0, 1, 0, 1, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glColor3f(1, 1, 0); // жовтий текст
        drawString(GLUT_BITMAP_TIMES_ROMAN_24, text, 0.01, 0.95);
        glPopMatrix();

        // Включаємо режим роботи з матрицею проекцій:
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        // Задаємо усічений конус видимості в лівосторонній системі координат, 
        // 60 - кут видимості в градусах по осі у,
        // width/height - кут видимості уздовж осі x,
        // 1 и 100 - відстань від спостерігача до площин відсікання по глибині:
        gluPerspective(60, width / height, 1, 100);

        // Включаємо режим роботи з видовою матрицею:
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0, 0, distZ);	// камера з початку координат зсувається на distZ, 

        glRotatef(angleX, 0.0f, 1.0f, 0.0f);  // потім обертається по осі Oy
        glRotatef(angleY, 1.0f, 0.0f, 0.0f);  // потім обертається по осі Ox
        glEnable(GL_DEPTH_TEST);	// включаємо буфер глибини (для відсікання невидимих частин зображення)

        // Включаємо режим для установки освітлення:
        glEnable(GL_LIGHTING);

        // Додаємо джерело світла № 0 (їх може бути до 8), зараз воно світить з "очей":
        glEnable(GL_LIGHT0);

        // Малюємо усі фігури:
        for (int i = 0; i < shapes.size(); i++)
        {
            shapes[i]->draw();
        }

        // Вимикаємо все, що включили:
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glFlush();
        // показуємо вікно:
        glutSwapBuffers(); // перемикання буферів
    }

    // Оброблювач події, пов'язаної зі зміною розмірів вікна 
    void Scene::on_size(int width, int height)
    {
        this->width = width;
        if (height == 0)
            height = 1;
        this->height = height;
    }

    // Оброблювач подій, пов'язаних з натисканням кнопок миші
    void Scene::on_mouse(int button, int state, int x, int y)
    {
        // Зберігаємо поточні координати миші:
        mouseX = x;
        mouseY = y;
        if ((state == GLUT_UP)) // кнопка віджата
        {
            downMarker();
            
            // Перевірка закінчення гри:
            if (botsHit == numOfShips) // перемога 
            {
                myWin = false;
                finish = true;
            }
            if (myHit == numOfShips) // перемога
            {
                myWin = true;
                finish = true;
            }
            this->button = -1;  // ніяка кнопка не натиснута
            return;
        }
        this->button = button;  // зберігаємо інформацію про кнопки
        if (finish)
        {
            return;
        }
        // Вибираємо диск для пересування:
        if (button == 0 && findNearest(x, y, xFrom, zFrom) && isMyRound == true)
        {
            upMarker(xFrom, zFrom);
        }
        else if (button == 0 && isMyRound == false) {
            botRound();
        }

    }

    // Оброблювач подій, пов'язаних з пересуванням миші з натисненою кнопкою
    void Scene::on_motion(int x, int y)
    {
        switch (button)
        {
        case 0: // ліва кнопка - пересування маркеру
            if (finish)
                break;
            if (findNearest(x, y, xTo, zTo) && isMyRound == true)
            {
                moveMarker(xFrom, zFrom, xTo, zTo);
                xFrom = xTo;
                zFrom = zTo;
            }
            break;
        case 2: // права кнопка - обертання сцени
            angleX += x - mouseX;
            angleY += y - mouseY;
            mouseX = x;
            mouseY = y;
            break;
        }
    }

    // Оброблювач подій, пов'язаних з натисненням функціональних клавіш і стрілок 
    void Scene::on_special(int key, int x, int y)
    {
        switch (key) {
        case GLUT_KEY_UP:   // наближення
            if (distZ > -1.7)
            {
                break;
            }
            distZ += 0.1;
            break;
        case GLUT_KEY_DOWN: // віддалення
            distZ -= 0.1;
            break;
        case GLUT_KEY_F2:   // нова гра
            initialize();
            break;
        }
    }

    int tick = 0; // лічильник, значення якого змінюється кожні 25 мс

    // Оброблювач події від таймера
    void Scene::on_timer(int value)
    {
        tick++;
        if (tick >= 40) // нарахували наступну секунду
        {
            if (!finish)// секунди нарощуються, якщо гру не закінчено
            {
                time++;
            }
            tick = 0;   // скинули лічильник
        }
        on_paint();     // здійснюємо перемалювання вікна
    }
}