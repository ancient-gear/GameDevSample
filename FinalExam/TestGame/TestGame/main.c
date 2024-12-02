#include "ElfMath.h"
#include "Elf2D.h"
#include "ElfTime.h"
#include "ElfObject.h"

#define WIDTH 60
#define HEIGHT 25

void Initialize(GameObject_Line * obj, int objNum)
{
    //��ü �ʱ�ȭ(���� 3��)
    int i = 0;
    for (i = 0; i < 3; i++)
    {
        obj[i].Position.x = 0;
        obj[i].Position.y = 0;

        obj[i].Rotation = 0;


        //(0,0) ~ (3,0) �� �մ� ����
        obj[i].Line[0].x = 0;
        obj[i].Line[0].y = 0;

        obj[i].Line[1].x = 3;
        obj[i].Line[1].y = 0;
    }

    obj[1].Rotation = 30;
    obj[2].Rotation = 30;

    //ȭ�� ��� �ɺ� �Է�
    obj[0].Symbol = "11";
    obj[1].Symbol = "22";
    obj[2].Symbol = "33";

    //ũ�� �Է�
    obj[0].Scale.x = 1;
    obj[0].Scale.y = 1;
    obj[1].Scale.x = 2;
    obj[1].Scale.y = 2;
    obj[2].Scale.x = 2;
    obj[2].Scale.y = 2;
}

// 0: �Է¾���
// 99 : ���� ����
int Input()
{
    // ESC Ű�� �������� Ȯ��
    if (_kbhit()) {
        char ch = _getch();
        if (ch == 27) {  // ESC Ű�� ������ ���� ����
            return 99;
        }
    }

    return 0;
}

void Update(GameObject_Line* obj, int objNum, int e)
{

}

void Render(GameObject_Line* obj, int objNum, char* Buf, int width, int height)
{
    Vector3 lineA, lineB;
    Matrix3x3 world = identity_matrix();    //������ķ� �ʱ�ȭ

    lineB.x = 0;
    lineB.y = 0;

    int i = 0;

    obj[i].Position.x = lineB.x;
    obj[i].Position.y = lineB.y;

    lineA.x = obj[i].Line[0].x;
    lineA.y = obj[i].Line[0].y;
    lineA.z = 1;
    lineB.x = obj[i].Line[1].x;
    lineB.y = obj[i].Line[1].y;
    lineB.z = 1;

    Matrix3x3 scale = scale_matrix(obj[i].Scale.x, obj[i].Scale.y); // ũ������ ��� ����
    Matrix3x3 rotation = rotation_matrix(obj[i].Rotation);// ȸ�� ��� ����
    Matrix3x3 translation = translation_matrix(obj[i].Position.x, obj[i].Position.y); // �̵� ��� ����

    world = multiply_matrices(scale, world);
    world = multiply_matrices(rotation, world);
    world = multiply_matrices(translation, world);
    lineA = multiply_matrix_vector(world, lineA);  // ���� ȸ�� ��� ���� (Matrix3x3 * Vector3)
    lineB = multiply_matrix_vector(world, lineB);  // ���� ȸ�� ��� ���� (Matrix3x3 * Vector3)

    Elf2DDrawLine((int)lineA.x, (int)lineA.y, (int)lineB.x, (int)lineB.y, Buf, width, height);
}


// ���� ����
int main() {
    int fps = 60;
    double frameTime = 1000.0 / fps;

    // ���� ������ ��ũ�� ���� ����
    char screenBuffer[(WIDTH + 1) * HEIGHT];
    int screenWidth = WIDTH;
    int screenHeight = HEIGHT;

    GameObject_Line LineObj[3];

    // ���� �ʱ�ȭ
    Initialize(LineObj, 3);

    Elf2DInitScreen();
    Elf2DClearScreen(screenBuffer, screenWidth, screenHeight);
    Elf2DDrawBuffer(screenBuffer);

    ElfTimeInitialize();

    int isGameRunning = 1;
    while (isGameRunning) {

        int gameEvent = Input();

        //�Է� ���
        if (gameEvent == 99)
        {
            isGameRunning = 0;
        }

        // ���� ������Ʈ
        Update(LineObj, 3, gameEvent);

        // ��ũ�����ۿ� ���� ������
        Render(LineObj, 3, screenBuffer, screenWidth, screenHeight);

        // ȭ�� �׸���
        Elf2DDrawBuffer(screenBuffer);

        ElfTimeCalculateDeltaTime();
        Elf2DSleep(frameTime - ElfTimeGetDeltaTime());
    }

    return 0;
}