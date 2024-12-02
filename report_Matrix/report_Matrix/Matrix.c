#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>  // _kbhit(), _getch() 함수 사용을 위한 헤더
#include <windows.h>
#include <math.h>  // sin(), cos() 함수

#include "ElfMath.h"
#include "Planet.h"

#define WIDTH 80
#define HEIGHT 24

// 전역 변수로 스크린 버퍼 선언
char screenBuffer[WIDTH * HEIGHT * 2];
char clearBuffer[WIDTH * HEIGHT * 2];   // 클리어 버퍼 (검정 배경)

void SetConsoleToUTF8() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

Planet sun = { {20.0f, 12.0f, 1.0f}, 5.0f, 3.0f, 0.0f, 0.0f, "SS" };
Planet earth = { {32.0f, 12.0f, 1.0f}, 3.0f, 2.0f, 3.0f, 12.0f, "EE" };
Planet moon = { {36.0f, 12.0f, 1.0f}, 1.5f, 1.0f, 2.0f, 4.0f, "MM" };

// 화면 초기화 함수
void Init() {
    // 화면 버퍼를 빈 공백으로 초기화
    for (int i = 0; i < WIDTH * HEIGHT * 2; i++) {
        screenBuffer[i] = ' ';  // 검정 배경, 문자 없음
        clearBuffer[i] = ' ';   // 클리어 버퍼도 같은 방식으로 초기화
    }

    // 커서 숨기기
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = FALSE;  // 커서를 숨김
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

// 화면을 그리는 함수
void Draw(char* Buffer) {
    // 0,0으로 이동
    COORD coord = { 0, 0 };  // 좌표 (0, 0)
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

    // 스크린 버퍼 출력
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH / 2; x++) {
            putchar(Buffer[y * WIDTH + x * 2]);
            putchar(Buffer[y * WIDTH + x * 2 + 1]);
        }
        putchar('\n');
    }
}

void DrawSun(Planet* planet, Vector3 vertices[]) {
    int size = (int)planet->size;

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH / 2; x++) {
            float alpha = ((vertices[1].y - vertices[2].y) * (x - vertices[2].x) + (vertices[2].x - vertices[1].x) * (y - vertices[2].y)) /
                ((vertices[1].y - vertices[2].y) * (vertices[0].x - vertices[2].x) + (vertices[2].x - vertices[1].x) * (vertices[0].y - vertices[2].y));
            float beta = ((vertices[2].y - vertices[0].y) * (x - vertices[2].x) + (vertices[0].x - vertices[2].x) * (y - vertices[2].y)) /
                ((vertices[1].y - vertices[2].y) * (vertices[0].x - vertices[2].x) + (vertices[2].x - vertices[1].x) * (vertices[0].y - vertices[2].y));
            float gamma = 1.0f - alpha - beta;

            if (alpha >= 0 && beta >= 0 && gamma >= 0) {
                int bufferIndex = y * WIDTH + x * 2;
                if (bufferIndex >= 0 && bufferIndex < WIDTH * HEIGHT * 2) {
                    screenBuffer[bufferIndex] = planet->symbol[0];
                    screenBuffer[bufferIndex + 1] = planet->symbol[1];
                }
            }
        }
    }
}

void UpdateSun(Planet* planet, float deltaTime) {
    static float accumulatedTime = 0.0f;
    accumulatedTime += deltaTime;

    float angle = -360.0f * (accumulatedTime / planet->rotationPeriod); // 3초 동안 360도 회전
    Matrix3x3 rotationMatrix = rotation_matrix(angle);

    // 정삼각형의 각 꼭짓점 좌표 계산
    Vector3 vertices[3] = {
        {0.0f, -planet->size, 1.0f},
        {-planet->size, (sqrtf(3.0f) / 2.0f * planet->size), 1.0f},
        {planet->size, (sqrtf(3.0f) / 2.0f * planet->size), 1.0f}
    };

    // 각 꼭짓점을 회전시킴
    for (int i = 0; i < 3; i++) {
        vertices[i] = multiply_matrix_vector(rotationMatrix, vertices[i]);
        vertices[i].x += planet->position.x;
        vertices[i].y += planet->position.y;
    }

    DrawSun(planet, vertices);
}



