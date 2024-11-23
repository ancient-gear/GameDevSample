#ifndef PLANET_H
#define PLANET_H

#include "ElfMath.h"

// �༺�� �����͸� ��Ÿ���� ����ü
typedef struct {
    Vector3 position;   // �༺�� ��ġ (�߽���)
    float size;         // �༺�� ũ��
    float rotationPeriod; // �����ֱ�
    float revolutionPeriod; // �����ֱ�
    float distance;  // �������κ����� �Ÿ�
    char symbol[2]; // �༺�� ���θ� ä�� ����
} Planet;

#endif