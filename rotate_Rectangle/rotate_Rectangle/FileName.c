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
    cursorInfo.bVisible = FALSE;  // Ŀ�� �����
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

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

// ��İ� ���� ����
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
void draw(float rect[4][2], int width, int height) {
    clearScreen();

    // ȭ�� �׸���
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // ���簢�� ���� ä���
            int inside = 0;
            for (int i = 0; i < 4; i++) {
                // �� (x, y)�� �簢�� ���ο� ������ ä���
                int j = (i + 1) % 4;
                float x1 = rect[i][0], y1 = rect[i][1];
                float x2 = rect[j][0], y2 = rect[j][1];

                // ���� ���� üũ
                if (((y1 > y) != (y2 > y)) && (x < (x2 - x1) * (y - y1) / (y2 - y1) + x1)) {
                    inside = !inside;
                }
            }
            if (inside) {
                printf("*");  // ä�� �κ��� '*'�� ���
            }
            else {
                printf(" ");  // �������� ����
            }
        }
        printf("\n");
    }
}

int main() {
    hideCursor();
    // �簢���� ũ��
    int rect_width = 11;
    int rect_height = 5;

    // �簢���� 4���� ������ (�簢���� �߽��� �������� �ʱ�ȭ)
    float rect[4][2] = {
        {-rect_width / 2, -rect_height / 2},  // �»��
        {rect_width / 2, -rect_height / 2},   // ����
        {rect_width / 2, rect_height / 2},    // ���ϴ�
        {-rect_width / 2, rect_height / 2}    // ���ϴ�
    };

    // �簢���� �߽�
    float cx = WIDTH / 2;
    float cy = HEIGHT / 2;

    // ȸ�� ���� �ʱ�ȭ
    float angle = 0.0f;

    // �� 9�� ���� ���� (1�ʿ� 120���� ȸ�� -> 360�� ȸ�� / 3�� ȸ��)
    // 3�� ȸ���Ϸ��� �� 9�� ���� 1080�� ȸ��
    // 1�ʿ� 30������, 3�� ���� 360�� ȸ���ϹǷ�, �����Ӹ��� ������ 120�� / 30������ = 4���� �����ؾ� ��
    int totalFrames = 3 * 30 + 1;  // 3�� ���� 30fps �������� 3�� ȸ�� (�� 9�� + 1������ �߰�)
    float angleIncrement = 360.0f / (30.0f);  // �� �����Ӹ��� ������ ���� (360�� / 30������)

    // 3�� ȸ���ϸ� ����
    for (int frame = 0; frame < totalFrames; frame++) {
        // ȸ�� ��� ����
        Matrix3x3 rotationMatrix = createRotationMatrix(angle);

        // �簢���� �� ������ ȸ��
        float rotatedRect[4][2];
        for (int i = 0; i < 4; i++) {
            vec3 point = { rect[i][0], rect[i][1], 1.0f };
            vec3 rotatedPoint = Mul(point, rotationMatrix);
            rotatedRect[i][0] = rotatedPoint.x + cx; // �߽� �̵�
            rotatedRect[i][1] = rotatedPoint.y + cy; // �߽� �̵�
        }

        draw(rotatedRect, WIDTH, HEIGHT); // ȸ���� �簢�� �׸���

        // ���� ���� (�� �����Ӹ��� 12����)
        angle += angleIncrement;

        Sleep(33); // 33ms ��� (�� 30 FPS)
    }

    return 0;
}