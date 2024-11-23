#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

#define WIDTH 40
#define HEIGHT 20

#define M_PI 3.14159265358979323846

void hideCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;  // 커서 숨기기
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

typedef struct {
    float m[3][3]; // 3x3 행렬
} Matrix3x3;

typedef struct {
    float x, y, w; // 점의 좌표
} vec3;

// 3x3 행렬 초기화
void initIdentityMatrix(Matrix3x3* mat) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (i == j) {
                mat->m[i][j] = 1;
            }
            else {
                mat->m[i][j] = 0;
            }
        }
    }
}

// 회전 행렬 생성
Matrix3x3 createRotationMatrix(float angle) {
    Matrix3x3 mat;
    initIdentityMatrix(&mat);
    float radian = angle * M_PI / 180;
    mat.m[0][0] = cos(radian); // cos(θ)
    mat.m[0][1] = -sin(radian); // -sin(θ)
    mat.m[1][0] = sin(radian); // sin(θ)
    mat.m[1][1] = cos(radian); // cos(θ)
    return mat;
}

// 행렬과 벡터 곱셈
vec3 Mul(vec3 a, Matrix3x3 b) {
    vec3 result;
    result.x = a.x * b.m[0][0] + a.y * b.m[0][1] + b.m[0][2];
    result.y = a.x * b.m[1][0] + a.y * b.m[1][1] + b.m[1][2];
    result.w = 1.0f; // 동차 좌표의 w는 1로 설정
    return result;
}

// 콘솔 화면 지우기
void clearScreen() {
    system("cls");
}

// 화면 그리기
void draw(float rect[4][2], int width, int height) {
    clearScreen();

    // 화면 그리기
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // 직사각형 내부 채우기
            int inside = 0;
            for (int i = 0; i < 4; i++) {
                // 점 (x, y)가 사각형 내부에 있으면 채우기
                int j = (i + 1) % 4;
                float x1 = rect[i][0], y1 = rect[i][1];
                float x2 = rect[j][0], y2 = rect[j][1];

                // 교차 여부 체크
                if (((y1 > y) != (y2 > y)) && (x < (x2 - x1) * (y - y1) / (y2 - y1) + x1)) {
                    inside = !inside;
                }
            }
            if (inside) {
                printf("*");  // 채운 부분은 '*'로 출력
            }
            else {
                printf(" ");  // 나머지는 공백
            }
        }
        printf("\n");
    }
}

int main() {
    hideCursor();
    // 사각형의 크기
    int rect_width = 11;
    int rect_height = 5;

    // 사각형의 4개의 꼭짓점 (사각형의 중심을 기준으로 초기화)
    float rect[4][2] = {
        {-rect_width / 2, -rect_height / 2},  // 좌상단
        {rect_width / 2, -rect_height / 2},   // 우상단
        {rect_width / 2, rect_height / 2},    // 우하단
        {-rect_width / 2, rect_height / 2}    // 좌하단
    };

    // 사각형의 중심
    float cx = WIDTH / 2;
    float cy = HEIGHT / 2;

    // 회전 각도 초기화
    float angle = 0.0f;

    // 총 9초 동안 실행 (1초에 120도씩 회전 -> 360도 회전 / 3번 회전)
    // 3번 회전하려면 총 9초 동안 1080도 회전
    // 1초에 30프레임, 3초 동안 360도 회전하므로, 프레임마다 각도는 120도 / 30프레임 = 4도씩 증가해야 함
    int totalFrames = 3 * 30 + 1;  // 3초 동안 30fps 기준으로 3번 회전 (총 9초 + 1프레임 추가)
    float angleIncrement = 360.0f / (30.0f);  // 각 프레임마다 증가할 각도 (360도 / 30프레임)

    // 3번 회전하며 실행
    for (int frame = 0; frame < totalFrames; frame++) {
        // 회전 행렬 생성
        Matrix3x3 rotationMatrix = createRotationMatrix(angle);

        // 사각형의 각 꼭짓점 회전
        float rotatedRect[4][2];
        for (int i = 0; i < 4; i++) {
            vec3 point = { rect[i][0], rect[i][1], 1.0f };
            vec3 rotatedPoint = Mul(point, rotationMatrix);
            rotatedRect[i][0] = rotatedPoint.x + cx; // 중심 이동
            rotatedRect[i][1] = rotatedPoint.y + cy; // 중심 이동
        }

        draw(rotatedRect, WIDTH, HEIGHT); // 회전된 사각형 그리기

        // 각도 증가 (매 프레임마다 12도씩)
        angle += angleIncrement;

        Sleep(33); // 33ms 대기 (약 30 FPS)
    }

    return 0;
}