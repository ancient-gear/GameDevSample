#include <stdio.h>
#include <math.h>
#include <windows.h>

#define WIDTH 40
#define HEIGHT 20

#define M_PI 3.14159265358979323846

typedef struct {
    float m[3][3]; // 3x3 ���
} Matrix3x3;

typedef struct {
    float x, y, w; // ���� ��ǥ
} vec3;

// 3x3 ��� �ʱ�ȭ
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

// �̵� ��� ����
Matrix3x3 createTranslationMatrix(float tx, float ty) {
    Matrix3x3 mat;
    initIdentityMatrix(&mat);
    mat.m[0][2] = tx; // X�� �̵�
    mat.m[1][2] = ty; // Y�� �̵�
    return mat;
}

// ȸ�� ��� ����
Matrix3x3 createRotationMatrix(float angle) {
    Matrix3x3 mat;
    initIdentityMatrix(&mat);
    float radian = angle * M_PI / 180;
    mat.m[0][0] = cos(radian); // cos(��)
    mat.m[0][1] = -sin(radian); // -sin(��)
    mat.m[1][0] = sin(radian); // sin(��)
    mat.m[1][1] = cos(radian); // cos(��)
    return mat;
}

// ��� ����
Matrix3x3 multiplyMatrices(Matrix3x3 a, Matrix3x3 b) {
    Matrix3x3 result;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result.m[i][j] = a.m[i][0] * b.m[0][j] + a.m[i][1] * b.m[1][j] + a.m[i][2] * b.m[2][j];
        }
    }
    return result;
}

// vec3�� Matrix3x3�� ��
vec3 Mul(vec3 a, Matrix3x3 b) {
    vec3 result;
    result.x = a.x * b.m[0][0] + a.y * b.m[0][1] + b.m[0][2];
    result.y = a.x * b.m[1][0] + a.y * b.m[1][1] + b.m[1][2];
    result.w = 1.0f; // ���� ��ǥ�� w�� 1�� ����
    return result;
}

// �ܼ� ȭ�� �����
void clearScreen() {
    system("cls");
}

// ȭ�� �׸���
void draw(vec3 S, vec3 E, vec3 M) {
    clearScreen();

    // ȭ�� �׸���
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if ((int)S.x == x && (int)S.y == y) {
                printf("S"); // S ���
            }
            else if ((int)E.x == x && (int)E.y == y) {
                printf("E"); // E ���
            }
            else if ((int)M.x == x && (int)M.y == y) {
                printf("M"); // M ���
            }
            else {
                printf(" "); // �� ���� ���
            }
        }
        printf("\n");
    }
}

int main() {
    // S�� �ʱ� ��ġ
    vec3 S = { WIDTH / 2, HEIGHT / 2, 1 };

    // E�� �ʱ� ��ġ
    vec3 E = { 0.0f, 0.0f, 1.0f };
    // M�� �ʱ� ��ġ
    vec3 M = { 0.0f, 0.0f, 1.0f };

    float radiusE = 7; // E�� ������
    float radiusM = 3;  // M�� ������ (E���� ����� ��ġ)

    for (int angle = 0; angle < 360; angle += 5) {
        // E�� ��ġ ���
        Matrix3x3 rotationMatrixE = createRotationMatrix(angle);
        E = Mul((vec3) { radiusE, 0, 1 }, rotationMatrixE); // E�� S�� �߽����� ȸ��
        E.x += S.x; // S�� �������� X�� �̵�
        E.y += S.y; // S�� �������� Y�� �̵�

        // M�� ��ġ ��� (E���� 3�� ������ ȸ��)
        Matrix3x3 rotationMatrixM = createRotationMatrix(angle * 3);
        M = Mul((vec3) { radiusM, 0, 1 }, rotationMatrixM); // M�� E�� �߽����� ȸ��
        M.x += E.x; // E�� �������� X�� �̵�
        M.y += E.y; // E�� �������� Y�� �̵�

        draw(S, E, M); // ���� ��ġ�� �׸���
        Sleep(100); // 0.1�� ���
    }
    return 0;
}
