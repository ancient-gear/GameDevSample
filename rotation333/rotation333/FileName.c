#include <stdio.h>
#include <math.h>
#include <windows.h>

#define WIDTH 40
#define HEIGHT 20

#define M_PI 3.14159265358979323846

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

// 이동 행렬 생성
Matrix3x3 createTranslationMatrix(float tx, float ty) {
    Matrix3x3 mat;
    initIdentityMatrix(&mat);
    mat.m[0][2] = tx; // X축 이동
    mat.m[1][2] = ty; // Y축 이동
    return mat;
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

// 행렬 곱셈
Matrix3x3 multiplyMatrices(Matrix3x3 a, Matrix3x3 b) {
    Matrix3x3 result;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result.m[i][j] = a.m[i][0] * b.m[0][j] + a.m[i][1] * b.m[1][j] + a.m[i][2] * b.m[2][j];
        }
    }
    return result;
}

// vec3와 Matrix3x3의 곱
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
void draw(vec3 S, vec3 E, vec3 M) {
    clearScreen();

    // 화면 그리기
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if ((int)S.x == x && (int)S.y == y) {
                printf("S"); // S 출력
            }
            else if ((int)E.x == x && (int)E.y == y) {
                printf("E"); // E 출력
            }
            else if ((int)M.x == x && (int)M.y == y) {
                printf("M"); // M 출력
            }
            else {
                printf(" "); // 빈 공간 출력
            }
        }
        printf("\n");
    }
}

int main() {
    // S의 초기 위치
    vec3 S = { WIDTH / 2, HEIGHT / 2, 1 };

    // E의 초기 위치
    vec3 E = { 0.0f, 0.0f, 1.0f };
    // M의 초기 위치
    vec3 M = { 0.0f, 0.0f, 1.0f };

    float radiusE = 7; // E의 반지름
    float radiusM = 3;  // M의 반지름 (E보다 가까운 위치)

    for (int angle = 0; angle < 360; angle += 5) {
        // E의 위치 계산
        Matrix3x3 rotationMatrixE = createRotationMatrix(angle);
        E = Mul((vec3) { radiusE, 0, 1 }, rotationMatrixE); // E는 S를 중심으로 회전
        E.x += S.x; // S를 기준으로 X축 이동
        E.y += S.y; // S를 기준으로 Y축 이동

        // M의 위치 계산 (E보다 3배 빠르게 회전)
        Matrix3x3 rotationMatrixM = createRotationMatrix(angle * 3);
        M = Mul((vec3) { radiusM, 0, 1 }, rotationMatrixM); // M은 E를 중심으로 회전
        M.x += E.x; // E를 기준으로 X축 이동
        M.y += E.y; // E를 기준으로 Y축 이동

        draw(S, E, M); // 현재 위치를 그리기
        Sleep(100); // 0.1초 대기
    }
    return 0;
}