void DrawEarth(Planet* planet, Vector3 vertices[]) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH / 2; x++) {
            Vector3 A = vertices[0];
            Vector3 B = vertices[1];
            Vector3 C = vertices[2];
            Vector3 D = vertices[3];

            float areaTotal = 0.0f;
            float area1 = fabsf((A.x - x) * (B.y - y) - (B.x - x) * (A.y - y)) / 2.0f;
            float area2 = fabsf((B.x - x) * (C.y - y) - (C.x - x) * (B.y - y)) / 2.0f;
            float area3 = fabsf((C.x - x) * (D.y - y) - (D.x - x) * (C.y - y)) / 2.0f;
            float area4 = fabsf((D.x - x) * (A.y - y) - (A.x - x) * (D.y - y)) / 2.0f;

            areaTotal = area1 + area2 + area3 + area4;
            float areaRect = fabsf((B.x - A.x) * (D.y - A.y) - (D.x - A.x) * (B.y - A.y));

            if (fabsf(areaTotal - areaRect) < 1e-2) {
                int bufferIndex = y * WIDTH + x * 2;
                if (bufferIndex >= 0 && bufferIndex < WIDTH * HEIGHT * 2) {
                    screenBuffer[bufferIndex] = planet->symbol[0];
                    screenBuffer[bufferIndex + 1] = planet->symbol[1];
                }
            }
        }
    }
}

void UpdateEarthRevolution(Planet* planet, Planet* center, float deltaTime) {
    static float accumulatedTime = 0.0f;
    accumulatedTime += deltaTime;

    // 공전 주기를 통해 각도 계산
    float angle = 360.0f * (accumulatedTime / planet->revolutionPeriod);
    Matrix3x3 rotationMatrix = rotation_matrix(angle);

    // 초기 지구의 위치 설정 (태양 중심으로부터의 거리)
    Vector3 position = { planet->distance, 0.0f, 1.0f };

    // 회전 행렬을 적용하여 새로운 위치 계산
    position = multiply_matrix_vector(rotationMatrix, position);

    // 새로운 위치를 중심점에 더하여 최종 위치 계산
    planet->position.x = position.x + center->position.x;
    planet->position.y = position.y + center->position.y;
}

void UpdateEarthRotation(Planet* planet, float deltaTime) {
    // 자전 주기를 통해 각도 계산
    float angle = 360.0f * (deltaTime / planet->rotationPeriod);
    Matrix3x3 rotationMatrix = rotation_matrix(angle);

    // 정사각형의 중심점 계산
    float size = planet->size * sqrt(2);
    float halfSize = size / 2;

    // 초기 꼭짓점 좌표 계산
    Vector3 vertices[4] = {
        {planet->position.x - halfSize, planet->position.y - halfSize, 1.0f},
        {planet->position.x + halfSize, planet->position.y - halfSize, 1.0f},
        {planet->position.x + halfSize, planet->position.y + halfSize, 1.0f},
        {planet->position.x - halfSize, planet->position.y + halfSize, 1.0f}
    };

    // 각 꼭짓점을 중심점 기준으로 회전
    for (int i = 0; i < 4; i++) {
        // 중심점 기준으로 회전
        float offsetX = vertices[i].x - planet->position.x;
        float offsetY = vertices[i].y - planet->position.y;

        vertices[i].x = planet->position.x + (offsetX * cosf(angle) - offsetY * sinf(angle));
        vertices[i].y = planet->position.y + (offsetX * sinf(angle) + offsetY * cosf(angle));
    }

    // 회전된 꼭짓점 좌표를 통해 정사각형 그리기
    DrawEarth(planet, vertices);
}

