#ifndef PLANET_H
#define PLANET_H

#include "ElfMath.h"

// 행성의 데이터를 나타내는 구조체
typedef struct {
    Vector3 position;   // 행성의 위치 (중심점)
    float size;         // 행성의 크기
    float rotationPeriod; // 자전주기
    float revolutionPeriod; // 공전주기
    float distance;  // 기준으로부터의 거리
    char symbol[2]; // 행성의 내부를 채울 문자
} Planet;

#endif