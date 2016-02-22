#pragma once
#include <math.h> 
#include <random>

namespace utils {
	// ��������������� �������

	// ������� ��������. t = [0, 1], ��� ����� � 1, ��� ���� true �������� �� false
	bool ease(float t) {
		t += 0.5f;
		return cos(t * t * t * 30.0f) > 0.f;
	}

	// ��������� ������ [min, max]. � PopCap ��� int ���
	int random(int min, int max) {
		random_device rd;
		mt19937 generator(rd());
		uniform_int_distribution<int> distribution(min, max);

		return distribution(generator);
	}
}