void DrawMoon(Planet* planet) {
    int centerX = (int)planet->position.x;
    int centerY = (int)planet->position.y;
    int size = (int)planet->size ;

    for (int y = -size; y <= size; y++) {
        for (int x = -size; x <= size; x++) {
            if (x * x + y * y <= size * size) {
                int bufferIndex = (centerY + y) * WIDTH + (centerX + x) * 2;
                if (bufferIndex >= 0 && bufferIndex < WIDTH * HEIGHT * 2) {
                    screenBuffer[bufferIndex] = planet->symbol[0];
                    screenBuffer[bufferIndex + 1] = planet->symbol[1];
                }
            }
        }
    }
}

void UpdateMoonRevolution(Planet* planet, Planet* center, float deltaTime) {
    static float accumulatedTime = 0.0f;
    accumulatedTime += deltaTime;

    // 달의 공전 각도 계산 (반시계 방향 회전)
    float angle = -360.0f * (accumulatedTime / planet->revolutionPeriod); // 반시계 방향으로 공전
    Matrix3x3 rotationMatrix = rotation_matrix(angle);

    Vector3 initialPosition = { planet->distance, 0.0f, 1.0f }; // 지구로부터의 초기 위치
    Vector3 newPosition = multiply_matrix_vector(rotationMatrix, initialPosition);

    planet->position.x = newPosition.x + center->position.x;
    planet->position.y = newPosition.y + center->position.y;
}

void UpdateMoonRotation(Planet* planet, float deltaTime) {
    static float accumulatedTime = 0.0f;
    accumulatedTime += deltaTime;

    // 달의 자전 각도 계산 (시계 방향 회전)
    float angle = 360.0f * (accumulatedTime / planet->rotationPeriod);
    Matrix3x3 rotationMatrix = rotation_matrix(angle);

    // 달의 자전 적용 (달 자체의 회전만 처리)
    DrawMoon(planet);
}

// 화면 업데이트 함수
void Update(float deltaTime) {
    for (int i = 0; i < WIDTH * HEIGHT * 2; i++) {
        screenBuffer[i] = ' ';
    }

    UpdateSun(&sun, deltaTime); // 태양의 자전
    UpdateEarthRevolution(&earth, &sun, deltaTime); // 지구의 공전
    UpdateEarthRotation(&earth, deltaTime); // 지구의 자전
    UpdateMoonRevolution(&moon, &earth, deltaTime); // 달의 자전
    UpdateMoonRotation(&moon, deltaTime); // 달의 공전
}


// 게임 루프
int main() {
    Init();

    clock_t lastTime = clock();
    int fps = 60;
    double frameTime = 1.0 / fps;

    int spacePressed = 0;

    while (1) {
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 27) {
                break;
            }
            else if (ch == ' ') {
                spacePressed++;

                if (spacePressed == 1) {
                    sun.position = (Vector3){ 20.0f, 12.0f, 1.0f };
                    earth.position = (Vector3){ 32.0f, 12.0f, 1.0f };
                    moon.position = (Vector3){ 36.0f, 12.0f, 1.0f };

                    Update(0);
                    Draw(screenBuffer);
                }
            }
        }

        if (spacePressed == 0) {
            const char* text = "20211416RyuHyunWook";
            int centerX = WIDTH / 4 - strlen(text) / 2; // 문자열 중앙 위치 계산
            int centerY = HEIGHT / 2;
            
            // 문자열을 출력할 위치 계산 및 버퍼에 저장
            for (int i = 0; text[i] != '\0'; ++i) {
                int bufferIndex = centerY * WIDTH + (centerX + i) * 2;
                if (bufferIndex >= 0 && bufferIndex < WIDTH * HEIGHT * 2) {
                    screenBuffer[bufferIndex] = text[i];
                    screenBuffer[bufferIndex + 1] = ' ';
                }
            }

            Draw(screenBuffer);
        }

        if (spacePressed > 1) {
            clock_t now = clock();
            double deltaTime = (double)(now - lastTime) / CLOCKS_PER_SEC;

            if (deltaTime >= frameTime) {
                lastTime = now;

                Update(deltaTime);

                Draw(screenBuffer);
            }
        }
    }

    return 0;
}